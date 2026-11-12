// Adicionar ao script.js da Aula 3.
// No evento connect, assinar tambem:
// cliente.subscribe("lab/grupo01/telemetria");
function atualizarTelemetria(payload) {
  let dados;
  try {
    dados = JSON.parse(payload);
  } catch (erro) {
    console.warn("JSON invalido:", erro);
    return;
  }
  const t = dados.temperatura;
  const u = dados.umidade;
  if (!Number.isFinite(t) || !Number.isFinite(u)) return;
  if (u < 0 || u > 100) return;
  document.getElementById("temperatura").textContent =
    t.toFixed(1) + " °C";
  document.getElementById("umidade").textContent =
    u.toFixed(1) + " %";
}
// No evento message ja existente, adicionar:
// if (topico === "lab/grupo01/telemetria") {
//   atualizarTelemetria(mensagem);
// }
// No HTML, criar elementos com id="temperatura" e id="umidade".
