using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using WaterDispenserWeb.Data;
using WaterDispenserWeb.Models;
using WaterDispenserWeb.Services;

namespace WaterDispenserWeb.Controllers;

/// <summary>
/// API 控制器 - 供 ESP32 调用，不需要登录验证
/// 注意：生产环境建议增加 API 鉴权（如 Token）
/// </summary>
[Route("api")]
[ApiController]
public class ApiController : ControllerBase
{
    private readonly AppDbContext _db;
    private readonly WaterAdviceService _advice;
    private readonly BillingService _billing;
    private readonly DeepSeekService _deepSeek;
    private readonly IServiceScopeFactory _scopeFactory;
    private readonly ILogger<ApiController> _logger;

    public ApiController(AppDbContext db, WaterAdviceService advice, BillingService billing, DeepSeekService deepSeek,
        IServiceScopeFactory scopeFactory, ILogger<ApiController> logger)
    {
        _db = db;
        _advice = advice;
        _billing = billing;
        _deepSeek = deepSeek;
        _scopeFactory = scopeFactory;
        _logger = logger;
    }

    /// <summary>
    /// ESP32 上报饮水记录
    /// POST /api/records
    /// 参数: user_id, volume_ml（本次增量）, target_ml（可选，C6 口径目标值）
    /// 网页端为权威累计器：volume_ml>0 时自己累加 TotalMl+=volume_ml / UseCount+=1
    /// today_total_ml / today_use_count 参数保留绑定但忽略（兼容旧固件）
    /// volume_ml==0 视为纯探活/纯同步，不写任何库
    /// </summary>
    [HttpPost("records")]
    public async Task<IActionResult> AddRecord([FromForm] int user_id, [FromForm] int volume_ml,
        [FromForm] int? today_total_ml = null, [FromForm] int? today_use_count = null,
        [FromForm] int? target_ml = null)
    {
        if (volume_ml < 0)
            return BadRequest(new { ok = false, message = "volume_ml 不能为负数" });

        var user = await _db.Users.FindAsync(user_id);
        if (user == null)
            return NotFound(new { ok = false, message = "未找到该用户" });

        if (user.Role != 2 && user.Role != 3)
            return BadRequest(new { ok = false, message = "只有饮水者或计费用户可以上报记录" });

        var now = DateTime.Now;
        var today = now.Date;

        // 纯同步/探活：volume_ml==0 不写任何库，只返回当前汇总（兼容旧固件识别流程）
        if (volume_ml == 0)
        {
            var cur = await _db.DailySummaries
                .FirstOrDefaultAsync(d => d.UserId == user_id && d.Date == today);
            var curTarget = cur?.TargetMl ?? _advice.CalculateTargetMl(user.WeightKg);
            return Ok(new
            {
                ok = true,
                message = "同步完成",
                data = new
                {
                    user_name = user.Name,
                    volume_ml = 0,
                    today_total = cur?.TotalMl ?? 0,
                    today_count = cur?.UseCount ?? 0,
                    target_ml = curTarget,
                    needed_ml = Math.Max(0, curTarget - (cur?.TotalMl ?? 0)),
                    advice = cur?.Advice
                }
            });
        }

        // 新增饮水记录明细（volume_ml 是本次增量）
        _db.Records.Add(new Record
        {
            UserId = user_id,
            VolumeMl = volume_ml,
            RecordedAt = now
        });

        // 每日汇总：网页端权威累加（不再被 C6 的总量覆盖）
        var summary = await _db.DailySummaries
            .FirstOrDefaultAsync(d => d.UserId == user_id && d.Date == today);

        if (summary == null)
        {
            var targetMl = target_ml ?? _advice.CalculateTargetMl(user.WeightKg);
            summary = new DailySummary
            {
                UserId = user_id,
                Date = today,
                TotalMl = volume_ml,
                UseCount = 1,
                TargetMl = targetMl,
                Advice = _advice.GenerateAdvice(user.Name, user.Age, user.Gender,
                    targetMl, volume_ml, 1)
            };
            _db.DailySummaries.Add(summary);
        }
        else
        {
            summary.TotalMl += volume_ml;                     // 权威累加
            summary.UseCount += 1;
            if (target_ml.HasValue) summary.TargetMl = target_ml.Value;
            summary.Advice = _advice.GenerateAdvice(user.Name, user.Age, user.Gender,
                summary.TargetMl, summary.TotalMl, summary.UseCount);
            summary.UpdatedAt = now;
        }

        await _db.SaveChangesAsync();

        // 计费板块：自动扣费（仅真实出水量时扣费，纯同步请求 volume_ml==0 不扣）
        if (volume_ml > 0 && user.Section == "billing" && user.Role == 3)
        {
            var account = await _db.BillingAccounts.FirstOrDefaultAsync(a => a.UserId == user.Id);
            if (account != null)
            {
                var cost = _billing.CalculateCost(volume_ml, account.RatePerMl);
                var balanceBefore = account.Balance;
                account.Balance -= cost;
                account.TotalConsumed += cost;
                account.UpdatedAt = DateTime.Now;

                _db.BillingRecords.Add(new BillingRecord
                {
                    UserId = user.Id,
                    VolumeMl = volume_ml,
                    Cost = cost,
                    BalanceBefore = balanceBefore,
                    BalanceAfter = account.Balance,
                    RecordedAt = DateTime.Now
                });
                await _db.SaveChangesAsync();
                _logger.LogInformation("计费扣款: 用户 {Name} 扣费 {Cost}元, 余额 {Balance}元",
                    user.Name, cost, account.Balance);

                // 计费用户绑定了微信token时，推送扣费提醒
                if (!string.IsNullOrEmpty(user.Phone))
                {
                    var pushPlus = HttpContext.RequestServices.GetRequiredService<PushPlusService>();
                    var content = $"{user.Name}，本次饮水{volume_ml}ml，扣费{cost:F2}元，当前余额{account.Balance:F2}元。";
                    if (account.Balance < 1m)
                        content += " 余额不足1元，请及时充值！";
                    else if (account.Balance < 0.08m)
                        content += " 余额已低于0.08元，将无法出水，请立即充值！";
                    var (ok, msg) = await pushPlus.SendAsync(user.Phone, $"💰 {user.Name}的扣费提醒", content);
                    _logger.LogInformation("推送扣费提醒到微信 {Name}: {Ok} ({Msg})", user.Name, ok, msg);
                }
            }
        }

        // 后台调用 DeepSeek 生成 AI 建议（不阻塞 ESP32 返回；纯同步请求 volume_ml==0 已在前面提前返回）
        var dbUserId = user.Id;
        var summaryId = summary.Id;

        _ = Task.Run(async () =>
        {
            try
            {
                using var scope = _scopeFactory.CreateScope();
                var db = scope.ServiceProvider.GetRequiredService<AppDbContext>();
                var deepSeek = scope.ServiceProvider.GetRequiredService<DeepSeekService>();
                var pushPlus = scope.ServiceProvider.GetRequiredService<PushPlusService>();
                var latestUser = await db.Users.FindAsync(dbUserId);
                var latestSummary = await db.DailySummaries.FindAsync(summaryId);
                if (latestUser == null || latestSummary == null) return;

                var aiAdvice = await deepSeek.GenerateAdviceAsync(
                    latestUser.Name, latestUser.Age, latestUser.Gender,
                    latestUser.HeightCm, latestUser.WeightKg,
                    latestSummary.TotalMl, latestSummary.TargetMl, latestSummary.UseCount
                );

                latestSummary.AiAdvice = aiAdvice;
                await db.SaveChangesAsync();
                _logger.LogInformation("AI建议已生成: 用户 {Name}", latestUser.Name);

                // 若用户绑定了微信token，推送DeepSeek建议
                if (!string.IsNullOrEmpty(latestUser.Phone))
                {
                    var (ok, msg) = await pushPlus.SendAsync(latestUser.Phone,
                        $"💧 {latestUser.Name}的饮水建议", aiAdvice);
                    _logger.LogInformation("推送饮水建议到微信 {Name}: {Ok} ({Msg})", latestUser.Name, ok, msg);
                }
            }
            catch (Exception ex)
            {
                _logger.LogWarning(ex, "AI建议生成失败: 用户 {Name}", user.Name);
            }
        });

        _logger.LogInformation("饮水记录: 用户 {Name}({UserId}) 饮水 {Volume}ml", user.Name, user_id, volume_ml);

        return Ok(new
        {
            ok = true,
            message = "记录成功",
            data = new
            {
                user_name = user.Name,
                volume_ml,
                today_total = summary.TotalMl,
                today_count = summary.UseCount,
                target_ml = summary.TargetMl,
                needed_ml = Math.Max(0, summary.TargetMl - summary.TotalMl),
                advice = summary.Advice
            }
        });
    }

