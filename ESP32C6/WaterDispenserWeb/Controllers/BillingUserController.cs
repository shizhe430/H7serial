using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Filters;
using Microsoft.EntityFrameworkCore;
using WaterDispenserWeb.Data;
using WaterDispenserWeb.Models;
using WaterDispenserWeb.Services;
using WaterDispenserWeb.ViewModels;

namespace WaterDispenserWeb.Controllers;

[ServiceFilter(typeof(BillingUserAuthFilter))]
public class BillingUserController : Controller
{
    private readonly AppDbContext _db;
    private readonly WaterAdviceService _advice;
    private readonly BillingService _billing;
    private readonly DeepSeekService _deepSeek;

    public BillingUserController(AppDbContext db, WaterAdviceService advice, BillingService billing,
        DeepSeekService deepSeek)
    {
        _db = db;
        _advice = advice;
        _billing = billing;
        _deepSeek = deepSeek;
    }

    public async Task<IActionResult> Index()
    {
        var userId = HttpContext.Session.GetInt32("UserId") ?? 0;
        var user = await _db.Users.FindAsync(userId);
        if (user == null) return RedirectToAction("Login", "Account");

        var account = await _db.BillingAccounts.FirstOrDefaultAsync(a => a.UserId == userId);
        if (account == null)
        {
            account = new BillingAccount { UserId = userId, Balance = 0, RatePerMl = 0.0050m };
            _db.BillingAccounts.Add(account);
            await _db.SaveChangesAsync();
        }

        var today = DateTime.Today;
        var summary = await _db.DailySummaries.FirstOrDefaultAsync(d => d.UserId == userId && d.Date == today);
        var todayMl = summary?.TotalMl ?? 0;
        var todayCount = summary?.UseCount ?? 0;
        var targetMl = summary?.TargetMl ?? _advice.CalculateTargetMl(user.WeightKg);

        // 今日饮水记录
        var todayRecords = await _db.Records
            .Where(r => r.UserId == userId && r.RecordedAt >= today)
            .OrderByDescending(r => r.RecordedAt)
            .Take(20)
            .ToListAsync();

        // 近7天
        var weekData = new List<DailySummaryItem>();
        for (int i = 6; i >= 0; i--)
        {
            var date = today.AddDays(-i);
            var day = await _db.DailySummaries.FirstOrDefaultAsync(d => d.UserId == userId && d.Date == date);
            weekData.Add(new DailySummaryItem
            {
                DateStr = date.ToString("MM-dd"),
                TotalMl = day?.TotalMl ?? 0,
                UseCount = day?.UseCount ?? 0,
                TargetMl = day?.TargetMl ?? targetMl
            });
        }

        var balanceAdvice = _billing.GenerateBalanceAdvice(account.Balance, account.RatePerMl, todayMl);
        var (canDispense, dispenseMsg) = _billing.CheckCanDispense(account.Balance);

        // 计算剩余可饮水量（按每20ml=0.01元）
        int remainingMl = _billing.CalculateMaxMl(account.Balance);

        var model = new BillingDashboardViewModel
        {
            UserId = user.Id,
            Name = user.Name,
            Username = user.Username,
            Balance = account.Balance,
            TotalConsumed = account.TotalConsumed,
            RatePerMl = account.RatePerMl,
            RemainingMl = remainingMl,
            TodayMl = todayMl,
            TodayCount = todayCount,
            TargetMl = targetMl,
            CanDispense = canDispense,
            DispenseMessage = dispenseMsg,
            BalanceAdvice = balanceAdvice,
            Advice = summary?.Advice,
            AiAdvice = summary?.AiAdvice,
            TodayRecords = todayRecords.Select(r => new DailySummaryItem
            {
                DateStr = r.RecordedAt.ToString("HH:mm"),
                TotalMl = r.VolumeMl
            }).ToList(),
            RecentWeek = weekData
        };

        ViewBag.UserName = user.Name;
        return View(model);
    }

    /// <summary>修改密码</summary>
    [HttpGet]
    public IActionResult ChangePassword()
    {
        return View();
    }

    [HttpPost]
    public async Task<IActionResult> ChangePassword(string oldPassword, string newPassword)
    {
        var userId = HttpContext.Session.GetInt32("UserId") ?? 0;
        var user = await _db.Users.FindAsync(userId);
        if (user == null) return NotFound();

        if (user.Password != oldPassword)
        {
            ModelState.AddModelError("", "原密码错误");
            return View();
        }

        if (string.IsNullOrWhiteSpace(newPassword) || newPassword.Length < 4)
        {
            ModelState.AddModelError("", "新密码至少4位");
            return View();
        }

        user.Password = newPassword;
        await _db.SaveChangesAsync();
        TempData["Success"] = "密码修改成功！";
        return RedirectToAction("Index");
    }

    /// <summary>计费用户与 AI 多轮问答 - 只能问自己</summary>
    [HttpPost]
    public async Task<IActionResult> AskAI(int userId, string messages)
    {
        var sessionUserId = HttpContext.Session.GetInt32("UserId") ?? 0;
        if (userId != sessionUserId)
            return Json(new { ok = false, answer = "无权操作" });

        var user = await _db.Users.FindAsync(userId);
        if (user == null) return Json(new { ok = false, answer = "未找到该用户" });

        var today = DateTime.Today;
        var summary = await _db.DailySummaries.FirstOrDefaultAsync(d => d.UserId == userId && d.Date == today);
        var targetMl = summary?.TargetMl ?? _advice.CalculateTargetMl(user.WeightKg);

        var history = ParseMessages(messages);
        var answer = await _deepSeek.ChatAsync(
            user.Name, user.Age, user.Gender, user.HeightCm, user.WeightKg,
            summary?.TotalMl ?? 0, summary?.UseCount ?? 0, targetMl, history);

        return Json(new { ok = true, answer });
    }

    /// <summary>解析前端传来的对话历史 JSON</summary>
    private static List<(string role, string content)> ParseMessages(string messages)
    {
        var list = new List<(string role, string content)>();
        if (string.IsNullOrWhiteSpace(messages)) return list;
        try
        {
            using var doc = System.Text.Json.JsonDocument.Parse(messages);
            foreach (var m in doc.RootElement.EnumerateArray())
            {
                var role = m.GetProperty("role").GetString();
                var content = m.GetProperty("content").GetString();
                if (!string.IsNullOrEmpty(role) && content != null)
                    list.Add((role, content));
            }
        }
        catch { /* 忽略解析错误 */ }
        return list;
    }
}

public class BillingUserAuthFilter : IActionFilter
{
    public void OnActionExecuting(ActionExecutingContext context)
    {
        var role = context.HttpContext.Session.GetInt32("UserRole");
        var section = context.HttpContext.Session.GetString("UserSection");
        if (role != 3 || section != "billing")
            context.Result = new RedirectToActionResult("Login", "Account", new { section = "billing" });
    }

    public void OnActionExecuted(ActionExecutedContext context) { }
}
