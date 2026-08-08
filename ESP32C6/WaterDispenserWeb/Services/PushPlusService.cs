using System.Text.Json;

namespace WaterDispenserWeb.Services;

/// <summary>
/// PushPlus 微信推送服务
/// 通过微信接收饮水提醒消息，免费版每日约200条
/// </summary>
public class PushPlusService
{
    private readonly HttpClient _httpClient;

    public PushPlusService(HttpClient httpClient)
    {
        _httpClient = httpClient;
    }

    /// <summary>
    /// 发送微信推送消息
    /// </summary>
    /// <param name="token">用户微信绑定的PushPlus token</param>
    /// <param name="title">消息标题</param>
    /// <param name="content">消息内容（饮水建议）</param>
    /// <returns>(成功与否, 返回信息)</returns>
    public async Task<(bool success, string message)> SendAsync(string token, string title, string content)
    {
        if (string.IsNullOrEmpty(token))
            return (false, "未配置微信token");

        var url = $"http://www.pushplus.plus/send?token={Uri.EscapeDataString(token)}" +
                  $"&title={Uri.EscapeDataString(title)}" +
                  $"&content={Uri.EscapeDataString(content)}";

        try
        {
            var response = await _httpClient.GetAsync(url);
            var body = await response.Content.ReadAsStringAsync();

            using var doc = JsonDocument.Parse(body);
            if (doc.RootElement.TryGetProperty("code", out var codeEl))
            {
                int code = codeEl.GetInt32();
                string msg = doc.RootElement.TryGetProperty("msg", out var msgEl) ? msgEl.GetString() ?? "" : "";
                return code == 200 ? (true, msg) : (false, msg);
            }
            return (false, body);
        }
        catch (Exception ex)
        {
            return (false, $"推送异常: {ex.Message}");
        }
    }
}
