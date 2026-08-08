using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace WaterDispenserWeb.Models;

[Table("sections")]
public class Section
{
    [Key]
    [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
    public int Id { get; set; }

    [Required, MaxLength(32)]
    public string Name { get; set; } = "";

    [Required, MaxLength(32)]
    public string Code { get; set; } = "";
}
