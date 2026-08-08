using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Filters;
using Microsoft.EntityFrameworkCore;
using WaterDispenserWeb.Data;
using WaterDispenserWeb.Services;
using WaterDispenserWeb.ViewModels;

namespace WaterDispenserWeb.Controllers;

/// <summary>
/// 饮水者控制器 - 需要饮水者角色(role=2)
/// 饮水者只能查看自己的信息
/// </summary>
[ServiceFilter(typeof(DrinkerAuthFilter))]
public class DrinkerController : Controller
{
    private readonly AppDbContext _db;
    private readonly WaterAdviceService _advice;
    private readonly DeepSeekService _deepSeek;

    public DrinkerController(AppDbContext db, WaterAdviceService advice, DeepSeekService deepSeek)
    {
        _db = db;
        _advice = advice;
        _deepSeek = deepSeek;
    }

    /// <summary>饮水者首页 - 查看自己的饮水信息</summary>
    public async Task<IActionResult> Index()
    {
        var userId = HttpContext.Session.GetInt32("UserId") ?? 0;
        var user = await _db.Users.FindAsync(userId);
        if (user == null) return RedirectToAction("Login", "Account");

        var today = DateTime.Today;
        var summary = await _db.DailySummaries.FirstOrDefaultAsync(d => d.UserId == userId && d.Date == today);
        var recentRecords = await _db.Records
            .Where(r => r.UserId == userId && r.RecordedAt >= today)
            .OrderByDescending(r => r.RecordedAt)
            .Take(20)
            .ToListAsync();

        var targetMl = summary?.TargetMl ?? _advice.CalculateTargetMl(user.WeightKg);

        // 近7天数据
        var recentWeek = new List<DailySummaryItem>();
        int weekTotalMl = 0, weekDays = 0;
        for (int i = 6; i >= 0; i--)
        {
            var date = today.AddDays(-i);
            var daySummary = await _db.DailySummaries.FirstOrDefaultAsync(d => d.UserId == userId && d.Date == date);
            var item = new DailySummaryItem
            {
                DateStr = date.ToString("MM-dd"),
                TotalMl = daySummary?.TotalMl ?? 0,
                UseCount = daySummary?.UseCount ?? 0,
                TargetMl = daySummary?.TargetMl ?? targetMl
            };
            recentWeek.Add(item);
            if (item.TotalMl > 0) { weekTotalMl += item.TotalMl; weekDays++; }
        }

        // 当天无饮水记录时，用近7天情况生成建议（格式与有记录时一致）
        string advice = summary?.Advice;
        string aiAdvice = summary?.AiAdvice;
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

            // 保存到数据库（若已有summary则更新，否则创建）
            if (summary == null)
            {
                summary = new Models.DailySummary
                {
                    UserId = userId,
                    Date = today,
                    TotalMl = 0,
                    UseCount = 0,
                    TargetMl = targetMl,
                    Advice = advice,
                    AiAdvice = aiAdvice
                };
                _db.DailySummaries.Add(summary);
            }
            else
            {
                summary.AiAdvice = aiAdvice;
                summary.Advice = advice;
                summary.UpdatedAt = DateTime.Now;
            }
            await _db.SaveChangesAsync();
        }

        var model = new DashboardViewModel
        {
            UserId = user.Id,
            Name = user.Name,
            Username = user.Username,
            Role = user.Role,
            RoleName = "饮水者",
            Gender = user.Gender,
            Age = user.Age,
            HeightCm = user.HeightCm,
            WeightKg = user.WeightKg,
            TodayTotalMl = summary?.TotalMl ?? 0,
            TodayUseCount = summary?.UseCount ?? 0,
            TargetMl = targetMl,
            Advice = advice,
            AiAdvice = aiAdvice,
            RecentRecords = recentRecords.Select(r => new DailySummaryItem
            {
                DateStr = r.RecordedAt.ToString("HH:mm"),
                TotalMl = r.VolumeMl,
                UseCount = 1,
                TargetMl = targetMl
            }).ToList(),
            RecentWeek = recentWeek
        };

        ViewBag.UserName = user.Name;
        return View(model);
    }

    /// <summary>饮水者与 AI 多轮问答 - 只能问自己</summary>
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

public class DrinkerAuthFilter : IActionFilter
{
    public void OnActionExecuting(ActionExecutingContext context)
    {
        var role = context.HttpContext.Session.GetInt32("UserRole");
        var section = context.HttpContext.Session.GetString("UserSection");
        if (role != 2 || section != "elderly")
        {
            context.Result = new RedirectToActionResult("Login", "Account", new { section = "elderly" });
        }
    }

    public void OnActionExecuted(ActionExecutedContext context) { }
}
