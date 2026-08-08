namespace WaterDispenserWeb.Services;

/// <summary>
/// 饮水建议服务
/// 根据体重法计算每日建议饮水量: 体重(kg) × 35ml
/// 并根据当前饮水情况给出个性化建议
/// </summary>
public class WaterAdviceService
{
    private const double ML_PER_KG = 35.0;

    /// <summary>
    /// 计算每日建议饮水量
    /// </summary>
    public int CalculateTargetMl(float weightKg)
    {
        if (weightKg <= 0) return 1700; // 默认值
        return (int)(weightKg * ML_PER_KG);
    }

    /// <summary>
    /// 生成个性化饮水建议
    /// </summary>
    /// <param name="name">姓名</param>
    /// <param name="age">年龄</param>
    /// <param name="gender">性别: 0=未知 1=男 2=女</param>
    /// <param name="targetMl">建议饮水量</param>
    /// <param name="totalMl">已饮水量</param>
    /// <param name="useCount">饮水次数</param>
    /// <returns>建议文本</returns>
    public string GenerateAdvice(string name, int age, int gender, int targetMl, int totalMl, int useCount)
    {
        int needed = Math.Max(0, targetMl - totalMl);
        double percent = targetMl > 0 ? (double)totalMl / targetMl * 100 : 0;

        if (totalMl == 0)
        {
            return $"{name}，您今天还没有喝水。建议每日饮水 {targetMl}ml，请记得及时补充水分。";
        }

        if (percent >= 100)
        {
            return $"👍 {name}，您今日饮水已达标（{totalMl}ml），继续保持良好的饮水习惯！";
        }

        if (percent >= 80)
        {
            return $"💪 {name}，今日已饮水 {totalMl}ml，已完成 {percent:F0}%，还差 {needed}ml 就达标了，加油！";
        }

        if (percent >= 50)
        {
            return $"💧 {name}，今日已饮水 {totalMl}ml，完成 {percent:F0}%。还差 {needed}ml，请继续补充水分。";
        }

        if (useCount > 0 && percent < 30)
        {
            return $"⚠️ {name}，今日饮水偏少（{totalMl}ml / {targetMl}ml），仅完成 {percent:F0}%。请增加饮水量，建议每小时喝一杯水。";
        }

        return $"{name}，今日已饮水 {totalMl}ml（目标 {targetMl}ml），还需饮水 {needed}ml。请记得按时喝水。";
    }

    /// <summary>
    /// 根据最近7天饮水情况生成建议（当天无饮水记录时使用）
    /// 格式与日常建议保持一致
    /// </summary>
    /// <param name="name">姓名</param>
    /// <param name="age">年龄</param>
    /// <param name="gender">性别</param>
    /// <param name="targetMl">每日建议饮水量</param>
    /// <param name="weekTotalMl">近7天总饮水量</param>
    /// <param name="weekDays">近7天有饮水的天数</param>
    /// <param name="todayUseCount">今日饮水次数（通常为0）</param>
    /// <returns>建议文本</returns>
    public string GenerateWeeklyAdvice(string name, int age, int gender, int targetMl, int weekTotalMl, int weekDays, int todayUseCount)
    {
        int avgMl = weekDays > 0 ? weekTotalMl / weekDays : 0;
        double percent = targetMl > 0 ? (double)avgMl / targetMl * 100 : 0;
        int needed = Math.Max(0, targetMl - avgMl);

        if (weekDays == 0)
        {
            return $"{name}，您近7天都没有饮水记录。建议每日饮水 {targetMl}ml，请记得及时补充水分。";
        }

        if (weekDays < 7)
        {
            return $"💧 {name}，您近7天只有{weekDays}天饮水，日均{avgMl}ml（目标{targetMl}ml）。饮水不够规律，建议每天都按时补充水分。";
        }

        if (percent >= 100)
        {
            return $"👍 {name}，您近7天饮水规律且充足（日均{avgMl}ml），继续保持良好的饮水习惯！";
        }

        if (percent >= 80)
        {
            return $"💪 {name}，您近7天日均饮水{avgMl}ml，已完成目标的{percent:F0}%，还差{needed}ml，继续加油！";
        }

        if (percent >= 50)
        {
            return $"💧 {name}，您近7天日均饮水{avgMl}ml，完成目标的{percent:F0}%。还差{needed}ml，建议增加饮水量。";
        }

        if (percent < 30)
        {
            return $"⚠️ {name}，您近7天日均饮水偏少（{avgMl}ml / {targetMl}ml），仅完成{percent:F0}%。请增加饮水量，建议每小时喝一杯水。";
        }

        return $"{name}，您近7天日均饮水{avgMl}ml（目标{targetMl}ml），还需平均每天饮水{needed}ml。请记得按时喝水。";
    }
}
