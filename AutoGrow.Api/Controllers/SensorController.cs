using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using AutoGrow.Api.Data;
using AutoGrow.Api.Models;
using TimeZoneConverter;

namespace AutoGrow.Api.Controllers;

[ApiController]
[Route("api/[controller]")]
public class SensorController : ControllerBase
{
    private readonly AppDbContext _context;
    private readonly ILogger<SensorController> _logger;
    private static readonly TimeZoneInfo BrasiliaTz = TZConvert.GetTimeZoneInfo("America/Sao_Paulo");

    public SensorController(AppDbContext context, ILogger<SensorController> logger)
    {
        _context = context;
        _logger = logger;
    }

    [HttpPost]
    public async Task<IActionResult> Receber([FromBody] Sensor dados)
    {
        if (dados == null)
            return BadRequest("JSON inválido.");
        
        dados.DataHora = DateTime.UtcNow;

        _context.Sensor.Add(dados);
        await _context.SaveChangesAsync();

        _logger.LogInformation("Salvo: {Nome} - {Temp}°C / {Umid}%",
            dados.Nome, dados.Temp, dados.Umid);
        
        return Ok(dados);
    }

    [HttpGet]
    public async Task<IActionResult> Listar()
    {
        var registros = await _context.Sensor
            .OrderByDescending(s => s.DataHora)
            .Take(50)
            .ToListAsync(); 

        var resultado = registros.Select(s => new Sensor
        {
            Id = s.Id,
            Nome = s.Nome,
            Temp = s.Temp,
            Umid = s.Umid,
            DataHora = TimeZoneInfo.ConvertTimeFromUtc(
                DateTime.SpecifyKind(s.DataHora, DateTimeKind.Utc),
                BrasiliaTz)
        }).ToList(); 

        return Ok(resultado);
    }
}