using System.Text.Json;

namespace WaterDispenserWeb.Services;

/// <summary>
/// 天气服务 - 通过 wttr.in 获取实时天气（免费无需key）
/// 中文城市名自动映射为拼音，如 杭州→Hangzhou
/// </summary>
public class WeatherService
{
    private readonly HttpClient _httpClient;
    private readonly string _defaultCity;

    // 常见城市中文→拼音映射（可扩展）
    private static readonly Dictionary<string, string> CityMap = new(StringComparer.OrdinalIgnoreCase)
    {
        ["北京"] = "Beijing", ["上海"] = "Shanghai", ["广州"] = "Guangzhou",
        ["深圳"] = "Shenzhen", ["杭州"] = "Hangzhou", ["南京"] = "Nanjing",
        ["苏州"] = "Suzhou", ["成都"] = "Chengdu", ["重庆"] = "Chongqing",
        ["武汉"] = "Wuhan", ["西安"] = "Xian", ["长沙"] = "Changsha",
        ["郑州"] = "Zhengzhou", ["青岛"] = "Qingdao", ["天津"] = "Tianjin",
        ["宁波"] = "Ningbo", ["无锡"] = "Wuxi", ["厦门"] = "Xiamen",
        ["福州"] = "Fuzhou", ["合肥"] = "Hefei", ["济南"] = "Jinan",
        ["昆明"] = "Kunming", ["贵阳"] = "Guiyang", ["沈阳"] = "Shenyang",
        ["大连"] = "Dalian", ["长春"] = "Changchun", ["哈尔滨"] = "Harbin",
        ["太原"] = "Taiyuan", ["石家庄"] = "Shijiazhuang", ["南昌"] = "Nanchang",
        ["南宁"] = "Nanning", ["海口"] = "Haikou", ["兰州"] = "Lanzhou",
        ["乌鲁木齐"] = "Urumqi", ["呼和浩特"] = "Hohhot", ["银川"] = "Yinchuan",
        ["西宁"] = "Xining", ["拉萨"] = "Lhasa", ["东莞"] = "Dongguan",
        ["佛山"] = "Foshan", ["温州"] = "Wenzhou", ["泉州"] = "Quanzhou",
        ["烟台"] = "Yantai", ["徐州"] = "Xuzhou", ["唐山"] = "Tangshan",
        ["洛阳"] = "Luoyang", ["襄阳"] = "Xiangyang", ["保定"] = "Baoding"
    };

    // 天气关键词，命中则注入天气数据
    private static readonly string[] WeatherKeywords =
    {
        "天气", "气温", "温度", "下雨", "降雨", "晴天", "阴天", "多云",
        "降温", "升温", "预报", "热不热", "冷不冷", "穿衣", "空气", "湿度",
        "风力", "风大", "紫外线", "雨水", "暴雨", "台风", "今天热", "明天气"
    };

    public WeatherService(HttpClient httpClient, IConfiguration configuration)
    {
        _httpClient = httpClient;
        _defaultCity = configuration["Weather:DefaultCity"] ?? "Hangzhou";
    }

    /// <summary>判断问题是否与天气相关</summary>
    public bool IsWeatherQuestion(string question)
    {
        if (string.IsNullOrWhiteSpace(question)) return false;
        return WeatherKeywords.Any(k => question.Contains(k, StringComparison.OrdinalIgnoreCase));
    }

    /// <summary>从问题中提取城市拼音，未提及则返回默认城市</summary>
    public string ExtractCity(string question)
    {
        if (!string.IsNullOrWhiteSpace(question))
        {
            foreach (var (cn, pinyin) in CityMap)
            {
                if (question.Contains(cn, StringComparison.OrdinalIgnoreCase))
                    return pinyin;
            }
        }
        return _defaultCity;
    }

    /// <summary>
    /// 获取天气摘要（含当前天气 + 今天 + 未来两天预报），失败返回 null
    /// </summary>
    public async Task<string?> GetWeatherSummaryAsync(string question)
    {
        var city = ExtractCity(question);
        try
        {
            var url = $"https://wttr.in/{Uri.EscapeDataString(city)}?format=j1&lang=zh";
            var response = await _httpClient.GetStringAsync(url);
            using var doc = JsonDocument.Parse(response);
            var root = doc.RootElement;

            var current = root.GetProperty("current_condition")[0];
            var tempC = current.GetProperty("temp_C").GetString();
            var feelsC = current.GetProperty("FeelsLikeC").GetString();
            var humidity = current.GetProperty("humidity").GetString();
            var desc = current.GetProperty("weatherDesc")[0].GetProperty("value").GetString();
            var windSpeed = current.GetProperty("windspeedKmph").GetString();
            var uv = current.GetProperty("uvIndex").GetString();

            // 近几小时预报（hourly 嵌套在 weather[0] 中）
            var todayWeather = root.GetProperty("weather")[0];
            var hourly = todayWeather.GetProperty("hourly").EnumerateArray().Take(6)
                .Select(h =>
                {
                    var t = int.Parse(h.GetProperty("tempC").GetString()!);
                    var d = h.GetProperty("lang_zh")[0].GetProperty("value").GetString() ?? h.GetProperty("weatherDesc")[0].GetProperty("value").GetString();
                    var time = h.GetProperty("time").GetString();
                    var hour = time switch
                    {
                        "0" => 0, "100" => 1, "200" => 2, "300" => 3, "400" => 4, "500" => 5,
                        "600" => 6, "700" => 7, "800" => 8, "900" => 9, "1000" => 10, "1100" => 11,
                        "1200" => 12, "1300" => 13, "1400" => 14, "1500" => 15, "1600" => 16,
                        "1700" => 17, "1800" => 18, "1900" => 19, "2000" => 20, "2100" => 21,
                        "2200" => 22, "2300" => 23, _ => -1
                    };
                    return new { hour, tempC = t, desc = d };
                })
                .Where(x => x.hour >= 0)
                .ToList();

            // 未来几天预报
            var forecast = root.GetProperty("weather").EnumerateArray().Select(w =>
            {
                var date = w.GetProperty("date").GetString();
                var maxT = w.GetProperty("maxtempC").GetString();
                var minT = w.GetProperty("mintempC").GetString();
                var dayDesc = w.GetProperty("hourly")[0].GetProperty("lang_zh")[0].GetProperty("value").GetString();
                var rain = w.GetProperty("hourly")[0].GetProperty("chanceofrain").GetString();
                return new { date, maxT, minT, desc = dayDesc, rain };
            }).Take(3).ToList();

            var cityName = CityMap.FirstOrDefault(x => x.Value.Equals(city, StringComparison.OrdinalIgnoreCase)).Key ?? city;

            var sb = new System.Text.StringBuilder();
            sb.AppendLine($"【城市】{cityName}");
            sb.AppendLine($"【当前】{desc}，气温{tempC}°C，体感{feelsC}°C，湿度{humidity}%，风速{windSpeed}km/h，紫外线指数{uv}");
            if (hourly.Any())
            {
                var hourlyText = string.Join("、", hourly.Take(6).Select(h => $"{h.hour}时:{h.desc}{h.tempC}°C"));
                sb.AppendLine($"【今日逐时】{hourlyText}");
            }
            if (forecast.Any())
            {
                var forecastText = string.Join("；", forecast.Select(f => $"{f.date}:{f.desc}，{f.minT}~{f.maxT}°C，降雨概率{f.rain}%"));
                sb.AppendLine($"【未来预报】{forecastText}");
            }
            return sb.ToString();
        }
        catch
        {
            return null;
        }
    }
}
