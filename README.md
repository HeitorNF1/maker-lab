# Integrantes

Guilherme Daher - RM98611
Gabriel Freitas - RM550187
Heitor Nobre - RM551539
Vinicius Yamashita - RM550908
Lucca Alexandre - RM99700

# 🚗 Carrinho Robótico com Arduino, Bluetooth e Sensor Ultrassônico

Projeto de um **carrinho robótico de duas rodas motorizadas**, controlado por **Arduino Uno**, com comunicação via **Bluetooth HC-05** e detecção de obstáculos utilizando o sensor ultrassônico **HC-SR04**.

O acionamento dos motores é realizado por um driver **DRV8833**, enquanto a alimentação utiliza uma bateria **18650** e um conversor **Step-Up de 5 V**.

![Montagem do carrinho](docs/carrinho-robotico.png)

---

## 📌 Objetivo do projeto

Desenvolver uma plataforma robótica móvel capaz de:

- movimentar-se para frente e para trás;
- realizar curvas para esquerda e direita;
- receber comandos por Bluetooth;
- detectar obstáculos à frente;
- controlar dois motores DC de maneira independente;
- funcionar de forma portátil utilizando bateria recarregável.

---

## 🧰 Componentes

| Componente | Quantidade | Função |
|---|---:|---|
| Arduino Uno | 1 | Controlador principal |
| Motor DC com caixa de redução | 2 | Movimentação do carrinho |
| Rodas | 2 | Tração |
| Driver DRV8833 | 1 | Controle dos motores |
| Módulo Bluetooth HC-05 | 1 | Comunicação sem fio |
| Sensor ultrassônico HC-SR04 | 1 | Detecção de obstáculos |
| Bateria 18650 | 1 | Alimentação |
| Suporte para bateria 18650 | 1 | Fixação e conexão da bateria |
| Conversor Step-Up 5 V | 1 | Regulagem da alimentação |
| Chave liga/desliga | 1 | Controle geral de energia |
| Chassi | 1 | Estrutura do robô |
| Jumpers, fios e conectores | — | Interligações elétricas |

---

## 🧠 Arquitetura do sistema

```text
                +-------------------+
                |   Bateria 18650   |
                +---------+---------+
                          |
                    Liga / Desliga
                          |
                +---------v---------+
                |    Step-Up 5 V    |
                +---------+---------+
                          |
          +---------------+----------------+
          |               |                |
          v               v                v
     Arduino Uno        HC-05          HC-SR04
          |
          v
       DRV8833
       /     \
      v       v
Motor Esq.  Motor Dir.
```

O **Arduino Uno** recebe os comandos enviados pelo celular através do **HC-05** e controla o **DRV8833**, responsável por definir o sentido de rotação e a velocidade dos motores.

O **HC-SR04** pode ser utilizado para interromper ou alterar o movimento do robô ao detectar um obstáculo.

---

## 🔌 Alimentação

A alimentação parte da bateria **18650** e passa pela chave liga/desliga.

O conversor **Step-Up** deve ser ajustado para fornecer aproximadamente **5,0 V** para os módulos que necessitam dessa tensão.

> [!IMPORTANT]
> Confira a tensão na saída do Step-Up com um multímetro **antes de conectar o Arduino**.

### Sugestão de distribuição

```text
Bateria 18650
     |
     +--> Chave liga/desliga
              |
              +--> Step-Up 5 V
                       |
                       +--> Arduino 5V
                       +--> HC-05
                       +--> HC-SR04

Bateria / alimentação dos motores
     |
     +--> DRV8833
              |
              +--> Motor esquerdo
              +--> Motor direito
```

Todos os módulos devem compartilhar o mesmo **GND**.

> [!NOTE]
> A alimentação dos motores deve respeitar a tensão nominal dos motores utilizados. O DRV8833 permite controlar os motores separadamente e suporta PWM para controle de velocidade.

---

## 🔗 Ligações principais

Os pinos podem ser definidos de acordo com o código utilizado.

### DRV8833

| DRV8833 | Arduino |
|---|---|
| AIN1 | Pino digital/PWM |
| AIN2 | Pino digital/PWM |
| BIN1 | Pino digital/PWM |
| BIN2 | Pino digital/PWM |
| GND | GND |
| VM | Alimentação dos motores |
| AOUT1 / AOUT2 | Motor esquerdo |
| BOUT1 / BOUT2 | Motor direito |

### HC-SR04

| HC-SR04 | Arduino |
|---|---|
| VCC | 5 V |
| GND | GND |
| TRIG | Pino digital |
| ECHO | Pino digital |

### HC-05

| HC-05 | Arduino |
|---|---|
| VCC | 5 V* |
| GND | GND |
| TXD | RX do Arduino |
| RXD | TX do Arduino através de divisor de tensão |

