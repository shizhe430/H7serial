using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace WaterDispenserWeb.Models;

[Table("billing_accounts")]
public class BillingAccount
{
    [Key]
    [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
    public int Id { get; set; }

    public int UserId { get; set; }

    [Column(TypeName = "decimal(10,2)")]
    public decimal Balance { get; set; } = 0;

    [Column("total_consumed", TypeName = "decimal(10,2)")]
    public decimal TotalConsumed { get; set; } = 0;

    [Column("rate_per_ml", TypeName = "decimal(10,4)")]
    public decimal RatePerMl { get; set; } = 0.0050m;

    public DateTime CreatedAt { get; set; } = DateTime.Now;
    public DateTime UpdatedAt { get; set; } = DateTime.Now;

    [ForeignKey(nameof(UserId))]
    public User? User { get; set; }
}