    /// <summary>
    /// 获取用户今日汇总
    /// GET /api/summary?user_id=xxx
    /// </summary>
    [HttpGet("summary")]
    public async Task<IActionResult> GetSummary([FromQuery] int user_id)
    {
        var user = await _db.Users.FindAsync(user_id);
        if (user == null)
            return NotFound(new { ok = false, message = "未找到该用户" });

        var today = DateTime.Today;
        var summary = await _db.DailySummaries
            .FirstOrDefaultAsync(d => d.UserId == user_id && d.Date == today);

        var targetMl = summary?.TargetMl ?? _advice.CalculateTargetMl(user.WeightKg);

        return Ok(new
        {
            ok = true,
            data = new
            {
                user_id = user.Id,
                today_date = today.ToString("yyyy-MM-dd"),
                user_name = user.Name,
                today_total_ml = summary?.TotalMl ?? 0,
                today_use_count = summary?.UseCount ?? 0,
                target_ml = targetMl,
                needed_ml = Math.Max(0, targetMl - (summary?.TotalMl ?? 0)),
                advice = summary?.Advice ?? "暂无数据",
                ai_advice = summary?.AiAdvice
            }
        });
    }

    /// <summary>
    /// 获取用户信息
    /// GET /api/users/{id}
    /// </summary>
    [HttpGet("users/{id}")]
    public async Task<IActionResult> GetUser(int id)
    {
        var user = await _db.Users.FindAsync(id);
        if (user == null)
            return NotFound(new { ok = false, message = "未找到该用户" });

        return Ok(new
        {
            ok = true,
            data = new
            {
                id = user.Id,
                name = user.Name,
                gender = user.Gender,
                age = user.Age,
                height_cm = user.HeightCm,
                weight_kg = user.WeightKg,
                role = user.Role
            }
        });
    }

