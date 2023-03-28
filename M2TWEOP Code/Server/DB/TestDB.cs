using EvolveDb;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Migrations;
using Microsoft.Extensions.Configuration;
using System;

namespace DB
{
#pragma warning disable IDE1006 // Naming Styles
    public class TestDB
    {
        public int id { get; set; }
        public string? text { get; set; }
        public int val { get; set; }
    }

    public class ApplicationContext : DbContext
    {
        public DbSet<TestDB> eoptesttable { get; set; } = null!;

        private readonly string ConnectionString;
        public ApplicationContext(string connectionString)
        {
            ConnectionString = connectionString;

            Database.EnsureCreated();
        }
        public void DoMigrations(string location)
        {
            var evolve = new Evolve(this.Database.GetDbConnection())
            {
                Locations = new[] { location },
                IsEraseDisabled = true,
                Placeholders = new Dictionary<string, string>
                {
                    ["${eoptesttable}"] = "eoptesttable"
                }
            };

            evolve.Migrate();
        }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseNpgsql(ConnectionString);
        }
    }
#pragma warning restore IDE1006 // Naming Styles
}