\* Em módulos HC-05 montados em placa breakout, o VCC normalmente aceita 5 V. O pino RX do módulo trabalha com nível lógico de aproximadamente 3,3 V, por isso é recomendável utilizar um divisor resistivo na linha Arduino TX → HC-05 RX.

---

## 🎮 Comandos Bluetooth

Uma implementação simples pode utilizar caracteres enviados por um aplicativo Bluetooth:

| Comando | Movimento |
|---|---|
| `F` | Frente |
| `B` | Ré |
| `L` | Esquerda |
| `R` | Direita |
| `S` | Parar |

Exemplo:

```text
Celular
   |
Bluetooth
   |
  HC-05
   |
Arduino
   |
DRV8833
   |
Motores
```

---

## 🚦 Lógica de movimentação

### Frente

```text
Motor esquerdo  -> frente
Motor direito   -> frente
```

### Ré

```text
Motor esquerdo  -> trás
Motor direito   -> trás
```

### Esquerda

```text
Motor esquerdo  -> reduz/parado/trás
Motor direito   -> frente
```

### Direita

```text
Motor esquerdo  -> frente
Motor direito   -> reduz/parado/trás
```

### Parar

```text
Motor esquerdo  -> parado
Motor direito   -> parado
```

---

## 📡 Detecção de obstáculos

O HC-SR04 calcula a distância utilizando o tempo de retorno de uma onda ultrassônica.

A distância aproximada pode ser calculada por:

```cpp
distancia_cm = duracao_us * 0.0343 / 2;
```

Uma lógica possível é:

```text
Se distância > limite:
    permitir movimento

Se distância <= limite:
    parar motores
    bloquear avanço
```

Por exemplo, pode-se definir um limite de segurança de **15 a 25 cm**, dependendo da velocidade do carrinho e do ambiente.

---

## 💻 Estrutura sugerida do repositório

```text
carrinho-robotico/
│
├── README.md
├── src/
│   └── carrinho_robotico.ino
│
├── docs/
│   ├── carrinho-robotico.png
│   ├── diagrama-eletrico.png
│   └── esquema-conexoes.md
│
└── LICENSE
```

---

## ▶️ Como executar

1. Monte os componentes no chassi.
2. Faça as conexões elétricas.
3. Ajuste o Step-Up para **5 V**.
4. Conecte o Arduino ao computador via USB.
5. Abra a **Arduino IDE**.
6. Selecione a placa **Arduino Uno**.
7. Selecione a porta serial correspondente.
8. Carregue o arquivo `.ino`.
9. Desconecte o USB.
10. Ligue o carrinho pela chave.
11. Pareie o celular com o módulo HC-05.
12. Abra um aplicativo de terminal Bluetooth.
13. Envie os comandos de movimentação.

---

## 📱 Pareamento do HC-05

Normalmente o módulo aparece no celular como:

```text
HC-05
```

Senhas de pareamento comuns:

```text
1234
```

ou

```text
0000
```

Após o pareamento, utilize um aplicativo capaz de enviar caracteres via Bluetooth Serial.

---

## ⚠️ Cuidados

- Não conecte a bateria com polaridade invertida.
- Verifique a tensão do Step-Up antes de alimentar o Arduino.
- Nunca provoque curto-circuito em uma célula 18650.
- Utilize uma bateria em boas condições.
- Garanta que todos os módulos compartilhem o mesmo GND.
- Evite alimentar os motores diretamente pelo pino 5 V do Arduino.
- Se o Arduino reiniciar quando os motores arrancarem, revise a alimentação e considere filtragem/desacoplamento adicional.
- Fixe os fios para que eles não encostem nas rodas.

---

## 🔮 Melhorias futuras

- [ ] Controle de velocidade por PWM
- [ ] Modo autônomo para desvio de obstáculos
- [ ] Aplicativo próprio para controle do carrinho
- [ ] Indicador de nível da bateria
- [ ] Controle proporcional de velocidade
- [ ] Sensores adicionais
- [ ] Servo motor para movimentar o HC-SR04
- [ ] Telemetria em tempo real
- [ ] Integração com ESP32 ou Wi-Fi
- [ ] Mapeamento de ambiente

---

## 🖼️ Montagem

A organização proposta utiliza:

- bateria na lateral esquerda;
- Arduino Uno na região central;
- DRV8833 próximo aos motores;
- HC-05 na lateral;
- HC-SR04 na parte frontal;
- Step-Up próximo ao circuito de alimentação;
- chave liga/desliga em posição de fácil acesso.

Essa disposição ajuda a reduzir o comprimento dos cabos e facilita a manutenção do projeto.

---

## 📄 Licença

Este projeto pode ser utilizado para fins acadêmicos, educacionais e de prototipagem.

---

## 👨‍💻 Autores

Projeto desenvolvido como trabalho acadêmico de robótica e sistemas embarcados.
