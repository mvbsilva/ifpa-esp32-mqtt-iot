// Carregar Chart.js ANTES deste arquivo no HTML:
// https://cdn.jsdelivr.net/npm/chart.js@4.4.8/dist/chart.umd.min.js
const MAX_AMOSTRAS = 20;
function criarGrafico(id, rotulo, cor) {
  return new Chart(document.getElementById(id), {
    type: "line",
    data: { labels: [], datasets: [{
      label: rotulo, data: [], borderColor: cor,
      tension: 0.25, pointRadius: 2
    }] },
    options: { responsive: true, maintainAspectRatio: false,
      animation: false }
  });
}
const graficoT = criarGrafico("graficoTemperatura", "°C", "#c2410c");
const graficoU = criarGrafico("graficoUmidade", "%", "#2563eb");
let ultimaLeitura = 0;
function adicionarPonto(grafico, horario, valor) {
  grafico.data.labels.push(horario);
  grafico.data.datasets[0].data.push(valor);
  if (grafico.data.labels.length > MAX_AMOSTRAS) {
    grafico.data.labels.shift();
    grafico.data.datasets[0].data.shift();
  }
  grafico.update("none");
}
function atualizarGraficos(temperatura, umidade) {
  if (!Number.isFinite(temperatura) ||
      !Number.isFinite(umidade)) return;
  const horario = new Date().toLocaleTimeString("pt-BR");
  adicionarPonto(graficoT, horario, temperatura);
  adicionarPonto(graficoU, horario, umidade);
  ultimaLeitura = Date.now();
}
function telemetriaRecente() {
  return ultimaLeitura > 0 &&
    Date.now() - ultimaLeitura <= 15000;
}
// No HTML: <canvas id="graficoTemperatura"></canvas>
//          <canvas id="graficoUmidade"></canvas>
// Ao receber telemetria valida, chamar atualizarGraficos(t, u).
