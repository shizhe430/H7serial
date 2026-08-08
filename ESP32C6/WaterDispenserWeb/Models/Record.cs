using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace WaterDispenserWeb.Models;

/// <summary>
/// 饮水记录表
/// </summary>
[Table("records")]
public class Record
{
    [Key]
    [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
    public int Id { get; set; }

    public int UserId { get; set; }

    /// <summary>饮水量(ml)</summary>
    public int VolumeMl { get; set; }

    /// <summary>记录时间</summary>
    public DateTime RecordedAt { get; set; } = DateTime.Now;

    // 导航属性
    [ForeignKey(nameof(UserId))]
    public User? User { get; set; }
}
