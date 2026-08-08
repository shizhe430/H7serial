using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Filters;
using Microsoft.EntityFrameworkCore;
using WaterDispenserWeb.Data;
using WaterDispenserWeb.Models;
using WaterDispenserWeb.Services;
using WaterDispenserWeb.ViewModels;

namespace WaterDispenserWeb.Controllers;

[ServiceFilter(typeof(BillingAdminAuthFilter))]
public class BillingAdminController : Controller
{
    private readonly AppDbContext _db;
    private readonly WaterAdviceService _advice;
    private readonly BillingService _billing;

    public BillingAdminController(AppDbContext db, WaterAdviceService advice, BillingService billing)
    {
        _db = db;
        _advice = advice;
        _billing = billing;
    }

    /// <summary>管理员仪表盘</summary>
    public async Task<IActionResult> Index()
    {
        var today = DateTime.Today;
        var users = await _db.Users.Where(u => u.Section == "billing" && u.Role == 3).ToListAsync();
        var accounts = await _db.BillingAccounts.ToListAsync();
        var summaries = await _db.DailySummaries.Where(d => d.Date == today).ToListAsync();

        var data = users.Select(u =>
        {
            var acc = accounts.FirstOrDefault(a => a.UserId == u.Id);
            var sum = summaries.FirstOrDefault(s => s.UserId == u.Id);
            return new BillingUserOverview
            {
                UserId = u.Id,
                Name = u.Name,
                Username = u.Username,
                Balance = acc?.Balance ?? 0,
                TodayMl = sum?.TotalMl ?? 0,
                TodayCount = sum?.UseCount ?? 0,
                Advice = sum?.Advice ?? ""
            };
        }).ToList();

        ViewBag.RoleName = "管理员";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "管理员";
        ViewBag.Section = "billing";
        return View(data);
    }

    /// <summary>用户管理</summary>
    public async Task<IActionResult> Users()
    {
        var users = await _db.Users.Where(u => u.Section == "billing").OrderBy(u => u.Role).ThenBy(u => u.Username).ToListAsync();
        var accounts = await _db.BillingAccounts.ToListAsync();
        ViewBag.Accounts = accounts.ToDictionary(a => a.UserId);
        ViewBag.RoleName = "管理员";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "管理员";
        return View(users);
    }

    /// <summary>添加用户</summary>
    [HttpGet]
    public IActionResult AddUser()
    {
        ViewBag.RoleName = "管理员";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "管理员";
        return View(new User());
    }

    [HttpPost]
    public async Task<IActionResult> AddUser(User user, decimal initialBalance = 0)
    {
        if (string.IsNullOrWhiteSpace(user.Username))
        { ModelState.AddModelError("Username", "请输入登录编号"); return View(user); }
        if (string.IsNullOrWhiteSpace(user.Password))
        { ModelState.AddModelError("Password", "请输入密码"); return View(user); }
        if (string.IsNullOrWhiteSpace(user.Name))
        { ModelState.AddModelError("Name", "请输入姓名"); return View(user); }

        if (await _db.Users.AnyAsync(u => u.Username == user.Username))
        { ModelState.AddModelError("Username", "该编号已被使用"); return View(user); }

        user.Role = 3;
        user.Section = "billing";
        user.CreatedAt = DateTime.Now;
        user.UpdatedAt = DateTime.Now;
        if (string.IsNullOrWhiteSpace(user.Phone)) user.Phone = null;
        _db.Users.Add(user);
        await _db.SaveChangesAsync();

        // 创建计费账号
        var account = new BillingAccount
        {
            UserId = user.Id,
            Balance = initialBalance,
            RatePerMl = 0.0050m
        };
        _db.BillingAccounts.Add(account);
        await _db.SaveChangesAsync();

        TempData["Success"] = $"用户 {user.Name} 添加成功！初始余额 {initialBalance:F2}元";
        return RedirectToAction("Users");
    }

    /// <summary>编辑用户</summary>
    [HttpGet]
    public async Task<IActionResult> EditUser(int id)
    {
        var user = await _db.Users.FindAsync(id);
        if (user == null) return NotFound();
        ViewBag.RoleName = "管理员";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "管理员";
        return View(user);
    }

    [HttpPost]
    public async Task<IActionResult> EditUser(User user)
    {
        var dbUser = await _db.Users.FindAsync(user.Id);
        if (dbUser == null) return NotFound();

        if (await _db.Users.AnyAsync(u => u.Username == user.Username && u.Id != user.Id))
        { ModelState.AddModelError("Username", "该编号已被其他用户使用"); return View(user); }

        dbUser.Username = user.Username;
        if (!string.IsNullOrWhiteSpace(user.Password)) dbUser.Password = user.Password;
        dbUser.Name = user.Name;
        dbUser.Gender = user.Gender;
        dbUser.Age = user.Age;
        dbUser.HeightCm = user.HeightCm;
        dbUser.WeightKg = user.WeightKg;
        dbUser.Phone = string.IsNullOrWhiteSpace(user.Phone) ? null : user.Phone;
        dbUser.UpdatedAt = DateTime.Now;
        await _db.SaveChangesAsync();

        TempData["Success"] = $"用户 {dbUser.Name} 已更新";
        return RedirectToAction("Users");
    }

    /// <summary>删除用户</summary>
    [HttpPost]
    public async Task<IActionResult> DeleteUser(int id)
    {
        var user = await _db.Users.FindAsync(id);
        if (user == null) return NotFound();

        _db.Users.Remove(user);
        await _db.SaveChangesAsync();
        TempData["Success"] = $"用户 {user.Name} 已删除";
        return RedirectToAction("Users");
    }

    /// <summary>充值</summary>
    [HttpPost]
    public async Task<IActionResult> Recharge(int userId, decimal amount)
    {
        if (amount <= 0)
        { TempData["Error"] = "充值金额必须大于0"; return RedirectToAction("Users"); }

        var account = await _db.BillingAccounts.FirstOrDefaultAsync(a => a.UserId == userId);
        if (account == null)
        {
            account = new BillingAccount { UserId = userId, Balance = amount, RatePerMl = 0.0050m };
            _db.BillingAccounts.Add(account);
        }
        else
        {
            account.Balance += amount;
        }
        await _db.SaveChangesAsync();

        TempData["Success"] = $"充值成功！当前余额 {account.Balance:F2}元";
        return RedirectToAction("Users");
    }

    /// <summary>设置单价</summary>
    [HttpPost]
    public async Task<IActionResult> SetRate(int userId, decimal rate)
    {
        if (rate <= 0)
        { TempData["Error"] = "单价必须大于0"; return RedirectToAction("Users"); }

        var account = await _db.BillingAccounts.FirstOrDefaultAsync(a => a.UserId == userId);
        if (account == null) return NotFound();
        account.RatePerMl = rate;
        await _db.SaveChangesAsync();

        TempData["Success"] = $"单价已设置为 {rate:F4}元/ml";
        return RedirectToAction("Users");
    }
}

public class BillingAdminAuthFilter : IActionFilter
{
    public void OnActionExecuting(ActionExecutingContext context)
    {
        var role = context.HttpContext.Session.GetInt32("UserRole");
        var section = context.HttpContext.Session.GetString("UserSection");
        if (role != 0 || section != "billing")
            context.Result = new RedirectToActionResult("Login", "Account", new { section = "billing" });
    }

    public void OnActionExecuted(ActionExecutedContext context) { }
}
