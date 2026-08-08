using WaterDispenserWeb.Models;

namespace WaterDispenserWeb.Services;

/// <summary>
/// 计费服务 - 管理饮水扣费、余额提醒
/// 计费规则：每20ml收费0.01元，向下取整
/// 例如：350ml = floor(350/20) × 0.01 = 17 × 0.01 = 0.17元
/// </summary>
public class BillingService
{
    private const int UNIT_ML = 20;
    private const decimal UNIT_PRICE = 0.01m;
    private readonly decimal _lowBalanceWarning = 1.00m;
    private readonly decimal _minBalance = 0.08m;

    /// <summary>计算本次饮水费用（向下取整）</summary>
    public decimal CalculateCost(int volumeMl, decimal ratePerMl)
    {
        int units = volumeMl / UNIT_ML;  // 向下取整
        if (units == 0) return 0;
        return units * UNIT_PRICE;
    }

    /// <summary>检查余额是否足够出水</summary>
    public (bool canDispense, string message) CheckCanDispense(decimal balance)
    {
        if (balance < _minBalance)
            return (false, "❌ 饮水余额太少（低于0.08元），无法出水，请及时充值");
        if (balance < _lowBalanceWarning)
            return (true, $"⚠️ 余额不足1元（当前{balance:F2}元），请及时充值");
        return (true, "");
    }

    /// <summary>根据余额计算最多可出水量</summary>
    public int CalculateMaxMl(decimal balance)
    {
        if (balance < UNIT_PRICE) return 0;
        int maxUnits = (int)(balance / UNIT_PRICE);
        return maxUnits * UNIT_ML;
    }

    /// <summary>生成余额建议</summary>
    public string GenerateBalanceAdvice(decimal balance, decimal ratePerMl, int todayMl)
    {
        int maxMl = CalculateMaxMl(balance);
        string advice;

        if (balance < _minBalance)
            advice = "余额极低，无法出水，请立即充值。";
        else if (balance < _lowBalanceWarning)
            advice = $"余额不足1元，最多可饮用约{maxMl}ml水，请尽快充值。";
        else if (balance < 5)
            advice = $"余额{balance:F2}元，最多可饮用约{maxMl}ml，建议及时充值。";
        else
            advice = $"余额充足（{balance:F2}元），最多可饮用约{maxMl}ml。";

        if (todayMl > 0)
            advice += $" 今日已饮水{todayMl}ml。";

        return advice;
    }
}
