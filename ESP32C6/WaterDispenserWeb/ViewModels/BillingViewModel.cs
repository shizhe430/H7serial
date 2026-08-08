namespace WaterDispenserWeb.ViewModels;

public class BillingDashboardViewModel
{
    public int UserId { get; set; }
    public string Name { get; set; } = "";
    public string Username { get; set; } = "";
    public decimal Balance { get; set; }
    public decimal TotalConsumed { get; set; }
    public decimal RatePerMl { get; set; }
    public int RemainingMl { get; set; }
    public int TodayMl { get; set; }
    public int TodayCount { get; set; }
    public int TargetMl { get; set; }
    public int NeededMl => Math.Max(0, TargetMl - TodayMl);
    public double ProgressPercent => TargetMl > 0 ? Math.Min(100.0, (double)TodayMl / TargetMl * 100) : 0;
    public bool CanDispense { get; set; }
    public string DispenseMessage { get; set; } = "";
    public string BalanceAdvice { get; set; } = "";
    public string? Advice { get; set; }
    public string? AiAdvice { get; set; }
    public List<DailySummaryItem> TodayRecords { get; set; } = new();
    public List<DailySummaryItem> RecentWeek { get; set; } = new();
}

public class BillingUserOverview
{
    public int UserId { get; set; }
    public string Name { get; set; } = "";
    public string Username { get; set; } = "";
    public decimal Balance { get; set; }
    public int TodayMl { get; set; }
    public int TodayCount { get; set; }
    public string? Advice { get; set; }
}
