using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using WaterDispenserWeb.Data;
using WaterDispenserWeb.Models;
using WaterDispenserWeb.ViewModels;

namespace WaterDispenserWeb.Controllers;

public class AccountController : Controller
{
    private readonly AppDbContext _db;
    private readonly IHttpContextAccessor _httpContextAccessor;

    public AccountController(AppDbContext db, IHttpContextAccessor httpContextAccessor)
    {
        _db = db;
        _httpContextAccessor = httpContextAccessor;
    }

    /// <summary>登录页（支持指定板块）</summary>
    [HttpGet]
    public IActionResult Login(string section = "elderly")
    {
        // 如果已经登录但想切换板块，直接更新板块并跳转
        if (HttpContext.Session.GetInt32("UserId") > 0)
        {
            var currentSection = HttpContext.Session.GetString("UserSection");
            if (currentSection != section)
            {
                HttpContext.Session.SetString("UserSection", section);
                return RedirectToAction("Dashboard", "Home");
            }
            // 同板块，直接进
            return RedirectToAction("Dashboard", "Home");
        }

        ViewBag.Section = section;
        ViewBag.SectionName = section switch
        {
            "elderly" => "养老院板块",
            "billing" => "个人计费板块",
            "home" => "家庭板块",
            _ => "未知板块"
        };
        return View();
    }

    /// <summary>登录提交</summary>
    [HttpPost]
    public async Task<IActionResult> Login(LoginViewModel model, string section = "elderly")
    {
        ViewBag.Section = section;
        ViewBag.SectionName = section switch
        {
            "elderly" => "养老院板块",
            "billing" => "个人计费板块",
            "home" => "家庭板块",
            _ => "未知板块"
        };

        if (!ModelState.IsValid)
            return View(model);

        var user = await _db.Users.FirstOrDefaultAsync(u => u.Username == model.Username);
        if (user == null || user.Password != model.Password)
        {
            ModelState.AddModelError("", "编号或密码错误");
            return View(model);
        }

        // 验证板块访问权限
        bool canAccess = user.Section == "*" || user.Section == section;
        if (!canAccess)
        {
            ModelState.AddModelError("", "该用户无权访问此板块");
            return View(model);
        }

        // 验证板块角色匹配
        if (section == "billing" && user.Role != 0 && user.Role != 3)
        {
            ModelState.AddModelError("", "该用户不属于个人计费板块");
            return View(model);
        }
        if (section == "elderly" && user.Role != 0 && user.Role != 1 && user.Role != 2)
        {
            ModelState.AddModelError("", "该用户不属于养老院板块");
            return View(model);
        }
        if (section == "home" && user.Role != 0 && user.Role != 2)
        {
            ModelState.AddModelError("", "该用户不属于家庭板块");
            return View(model);
        }

        // 保存登录状态
        HttpContext.Session.SetInt32("UserId", user.Id);
        HttpContext.Session.SetString("UserName", user.Name);
        HttpContext.Session.SetInt32("UserRole", user.Role);
        HttpContext.Session.SetString("UserSection", section);
        // 保存用户原始板块（区分全局管理员 * 与家长 home）
        HttpContext.Session.SetString("UserSectionRaw", user.Section);

        // 记录登录日志
        var ip = _httpContextAccessor.HttpContext?.Connection.RemoteIpAddress?.ToString() ?? "未知";
        _db.LoginLogs.Add(new LoginLog
        {
            UserId = user.Id,
            LoginIp = ip
        });
        await _db.SaveChangesAsync();

        return RedirectToAction("Dashboard", "Home");
    }

    /// <summary>退出登录</summary>
    [HttpGet]
    public IActionResult Logout()
    {
        HttpContext.Session.Clear();
        return RedirectToAction("Index", "Home"); // 回板块选择页
    }

    /// <summary>获取当前登录用户信息</summary>
    public IActionResult GetCurrentUser()
    {
        var userId = HttpContext.Session.GetInt32("UserId");
        if (userId == null)
            return Json(new { ok = false });

        return Json(new
        {
            ok = true,
            userId,
            userName = HttpContext.Session.GetString("UserName"),
            userRole = HttpContext.Session.GetInt32("UserRole"),
            userSection = HttpContext.Session.GetString("UserSection")
        });
    }
}
