using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace WaterDispenserWeb.Models;

/// <summary>
/// 每日汇总表
/// </summary>
[Table("daily_summary")]
public class DailySummary
{
    [Key]
    [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
    public int Id { get; set; }

    public int UserId { get; set; }

    /// <summary>日期</summary>
    public DateTime Date { get; set; }

    /// <summary>今日饮水总量(ml)</summary>
    public int TotalMl { get; set; } = 0;

    /// <summary>今日饮水次数</summary>
    public int UseCount { get; set; } = 0;

    /// <summary>建议饮水量(根据体重计算)</summary>
    public int TargetMl { get; set; } = 0;

    /// <summary>基础建议</summary>
    [MaxLength(512)]
    public string? Advice { get; set; }

    /// <summary>AI智能建议(DeepSeek生成)</summary>
    [Column("ai_advice")]
    [MaxLength(1024)]
    public string? AiAdvice { get; set; }

    public DateTime UpdatedAt { get; set; } = DateTime.Now;

    // 导航属性
    [ForeignKey(nameof(UserId))]
    public User? User { get; set; }

    /// <summary>还需饮水(ml)</summary>
    [NotMapped]
    public int NeededMl => Math.Max(0, TargetMl - TotalMl);
}
