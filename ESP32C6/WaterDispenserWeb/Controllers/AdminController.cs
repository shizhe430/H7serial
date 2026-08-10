using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Filters;
using Microsoft.EntityFrameworkCore;
using WaterDispenserWeb.Data;
using WaterDispenserWeb.Models;
using WaterDispenserWeb.Services;
using WaterDispenserWeb.ViewModels;

namespace WaterDispenserWeb.Controllers;

/// <summary>
/// 管理员控制器 - 需要管理员角色(role=0)
/// </summary>
[ServiceFilter(typeof(AdminAuthFilter))]
public class AdminController : Controller
{
    private readonly AppDbContext _db;
    private readonly WaterAdviceService _advice;
    private readonly DeepSeekService _deepSeek;
    private readonly PushPlusService _pushPlus;

    public AdminController(
        AppDbContext db,
        WaterAdviceService advice,
        DeepSeekService deepSeek,
        PushPlusService pushPlus)
    {
        _db = db;
        _advice = advice;
        _deepSeek = deepSeek;
        _pushPlus = pushPlus;
    }

    /// <summary>管理员首页仪表盘 - 所有饮水者今日概况</summary>
    public async Task<IActionResult> Index()
    {
        var today = DateTime.Today;
        var drinkers = await _db.Users.Where(u => u.Role == 2 && u.Section == "elderly").ToListAsync();
        var todaySummaries = await _db.DailySummaries
            .Where(d => d.Date == today)
            .ToListAsync();

        var overviews = new List<DrinkerOverview>();
        foreach (var d in drinkers)
        {
            var summary = todaySummaries.FirstOrDefault(s => s.UserId == d.Id);
            overviews.Add(new DrinkerOverview
            {
                UserId = d.Id,
                Name = d.Name,
                Username = d.Username,
                Gender = d.Gender,
                Age = d.Age,
                HeightCm = d.HeightCm,
                WeightKg = d.WeightKg,
                TodayTotalMl = summary?.TotalMl ?? 0,
                TodayUseCount = summary?.UseCount ?? 0,
                TargetMl = summary?.TargetMl ?? _advice.CalculateTargetMl(d.WeightKg),
                Advice = summary?.Advice
            });
        }

        ViewBag.RoleName = "管理员";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "管理员";
        return View(overviews);
    }

    /// <summary>用户管理页 - 查看所有用户</summary>
    public async Task<IActionResult> Users()
    {
        var users = await _db.Users.Where(u => u.Section == "elderly").OrderBy(u => u.Role).ThenBy(u => u.Username).ToListAsync();
        ViewBag.RoleName = "管理员";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "管理员";
        return View(users);
    }

    /// <summary>添加用户页面</summary>
    [HttpGet]
    public IActionResult AddUser()
    {
        ViewBag.RoleName = "管理员";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "管理员";
        return View(new User());
    }