    /// <summary>
    /// 更新用户个人信息（ESP32 从 App 录入后同步到 Web 数据库）
    /// POST /api/users/update  body: user_id, name, age, height_cm, weight_kg
    /// </summary>
    [HttpPost("users/update")]
    public async Task<IActionResult> UpdateUserInfo([FromForm] int user_id, [FromForm] string? name = null,
        [FromForm] int? age = null, [FromForm] float? height_cm = null, [FromForm] float? weight_kg = null,
        [FromForm] int? gender = null)
    {
        var user = await _db.Users.FindAsync(user_id);
        if (user == null)
            return NotFound(new { ok = false, message = "未找到该用户" });

        if (!string.IsNullOrWhiteSpace(name)) user.Name = name.Trim();
        if (age.HasValue) user.Age = age.Value;
        if (height_cm.HasValue) user.HeightCm = height_cm.Value;
        if (weight_kg.HasValue) user.WeightKg = weight_kg.Value;
        if (gender.HasValue) user.Gender = gender.Value;
        user.UpdatedAt = DateTime.Now;

        await _db.SaveChangesAsync();

        return Ok(new
        {
            ok = true,
            message = "用户信息已更新",
            data = new
            {
                id = user.Id,
                name = user.Name,
                gender = user.Gender,
                age = user.Age,
                height_cm = user.HeightCm,
                weight_kg = user.WeightKg
            }
        });
    }

    /// <summary>C6 的低内存 AI 代理，避免在设备端处理 HTTPS/TLS。</summary>
    [HttpGet("ask")]
    public async Task<IActionResult> AskDevice([FromQuery] string q)
    {
        if (string.IsNullOrWhiteSpace(q))
            return BadRequest(new { ok = false, reply = "问题不能为空" });

        var reply = await _deepSeek.AskDeviceAsync(q.Trim());
        return Ok(new { ok = true, reply });
    }
}
