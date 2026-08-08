using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Filters;
using Microsoft.EntityFrameworkCore;
using WaterDispenserWeb.Data;
using WaterDispenserWeb.Services;
using WaterDispenserWeb.ViewModels;

namespace WaterDispenserWeb.Controllers;

/// <summary>
/// 护工控制器 - 需要护工角色(role=1)
/// 护工可以查看所有饮水者的信息，但不能增删改用户
/// </summary>
[ServiceFilter(typeof(WorkerAuthFilter))]
public class WorkerController : Controller
{
    private readonly AppDbContext _db;
    private readonly WaterAdviceService _advice;
    private readonly DeepSeekService _deepSeek;

    public WorkerController(AppDbContext db, WaterAdviceService advice, DeepSeekService deepSeek)
    {
        _db = db;
        _advice = advice;
        _deepSeek = deepSeek;
    }

    /// <summary>护工首页 - 查看所有饮水者今日概况</summary>
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

        ViewBag.RoleName = "护工";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "护工";
        return View(overviews);
    }

    /// <summary>查看饮水者详情</summary>
    public async Task<IActionResult> DrinkerDetail(int id)
    {
        var user = await _db.Users.FindAsync(id);
        if (user == null || user.Role != 2 || user.Section != "elderly") return NotFound();

        var today = DateTime.Today;
        var summary = await _db.DailySummaries.FirstOrDefaultAsync(d => d.UserId == id && d.Date == today);
        var recentRecords = await _db.Records
            .Where(r => r.UserId == id && r.RecordedAt >= today)
            .OrderByDescending(r => r.RecordedAt)
            .Take(20)
            .ToListAsync();

        // 近7天统计（用于当天无记录时生成建议）
        int weekTotalMl = 0, weekDays = 0;
        for (int i = 6; i >= 0; i--)
        {
            var date = today.AddDays(-i);
            var daySummary = await _db.DailySummaries.FirstOrDefaultAsync(d => d.UserId == id && d.Date == date);
            if ((daySummary?.TotalMl ?? 0) > 0) { weekTotalMl += daySummary.TotalMl; weekDays++; }
        }

        string advice = summary?.Advice;
        string aiAdvice = summary?.AiAdvice;
        var targetMl = summary?.TargetMl ?? _advice.CalculateTargetMl(user.WeightKg);
        if (summary == null || (summary.TotalMl == 0 && string.IsNullOrEmpty(summary.Advice)))
        {
            advice = _advice.GenerateWeeklyAdvice(user.Name, user.Age, user.Gender, targetMl, weekTotalMl, weekDays, 0);
        }

        // 当天无 AI 建议时，按需调用 DeepSeek 生成并缓存到数据库
        if (string.IsNullOrEmpty(aiAdvice))
        {
            aiAdvice = await _deepSeek.GenerateWeeklyAdviceAsync(
                user.Name, user.Age, user.Gender, user.HeightCm, user.WeightKg,
                targetMl, weekTotalMl, weekDays, summary?.TotalMl ?? 0);
            if (summary != null)
            {
                summary.AiAdvice = aiAdvice;
                summary.UpdatedAt = DateTime.Now;
                await _db.SaveChangesAsync();
            }
        }

        ViewBag.Detail = user;
        ViewBag.Summary = summary != null ? new DailySummaryItem
        {
            TotalMl = summary.TotalMl,
            UseCount = summary.UseCount,
            TargetMl = summary.TargetMl
        } : null;
        ViewBag.Advice = advice;
        ViewBag.AiAdvice = aiAdvice;
        ViewBag.RecentRecords = recentRecords;
        ViewBag.RoleName = "护工";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "护工";
        return View();
    }

    /// <summary>护工对某饮水者的 AI 多轮问答</summary>
    [HttpPost]
    public async Task<IActionResult> AskAI(int userId, string messages)
    {
        var user = await _db.Users.FindAsync(userId);
        if (user == null || user.Role != 2 || user.Section != "elderly")
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

public class WorkerAuthFilter : IActionFilter
{
    public void OnActionExecuting(ActionExecutingContext context)
    {
        var role = context.HttpContext.Session.GetInt32("UserRole");
        var section = context.HttpContext.Session.GetString("UserSection");
        if (role != 1 || section != "elderly")
        {
            context.Result = new RedirectToActionResult("Login", "Account", new { section = "elderly" });
        }
    }

    public void OnActionExecuted(ActionExecutedContext context) { }
}
