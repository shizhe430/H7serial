using Microsoft.AspNetCore.Mvc;

namespace WaterDispenserWeb.Controllers;

public class HomeController : Controller
{
    /// <summary>首页 - 始终显示板块选择页</summary>
    public IActionResult Index()
    {
        // 清空当前板块session，强制重新选择
        HttpContext.Session.Remove("UserSection");
        return View("SectionSelect");
    }

    /// <summary>登录后跳转到对应板块首页</summary>
    public IActionResult Dashboard()
    {
        var userId = HttpContext.Session.GetInt32("UserId");
        var role = HttpContext.Session.GetInt32("UserRole");
        var section = HttpContext.Session.GetString("UserSection");

        if (userId == null || role == null || string.IsNullOrEmpty(section))
            return RedirectToAction("Index"); // 回板块选择

        if (section == "elderly")
        {
            return role switch
            {
                0 => RedirectToAction("Index", "Admin"),
                1 => RedirectToAction("Index", "Worker"),
                2 => RedirectToAction("Index", "Drinker"),
                _ => RedirectToAction("Index")
            };
        }
        else if (section == "billing")
        {
            return role switch
            {
                0 => RedirectToAction("Index", "BillingAdmin"),
                3 => RedirectToAction("Index", "BillingUser"),
                _ => RedirectToAction("Index")
            };
        }
        else if (section == "home")
        {
            // 家长(0)和孩子(2)都进家庭板块
            return RedirectToAction("Index", "Family");
        }

        return RedirectToAction("Index");
    }

    public IActionResult ComingSoon()
    {
        return View();
    }

    /// <summary>家庭板块 - 待完善</summary>
    public IActionResult HomeSection()
    {
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View();
    }
}
