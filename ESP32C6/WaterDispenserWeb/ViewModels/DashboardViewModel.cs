namespace WaterDispenserWeb.ViewModels;

/// <summary>
/// 仪表盘视图模型 - 用户今日饮水概况
/// </summary>
public class DashboardViewModel
{
    public int UserId { get; set; }
    public string Name { get; set; } = "";
    public string Username { get; set; } = "";
    public int Role { get; set; }
    public string RoleName { get; set; } = "";

    // 个人信息
    public int Gender { get; set; }
    public int Age { get; set; }
    public float HeightCm { get; set; }
    public float WeightKg { get; set; }

    // 今日饮水数据
    public int TodayTotalMl { get; set; }
    public int TodayUseCount { get; set; }
    public int TargetMl { get; set; }
    public int NeededMl => Math.Max(0, TargetMl - TodayTotalMl);
    public double ProgressPercent => TargetMl > 0 ? Math.Min(100.0, (double)TodayTotalMl / TargetMl * 100) : 0;

    // 个性化建议
    public string? Advice { get; set; }

    // AI智能建议（DeepSeek）
    public string? AiAdvice { get; set; }

    // 近7天数据（用于图表）
    public List<DailySummaryItem> RecentWeek { get; set; } = new();

    // 今日饮水记录列表
    public List<DailySummaryItem> RecentRecords { get; set; } = new();
}

/// <summary>
/// 每日汇总项
/// </summary>
public class DailySummaryItem
{
    public string DateStr { get; set; } = "";
    public int TotalMl { get; set; }
    public int UseCount { get; set; }
    public int TargetMl { get; set; }
    public int NeededMl => Math.Max(0, TargetMl - TotalMl);
}

/// <summary>
/// 饮水者概览（管理员/护工查看列表用）
/// </summary>
public class DrinkerOverview
{
    public int UserId { get; set; }
    public string Name { get; set; } = "";
    public string Username { get; set; } = "";
    public int Gender { get; set; }
    public int Age { get; set; }
    public float HeightCm { get; set; }
    public float WeightKg { get; set; }
    public int TodayTotalMl { get; set; }
    public int TodayUseCount { get; set; }
    public int TargetMl { get; set; }
    public int NeededMl => Math.Max(0, TargetMl - TodayTotalMl);
    public double ProgressPercent => TargetMl > 0 ? Math.Min(100.0, (double)TodayTotalMl / TargetMl * 100) : 0;
    public string? Advice { get; set; }
}
