# Montagem e segurança elétrica

## Componentes

ESP32 DevKit, 2 LEDs, resistores de 330 Ω, DHT22, protoboard, jumpers e cabo USB de dados. Para DHT22 sem pull-up interno, adicione resistor de 4,7–10 kΩ entre DATA e 3V3.

## Ligações

| Origem | Destino |
|---|---|
| GPIO23 | resistor 330 Ω → ânodo LED1; cátodo → GND |
| GPIO22 | resistor 330 Ω → ânodo LED2; cátodo → GND |
| 3V3 | VCC DHT22 |
| GND | GND DHT22 e cátodos LEDs |
| GPIO4 | DATA DHT22 |

> **Atenção:** desligue a alimentação antes de alterar conexões. Não conecte o GPIO diretamente a 5 V. Confira a pinagem de módulos DHT22 (ordem dos terminais pode variar). A montagem utiliza apenas LEDs de baixa potência.

## Diagramas por aula

- [Aula 1](../assets/diagramas/diagrama-aula-01.png)
- [Aula 2](../assets/diagramas/diagrama-aula-02.png)
- [Aula 3](../assets/diagramas/diagrama-aula-03.png)
- [Aula 4](../assets/diagramas/diagrama-aula-04.png)
- [Aula 5](../assets/diagramas/diagrama-aula-05.png)
- [Aula 6](../assets/diagramas/diagrama-aula-06.png)

As imagens são auxiliares pedagógicos: a tabela de conexões acima é a referência de pinagem para a montagem.
