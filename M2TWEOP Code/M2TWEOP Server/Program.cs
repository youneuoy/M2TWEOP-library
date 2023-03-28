using Serilog;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Configuration;
namespace M2TWEOPServer
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            Log.Logger = new LoggerConfiguration()
            .MinimumLevel.Debug()
            .WriteTo.File("log/EopServer.log", fileSizeLimitBytes: 1 * 1024 * 1024, rollingInterval: RollingInterval.Day)
            .CreateLogger();

            var host = new HostBuilder()
            .ConfigureAppConfiguration((hostContext, config) =>
            {
                config.AddJsonFile("appsettings.json", optional: false);
            })
            .ConfigureLogging((hostContext, logging) =>
            {
                logging.AddSerilog(dispose: true);
            })
            .ConfigureServices((hostContext, services) =>
            {
                services.AddHostedService<EOPService>();
            }).Build();
            await host.RunAsync();
        }
    }
}

