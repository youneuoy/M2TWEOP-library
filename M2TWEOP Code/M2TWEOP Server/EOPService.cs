using Microsoft.Extensions.Hosting;
using DB;
using Microsoft.Extensions.Configuration;
namespace M2TWEOPServer
{
    internal class EOPService : BackgroundService
    {
        private readonly IConfiguration Configuration;
        private MessagesHandler MessagesHandler = new MessagesHandler();
        public EOPService(IConfiguration configuration)
        {
            Configuration = configuration;
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            Serilog.Log.Information("Service started");
            try
            {
                using (ApplicationContext db = new ApplicationContext(Configuration.GetConnectionString("eopdb") ?? ""))
                {
                    db.DoMigrations("db/migrations");

                    TestDB v1 = new TestDB { text = "TestT1", val = 33 };
                    TestDB v2 = new TestDB { text = "TestT2", val = 26 };

                    db.eoptesttable.AddRange(v1, v2);
                    db.SaveChanges();
                }
            }
            catch (Exception ex)
            {
                Serilog.Log.Error(ex.ToString());
            }

            MessagesHandler.Run();
            while (!stoppingToken.IsCancellationRequested)
            {
                Tick();

                await Task.Delay(5000, stoppingToken);
            }
            Serilog.Log.Information("Service stopped");
        }

        private void Tick()
        {

        }
    }
}
