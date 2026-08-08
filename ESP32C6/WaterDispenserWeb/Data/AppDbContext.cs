using Microsoft.EntityFrameworkCore;
using WaterDispenserWeb.Models;

namespace WaterDispenserWeb.Data;

public class AppDbContext : DbContext
{
    public AppDbContext(DbContextOptions<AppDbContext> options) : base(options) { }

    public DbSet<User> Users => Set<User>();
    public DbSet<Record> Records => Set<Record>();
    public DbSet<DailySummary> DailySummaries => Set<DailySummary>();
    public DbSet<LoginLog> LoginLogs => Set<LoginLog>();
    public DbSet<Section> Sections => Set<Section>();
    public DbSet<BillingAccount> BillingAccounts => Set<BillingAccount>();
    public DbSet<BillingRecord> BillingRecords => Set<BillingRecord>();

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);

        // ===== User 表映射 =====
        modelBuilder.Entity<User>(entity =>
        {
            entity.ToTable("users");
            entity.HasKey(u => u.Id);
            entity.HasIndex(u => u.Username).IsUnique();

            entity.Property(u => u.Id).HasColumnName("id");
            entity.Property(u => u.Username).HasColumnName("username").HasMaxLength(32);
            entity.Property(u => u.Password).HasColumnName("password").HasMaxLength(256);
            entity.Property(u => u.Name).HasColumnName("name").HasMaxLength(32);
            entity.Property(u => u.Role).HasColumnName("role");
            entity.Property(u => u.Section).HasColumnName("section").HasMaxLength(32);
            entity.Property(u => u.Gender).HasColumnName("gender");
            entity.Property(u => u.Age).HasColumnName("age");
            entity.Property(u => u.HeightCm).HasColumnName("height_cm");
            entity.Property(u => u.WeightKg).HasColumnName("weight_kg");
            entity.Property(u => u.FpId).HasColumnName("fp_id");
            entity.Property(u => u.Phone).HasColumnName("phone").HasMaxLength(64);
            entity.Property(u => u.CreatedAt).HasColumnName("created_at");
            entity.Property(u => u.UpdatedAt).HasColumnName("updated_at");
        });

        // ===== Record 表映射 =====
        modelBuilder.Entity<Record>(entity =>
        {
            entity.ToTable("records");
            entity.HasKey(r => r.Id);
            entity.Property(r => r.Id).HasColumnName("id");
            entity.Property(r => r.UserId).HasColumnName("user_id");
            entity.Property(r => r.VolumeMl).HasColumnName("volume_ml");
            entity.Property(r => r.RecordedAt).HasColumnName("recorded_at");

            entity.HasIndex(r => r.RecordedAt);
            entity.HasIndex(r => new { r.UserId, r.RecordedAt });
            entity.HasOne(r => r.User)
                  .WithMany(u => u.Records)
                  .HasForeignKey(r => r.UserId)
                  .OnDelete(DeleteBehavior.Cascade);
        });

        // ===== DailySummary 表映射 =====
        modelBuilder.Entity<DailySummary>(entity =>
        {
            entity.ToTable("daily_summary");
            entity.HasKey(d => d.Id);
            entity.Property(d => d.Id).HasColumnName("id");
            entity.Property(d => d.UserId).HasColumnName("user_id");
            entity.Property(d => d.Date).HasColumnName("date").HasColumnType("date");
            entity.Property(d => d.TotalMl).HasColumnName("total_ml");
            entity.Property(d => d.UseCount).HasColumnName("use_count");
            entity.Property(d => d.TargetMl).HasColumnName("target_ml");
            entity.Property(d => d.Advice).HasColumnName("advice").HasMaxLength(512);
            entity.Property(d => d.AiAdvice).HasColumnName("ai_advice").HasMaxLength(1024);
            entity.Property(d => d.UpdatedAt).HasColumnName("updated_at");

            entity.HasIndex(d => new { d.UserId, d.Date }).IsUnique();
            entity.HasOne(d => d.User)
                  .WithMany(u => u.DailySummaries)
                  .HasForeignKey(d => d.UserId)
                  .OnDelete(DeleteBehavior.Cascade);
        });

        // ===== LoginLog 表映射 =====
        modelBuilder.Entity<LoginLog>(entity =>
        {
            entity.ToTable("login_logs");
            entity.HasKey(l => l.Id);
            entity.Property(l => l.Id).HasColumnName("id");
            entity.Property(l => l.UserId).HasColumnName("user_id");
            entity.Property(l => l.LoginIp).HasColumnName("login_ip").HasMaxLength(64);
            entity.Property(l => l.LoginTime).HasColumnName("login_time");

            entity.HasOne(l => l.User)
                  .WithMany()
                  .HasForeignKey(l => l.UserId)
                  .OnDelete(DeleteBehavior.Cascade);
        });

        // ===== Section 表映射 =====
        modelBuilder.Entity<Section>(entity =>
        {
            entity.ToTable("sections");
            entity.HasKey(s => s.Id);
            entity.Property(s => s.Id).HasColumnName("id");
            entity.Property(s => s.Name).HasColumnName("name").HasMaxLength(32);
            entity.Property(s => s.Code).HasColumnName("code").HasMaxLength(32);
            entity.HasIndex(s => s.Code).IsUnique();
        });

        // ===== BillingAccount 表映射 =====
        modelBuilder.Entity<BillingAccount>(entity =>
        {
            entity.ToTable("billing_accounts");
            entity.HasKey(b => b.Id);
            entity.Property(b => b.Id).HasColumnName("id");
            entity.Property(b => b.UserId).HasColumnName("user_id");
            entity.Property(b => b.Balance).HasColumnName("balance").HasColumnType("decimal(10,2)");
            entity.Property(b => b.TotalConsumed).HasColumnName("total_consumed").HasColumnType("decimal(10,2)");
            entity.Property(b => b.RatePerMl).HasColumnName("rate_per_ml").HasColumnType("decimal(10,4)");
            entity.Property(b => b.CreatedAt).HasColumnName("created_at");
            entity.Property(b => b.UpdatedAt).HasColumnName("updated_at");

            entity.HasIndex(b => b.UserId).IsUnique();
            entity.HasOne(b => b.User)
                  .WithMany()
                  .HasForeignKey(b => b.UserId)
                  .OnDelete(DeleteBehavior.Cascade);
        });

        // ===== BillingRecord 表映射 =====
        modelBuilder.Entity<BillingRecord>(entity =>
        {
            entity.ToTable("billing_records");
            entity.HasKey(b => b.Id);
            entity.Property(b => b.Id).HasColumnName("id");
            entity.Property(b => b.UserId).HasColumnName("user_id");
            entity.Property(b => b.VolumeMl).HasColumnName("volume_ml");
            entity.Property(b => b.Cost).HasColumnName("cost").HasColumnType("decimal(10,2)");
            entity.Property(b => b.BalanceBefore).HasColumnName("balance_before").HasColumnType("decimal(10,2)");
            entity.Property(b => b.BalanceAfter).HasColumnName("balance_after").HasColumnType("decimal(10,2)");
            entity.Property(b => b.RecordedAt).HasColumnName("recorded_at");

            entity.HasOne(b => b.User)
                  .WithMany()
                  .HasForeignKey(b => b.UserId)
                  .OnDelete(DeleteBehavior.Cascade);
        });
    }
}
