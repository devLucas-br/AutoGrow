namespace AutoGrow.Api.Models;

public class Sensor
{
    public int Id { get; set; }
    public string? Nome { get; set; } // Nome do sensor (DHT22)
    public double Temp { get; set; }
    public double Umid { get; set; }
    public DateTime DataHora { get; set; } = DateTime.UtcNow;
}