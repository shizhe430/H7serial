using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Filters;
using Microsoft.EntityFrameworkCore;
using WaterDispenserWeb.Data;
using WaterDispenserWeb.Models;
using WaterDispenserWeb.Services;
using WaterDispenserWeb.ViewModels;

namespace WaterDispenserWeb.Controllers;

/// <summary>
/// 家庭板块控制器 - 所有人互相查看饮水信息
/// 权限层级：管理员(全局 section=*) > 家长(role=0 section=home) > 孩子(role=2 section=home)
/// 家长和管理员可管理成员，孩子只能查看
/// </summary>
[ServiceFilter(typeof(FamilyAuthFilter))]
public class FamilyController : Controller
{
    private readonly AppDbContext _db;
    private readonly WaterAdviceService _advice;
    private readonly DeepSeekService _deepSeek;

    public FamilyController(AppDbContext db, WaterAdviceService advice, DeepSeekService deepSeek)
    {
        _db = db;
        _advice = advice;
        _deepSeek = deepSeek;
    }

    /// <summary>是否全局管理员（登录时 user.Section 存入 Session["UserSectionRaw"]）</summary>
    private bool IsGlobalAdmin()
        => HttpContext.Session.GetString("UserSectionRaw") == "*";

    /// <summary>是否家长（role=0 且 home 板块），含全局管理员</summary>
    private bool IsParent()
    {
        var role = HttpContext.Session.GetInt32("UserRole") ?? -1;
        var section = HttpContext.Session.GetString("UserSection");
        return role == 0 && section == "home";
    }

    /// <summary>家庭成员首页 - 所有成员今日饮水概况（全员互相查看）</summary>
    public async Task<IActionResult> Index()
    {
        var today = DateTime.Today;
        var members = await _db.Users
            .Where(u => u.Section == "home" && (u.Role == 0 || u.Role == 2))
            .ToListAsync();
        var todaySummaries = await _db.DailySummaries
            .Where(d => d.Date == today)
            .ToListAsync();

        var overviews = new List<DrinkerOverview>();
        foreach (var m in members)
        {
            var summary = todaySummaries.FirstOrDefault(s => s.UserId == m.Id);
            overviews.Add(new DrinkerOverview
            {
                UserId = m.Id,
                Name = m.Name,
                Username = m.Username,
                Gender = m.Gender,
                Age = m.Age,
                HeightCm = m.HeightCm,
                WeightKg = m.WeightKg,
                TodayTotalMl = summary?.TotalMl ?? 0,
                TodayUseCount = summary?.UseCount ?? 0,
                TargetMl = summary?.TargetMl ?? _advice.CalculateTargetMl(m.WeightKg),
                Advice = summary?.Advice
            });
        }

        ViewBag.RoleName = HttpContext.Session.GetInt32("UserRole") == 0 ? (IsGlobalAdmin() ? "管理员" : "家长") : "孩子";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "家庭成员";
        return View(overviews);
    }

