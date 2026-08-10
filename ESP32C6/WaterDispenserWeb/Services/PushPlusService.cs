using System.Net.Http.Json;
using System.Text.Json;

namespace WaterDispenserWeb.Services;

/// <summary>
/// Sends per-user notifications through PushPlus.
/// </summary>
public class PushPlusService
{
    private const string SendEndpoint = "https://www.pushplus.plus/send";
    private readonly HttpClient _httpClient;

    public PushPlusService(HttpClient httpClient)
    {
        _httpClient = httpClient;
    }

    public async Task<(bool success, string message)> SendAsync(
        string token,
        string title,
        string content,
        CancellationToken cancellationToken = default)
    {
        token = token?.Trim() ?? "";
        if (string.IsNullOrEmpty(token))
            return (false, "未配置 PushPlus Token");

        try
        {
            using var response = await _httpClient.PostAsJsonAsync(
                SendEndpoint,
                new
                {
                    token,
                    title,
                    content,
                    template = "txt",
                    channel = "wechat"
                },
                cancellationToken);

            var body = await response.Content.ReadAsStringAsync(cancellationToken);
            if (!response.IsSuccessStatusCode)
                return (false, $"PushPlus HTTP {(int)response.StatusCode}");

            using var doc = JsonDocument.Parse(body);
            if (!doc.RootElement.TryGetProperty("code", out var codeElement))
                return (false, "PushPlus 返回格式异常");

            var code = codeElement.ValueKind == JsonValueKind.Number
                ? codeElement.GetInt32()
                : int.TryParse(codeElement.GetString(), out var parsedCode) ? parsedCode : 0;
            var message = doc.RootElement.TryGetProperty("msg", out var messageElement)
                ? messageElement.GetString() ?? ""
                : "";

            return code == 200
                ? (true, string.IsNullOrWhiteSpace(message) ? "发送成功" : message)
                : (false, string.IsNullOrWhiteSpace(message) ? $"PushPlus 错误码 {code}" : message);
        }
        catch (OperationCanceledException) when (!cancellationToken.IsCancellationRequested)
        {
            return (false, "PushPlus 请求超时");
        }
        catch (Exception ex)
        {
            return (false, $"推送异常: {ex.Message}");
        }
    }
}
