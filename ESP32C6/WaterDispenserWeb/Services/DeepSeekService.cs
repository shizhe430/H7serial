using System.Text;
using System.Text.Json;

namespace WaterDispenserWeb.Services;

/// <summary>
/// DeepSeek API 服务 - 生成个性化饮水建议
/// </summary>
public class DeepSeekService
{
    private readonly HttpClient _httpClient;
    private readonly string _apiKey;
    private readonly string _apiUrl = "https://api.deepseek.com/v1/chat/completions";
    private readonly WeatherService _weather;

    public DeepSeekService(HttpClient httpClient, IConfiguration configuration, WeatherService weather)
    {
        _httpClient = httpClient;
        _apiKey = configuration["DeepSeek:ApiKey"] ?? "";
        _weather = weather;
    }

    /// <summary>
    /// 调用 DeepSeek 生成个性化饮水建议
    /// </summary>
    public async Task<string> GenerateAdviceAsync(string name, int age, int gender, float heightCm, float weightKg,
        int todayTotalMl, int targetMl, int useCount)
    {
        if (string.IsNullOrEmpty(_apiKey))
            return "AI建议暂不可用（未配置API密钥）";

        string genderText = gender switch { 1 => "男", 2 => "女", _ => "未知" };
        int neededMl = Math.Max(0, targetMl - todayTotalMl);
        double percent = targetMl > 0 ? (double)todayTotalMl / targetMl * 100 : 0;

        var prompt = $@"你是一个专业的饮水健康助手。请根据以下用户信息，给出简短、实用的个性化饮水建议（50字以内）：

用户信息：
- 姓名：{name}
- 年龄：{age}岁
- 性别：{genderText}
- 身高：{heightCm}cm
- 体重：{weightKg}kg
- 今日已饮水：{todayTotalMl}ml
- 今日饮水次数：{useCount}次
- 建议日饮水量：{targetMl}ml（体重×35ml）
- 还需饮水：{neededMl}ml
- 完成进度：{percent:F0}%

请根据完成进度和用户的身体情况，给出针对性的饮水建议。语气友好、鼓励。";

        var requestBody = new
        {
            model = "deepseek-chat",
            messages = new[]
            {
                new { role = "system", content = "你是一个专业的饮水健康助手，回答简洁、友好、实用，每次不超过50字。" },
                new { role = "user", content = prompt }
            },
            max_tokens = 200,
            temperature = 0.7
        };

        try
        {
            var json = JsonSerializer.Serialize(requestBody);
            var content = new StringContent(json, Encoding.UTF8, "application/json");

            var request = new HttpRequestMessage(HttpMethod.Post, _apiUrl);
            request.Headers.Add("Authorization", $"Bearer {_apiKey}");
            request.Content = content;

            var response = await _httpClient.SendAsync(request);
            response.EnsureSuccessStatusCode();

            var responseJson = await response.Content.ReadAsStringAsync();
            using var doc = JsonDocument.Parse(responseJson);

            var advice = doc.RootElement
                .GetProperty("choices")[0]
                .GetProperty("message")
                .GetProperty("content")
                .GetString();

            return advice?.Trim() ?? "未能生成建议";
        }
        catch (Exception ex)
        {
            return $"AI建议生成失败: {ex.Message}";
        }
    }

