using System.ComponentModel.DataAnnotations;

namespace WaterDispenserWeb.ViewModels;

public class LoginViewModel
{
    /// <summary>登录编号</summary>
    [Required(ErrorMessage = "请输入登录编号")]
    [Display(Name = "登录编号")]
    public string Username { get; set; } = "";

    /// <summary>密码</summary>
    [Required(ErrorMessage = "请输入密码")]
    [DataType(DataType.Password)]
    [Display(Name = "密码")]
    public string Password { get; set; } = "";

    [Display(Name = "记住我")]
    public bool RememberMe { get; set; }
}