    /// <summary>家庭成员详情 - 所有成员都可查看任何成员</summary>
    public async Task<IActionResult> Detail(int id)
    {
        var user = await _db.Users.FindAsync(id);
        if (user == null) return NotFound();
        if (user.Section != "home" || (user.Role != 0 && user.Role != 2))
        {
            TempData["Error"] = "只能查看家庭成员详情";
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
        ViewBag.RoleName = HttpContext.Session.GetInt32("UserRole") == 0 ? (IsGlobalAdmin() ? "管理员" : "家长") : "孩子";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "家庭成员";
        return View();
    }

    /// <summary>家庭成员管理页 - 仅家长/管理员可访问</summary>
    public async Task<IActionResult> Users()
    {
        if (!IsParent())
        {
            TempData["Error"] = "无权访问用户管理";
            return RedirectToAction("Index");
        }

        var users = await _db.Users
            .Where(u => u.Section == "home" && (u.Role == 0 || u.Role == 2))
            .OrderBy(u => u.Role)
            .ThenBy(u => u.Username)
            .ToListAsync();
        ViewBag.IsGlobalAdmin = IsGlobalAdmin();
        ViewBag.CurrentUserId = HttpContext.Session.GetInt32("UserId");
        ViewBag.RoleName = IsGlobalAdmin() ? "管理员" : "家长";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "家长";
        return View(users);
    }

    /// <summary>添加家庭成员页面 - 仅家长/管理员</summary>
    [HttpGet]
    public IActionResult AddUser()
    {
        if (!IsParent())
        {
            TempData["Error"] = "无权访问用户管理";
            return RedirectToAction("Index");
        }
        ViewBag.CanAddParent = IsGlobalAdmin(); // 只有全局管理员可添加家长
        ViewBag.RoleName = IsGlobalAdmin() ? "管理员" : "家长";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "家长";
        return View(new User { Section = "home", Role = 2 });
    }

    /// <summary>添加家庭成员提交</summary>
    [HttpPost]
    public async Task<IActionResult> AddUser(User user)
    {
        if (!IsParent())
        {
            TempData["Error"] = "无权访问用户管理";
            return RedirectToAction("Index");
        }

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
        // 添加家长仅限全局管理员
        if (user.Role == 0 && !IsGlobalAdmin())
        {
            ModelState.AddModelError("Role", "只有管理员能添加家长身份");
            ViewBag.CanAddParent = IsGlobalAdmin();
            ViewBag.RoleName = "家长";
            ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "家长";
            return View(user);
        }
        // 孩子角色固定为2，家长角色固定为0；其他角色一律不允许
        if (user.Role != 0 && user.Role != 2)
        {
            ModelState.AddModelError("Role", "家庭板块身份只能是家长或孩子");
            ViewBag.CanAddParent = IsGlobalAdmin();
            ViewBag.RoleName = IsGlobalAdmin() ? "管理员" : "家长";
            ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "家长";
            return View(user);
        }

        // 检查编号是否重复
        if (await _db.Users.AnyAsync(u => u.Username == user.Username))
        {
            ModelState.AddModelError("Username", "该登录编号已被使用");
            ViewBag.CanAddParent = IsGlobalAdmin();
            ViewBag.RoleName = IsGlobalAdmin() ? "管理员" : "家长";
            ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "家长";
            return View(user);
        }

        user.Section = "home"; // 强制归入家庭板块
        user.CreatedAt = DateTime.Now;
        user.UpdatedAt = DateTime.Now;
        if (string.IsNullOrWhiteSpace(user.Phone)) user.Phone = null;
        _db.Users.Add(user);
        await _db.SaveChangesAsync();

        TempData["Success"] = $"家庭成员 {user.Name} 添加成功！{(string.IsNullOrEmpty(user.Phone) ? "" : "已绑定微信推送")}";
        return RedirectToAction("Users");
    }

    /// <summary>编辑家庭成员 - 仅家长/管理员；编辑家长仅限全局管理员</summary>
    [HttpGet]
    public async Task<IActionResult> EditUser(int id)
    {
        if (!IsParent())
        {
            TempData["Error"] = "无权访问用户管理";
            return RedirectToAction("Index");
        }
        var user = await _db.Users.FindAsync(id);
        if (user == null) return NotFound();
        if (user.Section != "home") return NotFound();
        // 编辑家长仅限全局管理员
        if (user.Role == 0 && !IsGlobalAdmin())
        {
            TempData["Error"] = "无权编辑家长身份";
            return RedirectToAction("Users");
        }
        ViewBag.IsGlobalAdmin = IsGlobalAdmin();
        ViewBag.RoleName = IsGlobalAdmin() ? "管理员" : "家长";
        ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "家长";
        return View(user);
    }

    /// <summary>编辑家庭成员提交</summary>
    [HttpPost]
    public async Task<IActionResult> EditUser(User user)
    {
        if (!IsParent())
        {
            TempData["Error"] = "无权访问用户管理";
            return RedirectToAction("Index");
        }
        var dbUser = await _db.Users.FindAsync(user.Id);
        if (dbUser == null) return NotFound();
        if (dbUser.Section != "home") return NotFound();
        // 编辑家长仅限全局管理员
        if (dbUser.Role == 0 && !IsGlobalAdmin())
        {
            TempData["Error"] = "无权编辑家长身份";
            return RedirectToAction("Users");
        }

        if (string.IsNullOrWhiteSpace(user.Username))
        {
            ModelState.AddModelError("Username", "请输入登录编号");
            ViewBag.IsGlobalAdmin = IsGlobalAdmin();
            ViewBag.RoleName = IsGlobalAdmin() ? "管理员" : "家长";
            ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "家长";
            return View(user);
        }

        // 检查编号是否被其他用户占用
        if (await _db.Users.AnyAsync(u => u.Username == user.Username && u.Id != user.Id))
        {
            ModelState.AddModelError("Username", "该登录编号已被其他用户使用");
            ViewBag.IsGlobalAdmin = IsGlobalAdmin();
            ViewBag.RoleName = IsGlobalAdmin() ? "管理员" : "家长";
            ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "家长";
            return View(user);
        }
        // 家长不能把成员改成家长；管理员可以把孩子改成家长（但不能把家长改成孩子以外的其他）
        if (user.Role != 0 && user.Role != 2)
        {
            ModelState.AddModelError("Role", "家庭板块身份只能是家长或孩子");
            ViewBag.IsGlobalAdmin = IsGlobalAdmin();
            ViewBag.RoleName = IsGlobalAdmin() ? "管理员" : "家长";
            ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "家长";
            return View(user);
        }
        if (user.Role == 0 && !IsGlobalAdmin())
        {
            ModelState.AddModelError("Role", "只有管理员能设置家长身份");
            ViewBag.IsGlobalAdmin = IsGlobalAdmin();
            ViewBag.RoleName = IsGlobalAdmin() ? "管理员" : "家长";
            ViewBag.UserName = HttpContext.Session.GetString("UserName") ?? "家长";
            return View(user);
        }

        dbUser.Username = user.Username;
        if (!string.IsNullOrWhiteSpace(user.Password))
            dbUser.Password = user.Password;
        dbUser.Name = user.Name;
        dbUser.Role = user.Role;
        dbUser.Section = "home";
        dbUser.Gender = user.Gender;
        dbUser.Age = user.Age;
        dbUser.HeightCm = user.HeightCm;
        dbUser.WeightKg = user.WeightKg;
        dbUser.Phone = string.IsNullOrWhiteSpace(user.Phone) ? null : user.Phone;
        dbUser.UpdatedAt = DateTime.Now;

        await _db.SaveChangesAsync();
        TempData["Success"] = $"家庭成员 {dbUser.Name} 信息已更新！";
        return RedirectToAction("Users");
    }

    /// <summary>删除家庭成员 - 仅家长/管理员；删家长仅限全局管理员且不能删自己</summary>
    [HttpPost]
    public async Task<IActionResult> DeleteUser(int id)
    {
        if (!IsParent())
        {
            TempData["Error"] = "无权访问用户管理";
            return RedirectToAction("Index");
        }
        var user = await _db.Users.FindAsync(id);
        if (user == null) return NotFound();
        if (user.Section != "home") return NotFound();

        // 删自己：任何情况都不允许
        if (user.Id == HttpContext.Session.GetInt32("UserId"))
        {
            TempData["Error"] = "不能删除当前登录的自己";
            return RedirectToAction("Users");
        }
        // 删家长：仅全局管理员
        if (user.Role == 0 && !IsGlobalAdmin())
        {
            TempData["Error"] = "无权删除家长身份";
            return RedirectToAction("Users");
        }

        _db.Users.Remove(user);
        await _db.SaveChangesAsync();
        TempData["Success"] = $"家庭成员 {user.Name} 已删除！";
        return RedirectToAction("Users");
    }

    /// <summary>家庭成员 AI 多轮问答 - 任何成员可问任何成员（互相查看）</summary>
    [HttpPost]
    public async Task<IActionResult> AskAI(int userId, string messages)
    {
        var user = await _db.Users.FindAsync(userId);
        if (user == null) return Json(new { ok = false, answer = "未找到该用户" });
        if (user.Section != "home" || (user.Role != 0 && user.Role != 2))
            return Json(new { ok = false, answer = "只能查询家庭成员" });

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
/// 家庭板块身份验证过滤器 - role 0/2 且 section=home
/// </summary>
public class FamilyAuthFilter : IActionFilter
{
    public void OnActionExecuting(ActionExecutingContext context)
    {
        var role = context.HttpContext.Session.GetInt32("UserRole");
        var section = context.HttpContext.Session.GetString("UserSection");
        if ((role != 0 && role != 2) || section != "home")
        {
            context.Result = new RedirectToActionResult("Login", "Account", new { section = "home" });
        }
    }

    public void OnActionExecuted(ActionExecutedContext context) { }
}