    /// <summary>
    /// 根据最近7天饮水情况生成 AI 建议（当天无饮水记录时使用）
    /// </summary>
    public async Task<string> GenerateWeeklyAdviceAsync(string name, int age, int gender, float heightCm, float weightKg,
        int targetMl, int weekTotalMl, int weekDays, int todayTotalMl)
    {
        if (string.IsNullOrEmpty(_apiKey))
            return "AI建议暂不可用（未配置API密钥）";

        string genderText = gender switch { 1 => "男", 2 => "女", _ => "未知" };
        int avgMl = weekDays > 0 ? weekTotalMl / weekDays : 0;
        int neededMl = Math.Max(0, targetMl - avgMl);
        double percent = targetMl > 0 ? (double)avgMl / targetMl * 100 : 0;

        var prompt = $@"你是一个专业的饮水健康助手。请根据以下用户信息，给出简短、实用的个性化饮水建议（50字以内）：

用户信息：
- 姓名：{name}
- 年龄：{age}岁
- 性别：{genderText}
- 身高：{heightCm}cm
- 体重：{weightKg}kg
- 今日已饮水：{todayTotalMl}ml
- 近7天日均饮水：{avgMl}ml（共{weekDays}天有饮水记录）
- 建议日饮水量：{targetMl}ml（体重×35ml）
- 近7天日均完成进度：{percent:F0}%
- 按近7天情况，每天还需饮水约：{neededMl}ml

请根据近7天整体情况和用户的身体情况，给出针对性的饮水建议。语气友好、鼓励。";

        var requestBody = new
        {
            model = "deepseek-chat",
            messages = new[]
            {
                new { role = "system", content = "你是一个专业的饮水健康助手，回答简洁、友好、实用，每次不超过50字。" },
                new { role = "user", content = prompt }
            },
            max_tokens = 200,
            temperature = 0.7
        };

        try
        {
            var json = JsonSerializer.Serialize(requestBody);
            var content = new StringContent(json, Encoding.UTF8, "application/json");

            var request = new HttpRequestMessage(HttpMethod.Post, _apiUrl);
            request.Headers.Add("Authorization", $"Bearer {_apiKey}");
            request.Content = content;

            var response = await _httpClient.SendAsync(request);
            response.EnsureSuccessStatusCode();

            var responseJson = await response.Content.ReadAsStringAsync();
            using var doc = JsonDocument.Parse(responseJson);

            var advice = doc.RootElement
                .GetProperty("choices")[0]
                .GetProperty("message")
                .GetProperty("content")
                .GetString();

            return advice?.Trim() ?? "未能生成建议";
        }
        catch (Exception ex)
        {
            return $"AI建议生成失败: {ex.Message}";
        }
    }

