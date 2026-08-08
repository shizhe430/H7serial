using Microsoft.EntityFrameworkCore;
using WaterDispenserWeb.Data;

namespace WaterDispenserWeb.Services;

/// <summary>
/// 每日饮水提醒服务
/// 每天固定时间向录入微信token、且当日饮水不足的老人推送DeepSeek建议
/// </summary>
public class ReminderService : BackgroundService
{
    private readonly IServiceScopeFactory _scopeFactory;
    private readonly ILogger<ReminderService> _logger;
    private static readonly TimeSpan _interval = TimeSpan.FromMinutes(5);
    private static DateTime _lastRemindDate = DateTime.MinValue;
    private static TimeSpan _remindTime = new TimeSpan(10, 0, 0); // 每天上午10点

    public ReminderService(IServiceScopeFactory scopeFactory, ILogger<ReminderService> logger)
    {
        _scopeFactory = scopeFactory;
        _logger = logger;
    }

    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        _logger.LogInformation("饮水提醒服务已启动，每天 {Hour}:{Minute} 推送", _remindTime.Hours, _remindTime.Minutes);
        while (!stoppingToken.IsCancellationRequested)
        {
            try
            {
                var now = DateTime.Now;
                // 到达提醒时间且当天未提醒过
                if (now.TimeOfDay >= _remindTime && _lastRemindDate != now.Date)
                {
                    await RemindAllAsync(now.Date);
                    _lastRemindDate = now.Date;
                }
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "饮水提醒任务执行失败");
            }
            await Task.Delay(_interval, stoppingToken);
        }
    }

    private async Task RemindAllAsync(DateTime today)
    {
        using var scope = _scopeFactory.CreateScope();
        var db = scope.ServiceProvider.GetRequiredService<AppDbContext>();
        var pushPlus = scope.ServiceProvider.GetRequiredService<PushPlusService>();

        // 找所有养老院饮水者，且已录入微信token
        var users = await db.Users
            .Where(u => u.Role == 2 && u.Section == "elderly" && !string.IsNullOrEmpty(u.Phone))
            .ToListAsync();

        if (users.Count == 0)
        {
            _logger.LogInformation("今日提醒: 无已绑定微信的老人");
            return;
        }

        var todaySummaries = await db.DailySummaries
            .Where(d => d.Date == today)
            .ToDictionaryAsync(d => d.UserId);

        int sentCount = 0;
        foreach (var user in users)
        {
            try
            {
                var summary = todaySummaries.GetValueOrDefault(user.Id);
                var targetMl = summary?.TargetMl ?? (int)(user.WeightKg * 35.0);
                var todayMl = summary?.TotalMl ?? 0;

                // 饮水达标或没有token则跳过
                if (todayMl >= targetMl || string.IsNullOrEmpty(user.Phone)) continue;

                // 推送内容 = DeepSeek建议（若有）否则用基础建议
                string content = summary?.AiAdvice ?? summary?.Advice;
                if (string.IsNullOrEmpty(content))
                {
                    content = $"{user.Name}，您今日已饮水{todayMl}ml（目标{targetMl}ml），还需{Math.Max(0, targetMl - todayMl)}ml，请记得多喝水。";
                }

                var (success, msg) = await pushPlus.SendAsync(user.Phone, $"💧{user.Name}的饮水提醒", content);
                _logger.LogInformation("推送提醒 {Name}: {Success} ({Msg})", user.Name, success, msg);
                if (success) sentCount++;
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "推送失败: 用户 {Name}", user.Name);
            }
        }
        _logger.LogInformation("今日饮水提醒完成，成功推送 {Count} 条", sentCount);
    }
}
