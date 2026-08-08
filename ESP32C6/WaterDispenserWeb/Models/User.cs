using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace WaterDispenserWeb.Models;

/// <summary>
/// 用户表
/// </summary>
[Table("users")]
public class User
{
    [Key]
    [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
    public int Id { get; set; }

    /// <summary>登录编号</summary>
    [Required, MaxLength(32)]
    public string Username { get; set; } = "";

    /// <summary>密码</summary>
    [Required, MaxLength(256)]
    public string Password { get; set; } = "";

    /// <summary>姓名</summary>
    [Required, MaxLength(32)]
    public string Name { get; set; } = "";

    /// <summary>角色: 0=管理员, 1=护工, 2=饮水者, 3=计费普通用户</summary>
    public int Role { get; set; } = 2;

    /// <summary>所属板块: elderly=养老院 billing=饮水计费 coming=敬请期待 *=所有板块(管理员)</summary>
    [MaxLength(32)]
    public string Section { get; set; } = "elderly";

    /// <summary>性别: 0=未知, 1=男, 2=女</summary>
    public int Gender { get; set; } = 0;

    /// <summary>年龄</summary>
    public int Age { get; set; } = 0;

    /// <summary>身高(cm)</summary>
    public float HeightCm { get; set; } = 0;

    /// <summary>体重(kg)</summary>
    public float WeightKg { get; set; } = 0;

    /// <summary>预留: 指纹ID</summary>
    public int? FpId { get; set; }

    /// <summary>PushPlus微信token</summary>
    [MaxLength(64)]
    public string? Phone { get; set; }

    public DateTime CreatedAt { get; set; } = DateTime.Now;

    public DateTime UpdatedAt { get; set; } = DateTime.Now;

    // 导航属性
    public ICollection<Record> Records { get; set; } = new List<Record>();
    public ICollection<DailySummary> DailySummaries { get; set; } = new List<DailySummary>();

    /// <summary>角色显示名称</summary>
    [NotMapped]
    public string RoleName => Role switch
    {
        0 => "管理员",
        1 => "护工",
        2 => "饮水者",
        3 => "普通用户",
        _ => "未知"
    };

    /// <summary>板块显示名称</summary>
    [NotMapped]
    public string SectionName => Section switch
    {
        "elderly" => "养老院板块",
        "billing" => "个人计费板块",
        "home" => "家庭板块",
        "coming" => "敬请期待板块",
        "*" => "所有板块",
        _ => Section
    };

    /// <summary>性别显示名称</summary>
    [NotMapped]
    public string GenderName => Gender switch
    {
        1 => "男",
        2 => "女",
        _ => "未知"
    };
}
