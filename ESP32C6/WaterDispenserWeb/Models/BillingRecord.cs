using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace WaterDispenserWeb.Models;

[Table("billing_records")]
public class BillingRecord
{
    [Key]
    [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
    public int Id { get; set; }

    public int UserId { get; set; }

    public int VolumeMl { get; set; }

    [Column(TypeName = "decimal(10,2)")]
    public decimal Cost { get; set; }

    [Column("balance_before", TypeName = "decimal(10,2)")]
    public decimal BalanceBefore { get; set; }

    [Column("balance_after", TypeName = "decimal(10,2)")]
    public decimal BalanceAfter { get; set; }

    public DateTime RecordedAt { get; set; } = DateTime.Now;

    [ForeignKey(nameof(UserId))]
    public User? User { get; set; }
}
