using Microsoft.EntityFrameworkCore;
using WaterDispenserWeb.Data;
using WaterDispenserWeb.Services;
using WaterDispenserWeb.Controllers;

var builder = WebApplication.CreateBuilder(args);

// 配置数据库连接
var connectionString = builder.Configuration.GetConnectionString("DefaultConnection");
builder.Services.AddDbContext<AppDbContext>(options =>
    options.UseMySql(connectionString, ServerVersion.AutoDetect(connectionString))
);

// 注册服务
builder.Services.AddSingleton<WaterAdviceService>();
builder.Services.AddSingleton<BillingService>();
builder.Services.AddHttpClient<DeepSeekService>();
builder.Services.AddHttpClient<PushPlusService>(client =>
    client.Timeout = TimeSpan.FromSeconds(15));
builder.Services.AddHttpClient<WeatherService>();

// 注册授权过滤器
builder.Services.AddScoped<AdminAuthFilter>();
builder.Services.AddScoped<WorkerAuthFilter>();
builder.Services.AddScoped<DrinkerAuthFilter>();
builder.Services.AddScoped<BillingAdminAuthFilter>();
builder.Services.AddScoped<BillingUserAuthFilter>();
builder.Services.AddScoped<FamilyAuthFilter>();

// 添加 Session 支持
builder.Services.AddDistributedMemoryCache();
builder.Services.AddSession(options =>
{
    options.IdleTimeout = TimeSpan.FromHours(8);
    options.Cookie.HttpOnly = true;
    options.Cookie.IsEssential = true;
});

builder.Services.AddHttpContextAccessor();
builder.Services.AddControllersWithViews();
builder.Services.AddHostedService<ReminderService>();

var app = builder.Build();

// 配置 HTTP 管道
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Home/Error");
}
app.UseStaticFiles();
app.UseRouting();
app.UseSession();
app.UseAuthorization();

app.MapControllerRoute(
    name: "default",
    pattern: "{controller=Home}/{action=Index}/{id?}");

// 启动时自动建库建表（开发环境）
using (var scope = app.Services.CreateScope())
{
    var db = scope.ServiceProvider.GetRequiredService<AppDbContext>();
    try
    {
        db.Database.EnsureCreated();
        Console.WriteLine("数据库连接成功，表结构已就绪。");
    }
    catch (Exception ex)
    {
        Console.WriteLine($"数据库初始化警告: {ex.Message}");
        Console.WriteLine("请确保 MySQL 已启动，然后手动执行 Sql/schema.sql 建库建表。");
    }
}

app.Run();
