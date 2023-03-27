using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using System;

namespace M2TWEOPServer
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            IHost host = Host.CreateDefaultBuilder(args)
    .ConfigureServices(services =>
    {
        services.AddHostedService<Worker>();
    })
    .Build();

            try
            {
                await host.RunAsync();
            }
            catch (Exception ex)
            {

            }

        }
    }

    public class Worker : BackgroundService
    {
        private readonly ILogger<Worker> _logger;

        public Worker(ILogger<Worker> logger)
        {
            _logger = logger;
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            _logger.LogInformation("");

            stoppingToken.Register(() => _logger.LogDebug(""));

            while (!stoppingToken.IsCancellationRequested)
            {
                _logger.LogInformation("");


                await Task.Delay(5000, stoppingToken);
            }
        }
    }
}

