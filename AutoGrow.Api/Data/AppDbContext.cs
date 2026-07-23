using Microsoft.EntityFrameworkCore;
using AutoGrow.Api.Models;

namespace AutoGrow.Api.Data;

public class AppDbContext : DbContext
{
    public AppDbContext(DbContextOptions<AppDbContext> options) : base(options) { }

    public DbSet<Sensor> Sensor { get; set; }
}