    /// <summary>添加用户提交</summary>
    [HttpPost]
    public async Task<IActionResult> AddUser(User user)
    {
        if (string.IsNullOrWhiteSpace(user.Username))
        {
            ModelState.AddModelError("Username", "请输入登录编号");
            return View(user);
        }
        if (string.IsNullOrWhiteSpace(user.Password))
        {
            ModelState.AddModelError("Password", "请输入密码");
            return View(user);
        }
        if (string.IsNullOrWhiteSpace(user.Name))
        {
            ModelState.AddModelError("Name", "请输入姓名");
            return View(user);
        }

        // 检查编号是否重复
        if (await _db.Users.AnyAsync(u => u.Username == user.Username))
        {
            ModelState.AddModelError("Username", "该登录编号已被使用");
            return View(user);
        }

        user.Section = "elderly"; // 养老院板块添加的用户强制归入养老院
        user.CreatedAt = DateTime.Now;
        user.UpdatedAt = DateTime.Now;
        if (string.IsNullOrWhiteSpace(user.Phone)) user.Phone = null;
        _db.Users.Add(user);
        await _db.SaveChangesAsync();

        TempData["Success"] = $"用户 {user.Name} 添加成功！{(string.IsNullOrEmpty(user.Phone) ? "" : "已绑定微信推送")}";
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

    /// <summary>编辑用户提交</summary>
    [HttpPost]
    public async Task<IActionResult> EditUser(User user)
    {
        var dbUser = await _db.Users.FindAsync(user.Id);
        if (dbUser == null) return NotFound();
        if (dbUser.Section != "elderly") return NotFound(); // 只能编辑养老院板块用户

        if (string.IsNullOrWhiteSpace(user.Username))
        {
            ModelState.AddModelError("Username", "请输入登录编号");
            return View(user);
        }

        // 检查编号是否被其他用户占用
        if (await _db.Users.AnyAsync(u => u.Username == user.Username && u.Id != user.Id))
        {
            ModelState.AddModelError("Username", "该登录编号已被其他用户使用");
            return View(user);
        }

        dbUser.Username = user.Username;
        if (!string.IsNullOrWhiteSpace(user.Password))
            dbUser.Password = user.Password;
        dbUser.Name = user.Name;
        dbUser.Role = user.Role;
        dbUser.Gender = user.Gender;
        dbUser.Age = user.Age;
        dbUser.HeightCm = user.HeightCm;
        dbUser.WeightKg = user.WeightKg;
        dbUser.Phone = string.IsNullOrWhiteSpace(user.Phone) ? null : user.Phone;
        dbUser.UpdatedAt = DateTime.Now;

        await _db.SaveChangesAsync();
        TempData["Success"] = $"用户 {dbUser.Name} 信息已更新！";
        return RedirectToAction("Users");
    }

    [HttpPost]
    public async Task<IActionResult> TestPush(int id)
    {
        var user = await _db.Users.FindAsync(id);
        if (user == null || user.Section != "elderly") return NotFound();
        if (string.IsNullOrWhiteSpace(user.Phone))
        {
            TempData["Error"] = $"用户 {user.Name} 尚未绑定 PushPlus Token";
            return RedirectToAction("Users");
        }

        var (success, message) = await _pushPlus.SendAsync(
            user.Phone,
            "饮水机消息推送测试",
            $"{user.Name}，PushPlus 已成功连接到智能饮水机系统。测试时间：{DateTime.Now:yyyy-MM-dd HH:mm:ss}",
            HttpContext.RequestAborted);
        TempData[success ? "Success" : "Error"] = success
            ? $"测试消息已发送给 {user.Name}"
            : $"测试消息发送失败：{message}";
        return RedirectToAction("Users");
    }

    /// <summary>删除用户</summary>
    [HttpPost]
    public async Task<IActionResult> DeleteUser(int id)
    {
        var user = await _db.Users.FindAsync(id);
        if (user == null) return NotFound();
        if (user.Section != "elderly") return NotFound(); // 只能删除养老院板块用户

        if (user.Role == 0)
        {
            TempData["Error"] = "不能删除管理员账号";
            return RedirectToAction("Users");
        }

        _db.Users.Remove(user);
        await _db.SaveChangesAsync();
        TempData["Success"] = $"用户 {user.Name} 已删除！";
        return RedirectToAction("Users");
    }

    /// <summary>查看饮水者详情</summary>
    public async Task<IActionResult> UserDetail(int id)
    {
        var user = await _db.Users.FindAsync(id);
        if (user == null) return NotFound();
        if (user.Role != 2 || user.Section != "elderly")
        {
            TempData["Error"] = "只能查看养老院饮水者详情";
            return RedirectToAction("Index");
        }

        var today = DateTime.Today;
        var summary = await _db.DailySummaries.FirstOrDefaultAsync(d => d.UserId == id && d.Date == today);
        var recentRecords = await _db.Records
            .Where(r => r.UserId == id && r.RecordedAt >= today)
            .OrderByDescending(r => r.RecordedAt)
            .Take(20)
            .ToListAsync();

        // 近7天数据
        var weekData = new List<DailySummaryItem>();
        int weekTotalMl = 0, weekDays = 0;
        for (int i = 6; i >= 0; i--)
        {
            var date = today.AddDays(-i);
            var daySummary = await _db.DailySummaries.FirstOrDefaultAsync(d => d.UserId == id && d.Date == date);
            var item = new DailySummaryItem
            {
                DateStr = date.ToString("MM-dd"),
                TotalMl = daySummary?.TotalMl ?? 0,
                UseCount = daySummary?.UseCount ?? 0,
                TargetMl = daySummary?.TargetMl ?? _advice.CalculateTargetMl(user.WeightKg)
            };
            weekData.Add(item);
            if (item.TotalMl > 0) { weekTotalMl += item.TotalMl; weekDays++; }
        }

        // 当天无饮水记录时，用近7天情况生成建议
        var targetMl = _advice.CalculateTargetMl(user.WeightKg);
        bool isNewSummary = false;
        if (summary == null)
        {
            // 只在数据库确实无今日记录时才新建
            summary = new DailySummary
            {
                UserId = id,
                Date = today,
                TotalMl = 0,
                UseCount = 0,
                TargetMl = targetMl,
                Advice = _advice.GenerateWeeklyAdvice(user.Name, user.Age, user.Gender, targetMl, weekTotalMl, weekDays, 0)
            };
            _db.DailySummaries.Add(summary);
            isNewSummary = true;
        }
        else if (summary.TotalMl == 0 && string.IsNullOrEmpty(summary.Advice))
        {
            // 记录已存在但建议为空，直接更新（不重复插入）
            summary.TargetMl = targetMl;
            summary.Advice = _advice.GenerateWeeklyAdvice(user.Name, user.Age, user.Gender, targetMl, weekTotalMl, weekDays, 0);
        }

        // 当天无 AI 建议时，按需调用 DeepSeek 生成并缓存到数据库
        if (string.IsNullOrEmpty(summary.AiAdvice))
        {
            var aiAdvice = await _deepSeek.GenerateWeeklyAdviceAsync(
                user.Name, user.Age, user.Gender, user.HeightCm, user.WeightKg,
                targetMl, weekTotalMl, weekDays, summary.TotalMl);
            summary.AiAdvice = aiAdvice;
            summary.UpdatedAt = DateTime.Now;
            await _db.SaveChangesAsync();
        }

        ViewBag.Detail = user;
        ViewBag.Summary = summary;
        ViewBag.RecentRecords = recentRecords;
        ViewBag.WeekData = weekData;
        ViewBag.RoleName = "管理员";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "管理员";
        return View();
    }

    /// <summary>管理员对某饮水者的 AI 多轮问答</summary>
    [HttpPost]
    public async Task<IActionResult> AskAI(int userId, string messages)
    {
        var user = await _db.Users.FindAsync(userId);
        if (user == null) return Json(new { ok = false, answer = "未找到该用户" });
        if (user.Role != 2 || user.Section != "elderly")
            return Json(new { ok = false, answer = "只能查询养老院饮水者" });

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

/// <summary>
/// 管理员身份验证过滤器
/// </summary>
public class AdminAuthFilter : IActionFilter
{
    public void OnActionExecuting(ActionExecutingContext context)
    {
        var role = context.HttpContext.Session.GetInt32("UserRole");
        var section = context.HttpContext.Session.GetString("UserSection");
        if (role != 0 || section != "elderly")
        {
            context.Result = new RedirectToActionResult("Login", "Account", new { section = "elderly" });
        }
    }

    public void OnActionExecuted(ActionExecutedContext context) { }
}