    /// <summary>
    /// 交互式问答 - 多轮对话，DeepSeek 结合该用户的个人健康数据 + 对话历史给出个性化回答
    /// history 格式: 每项为 (role, content)，role ∈ {user, assistant}
    /// </summary>
    public async Task<string> ChatAsync(string name, int age, int gender, float heightCm, float weightKg,
        int todayTotalMl, int useCount, int targetMl, List<(string role, string content)> history)
    {
        if (string.IsNullOrEmpty(_apiKey))
            return "AI问答暂不可用（未配置API密钥）";

        string genderText = gender switch { 1 => "男", 2 => "女", _ => "未知" };
        int neededMl = Math.Max(0, targetMl - todayTotalMl);
        double percent = targetMl > 0 ? (double)todayTotalMl / targetMl * 100 : 0;

        var systemPrompt = $@"你是智能饮水机的全能智能助手。当前对话中，你只掌握了下面这一位用户的数据。

用户信息（仅此一位，其余用户的数据你一概不知道）：
- 姓名：{name}
- 年龄：{age}岁
- 性别：{genderText}
- 身高：{heightCm}cm
- 体重：{weightKg}kg
- 今日已饮水：{todayTotalMl}ml（共{useCount}次）
- 建议日饮水量：{targetMl}ml（体重×35ml）
- 还需饮水：{neededMl}ml
- 今日完成进度：{percent:F0}%

回答规则：
1. 结合这位用户的个人数据，尽力回答关于他/她的饮水、健康等个性化问题。
2. 除涉及用户隐私外，其他任何问题（生活常识、健康咨询、知识问答、聊天等）都请尽最大可能解答，给出有帮助、准确的回答。
3. 隐私红线：绝对不能透露、猜测或编造其他用户的任何信息（姓名、饮水量、身体状况、账号等）。如果有人询问其他用户的情况，请礼貌说明你无法提供他人信息。
4. 涉及严重健康异常或需要就医的情况，请建议及时就医，并注明健康建议仅供参考。
5. 回答语气友好、通俗易懂；正常问答不受字数限制，除非问题简单。
6. 你拥有完整的对话记忆，可以回顾本次对话中之前的问答，自然地延续话题、回应追问。";

        // 天气相关问题时，获取实时天气数据注入上下文，让 AI 基于真实数据回答
        var lastUserContent = history.Count > 0 ? history[^1].content : "";
        if (_weather != null && _weather.IsWeatherQuestion(lastUserContent))
        {
            var weatherText = await _weather.GetWeatherSummaryAsync(lastUserContent);
            if (!string.IsNullOrEmpty(weatherText))
            {
                systemPrompt += $"\n\n【以下是刚刚实时获取的天气预报数据，回答天气相关问题时必须以此为准，不要编造】\n{weatherText}";
            }
            else
            {
                systemPrompt += "\n\n【注：实时天气数据获取失败，请如实告知用户暂时无法获取天气信息，不要编造】";
            }
        }

        // 只保留最近 12 条历史，避免上下文过长
        var recentHistory = history.Count > 12 ? history.GetRange(history.Count - 12, 12) : history;

        var messages = new List<object> { new { role = "system", content = systemPrompt } };
        foreach (var (role, content) in recentHistory)
        {
            messages.Add(new { role, content });
        }
        // 确保最后一条是用户消息
        if (recentHistory.Count == 0 || recentHistory[^1].role != "user")
            messages.Add(new { role = "user", content = "请继续回答。" });

        var requestBody = new
        {
            model = "deepseek-chat",
            messages,
            max_tokens = 800,
            temperature = 0.7
        };

        try
        {
            var json = JsonSerializer.Serialize(requestBody);
            var content = new StringContent(json, Encoding.UTF8, "application/json");

            var request = new HttpRequestMessage(HttpMethod.Post, _apiUrl);
            request.Headers.Add("Authorization", $"Bearer {_apiKey}");
            request.Content = content;

            var response = await _httpClient.SendAsync(request);
            response.EnsureSuccessStatusCode();

            var responseJson = await response.Content.ReadAsStringAsync();
            using var doc = JsonDocument.Parse(responseJson);

            var answer = doc.RootElement
                .GetProperty("choices")[0]
                .GetProperty("message")
                .GetProperty("content")
                .GetString();

            return answer?.Trim() ?? "未能生成回答";
        }
        catch (Exception ex)
        {
            return $"AI问答失败: {ex.Message}";
        }
    }

    public async Task<string> AskDeviceAsync(string prompt)
    {
        if (string.IsNullOrEmpty(_apiKey))
            return "AI建议暂不可用（未配置API密钥）";

        var requestBody = new
        {
            model = "deepseek-chat",
            messages = new[]
            {
                new { role = "system", content = "你是智能饮水机的健康助手。严格按用户要求的格式简短回答。" },
                new { role = "user", content = prompt }
            },
            max_tokens = 200,
            temperature = 0.7
        };

        try
        {
            var request = new HttpRequestMessage(HttpMethod.Post, _apiUrl);
            request.Headers.Add("Authorization", $"Bearer {_apiKey}");
            request.Content = new StringContent(
                JsonSerializer.Serialize(requestBody), Encoding.UTF8, "application/json");

            var response = await _httpClient.SendAsync(request);
            response.EnsureSuccessStatusCode();

            var responseJson = await response.Content.ReadAsStringAsync();
            using var doc = JsonDocument.Parse(responseJson);
            return doc.RootElement.GetProperty("choices")[0]
                .GetProperty("message").GetProperty("content").GetString()?.Trim()
                ?? "未能生成建议";
        }
        catch (Exception ex)
        {
            return $"AI建议生成失败: {ex.Message}";
        }
    }
}
