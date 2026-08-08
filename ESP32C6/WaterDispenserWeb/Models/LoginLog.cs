using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace WaterDispenserWeb.Models;

/// <summary>
/// 登录日志表
/// </summary>
[Table("login_logs")]
public class LoginLog
{
    [Key]
    [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
    public int Id { get; set; }

    public int UserId { get; set; }

    /// <summary>登录IP</summary>
    [MaxLength(64)]
    public string? LoginIp { get; set; }

    /// <summary>登录时间</summary>
    public DateTime LoginTime { get; set; } = DateTime.Now;

    // 导航属性
    [ForeignKey(nameof(UserId))]
    public User? User { get; set; }
}
