// =================================================================
// PROJETO ROBÔ 2WD - VERSÃO 0.2
// Integração: Motores (DRV8833), Bluetooth (HC-05) e HC-SR04
// =================================================================

#include <SoftwareSerial.h>

// --- Definições de Pinos: Ponte H DRV8833 ---
#define IN1_ESQ 5
#define IN2_ESQ 6
#define IN1_DIR 9
#define IN2_DIR 10

// --- Definições de Pinos: Bluetooth HC-05 ---
// TX do HC-05 vai no 2 (RX do Arduino)
// RX do HC-05 vai no 3 (TX do Arduino)
SoftwareSerial BT(2, 3);

// --- Definições de Pinos: Sensor Ultrassônico HC-SR04 ---
#define TRIG_PIN 7
#define ECHO_PIN 8

// --- Variáveis Globais ---
char comandoBT;
long duracao;
int distancia;
const int DISTANCIA_SEGURA = 20; // cm

void setup()
{
    Serial.begin(9600);
    BT.begin(9600);

    // Configuração dos Pinos dos Motores
    pinMode(IN1_ESQ, OUTPUT);
    pinMode(IN2_ESQ, OUTPUT);
    pinMode(IN1_DIR, OUTPUT);
    pinMode(IN2_DIR, OUTPUT);

    // Configuração dos Pinos do Sensor Ultrassônico
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    pararMotores();
}

void loop()
{
    // 1. Leitura do Sensor Ultrassônico
    distancia = lerDistancia();

    // 2. Sistema Anti-Colisão (Override)
    if (distancia > 0 && distancia < DISTANCIA_SEGURA)
    {
        pararMotores();
        if (BT.available() > 0)
        {
            char tentaComando = BT.read();
            // Permite apenas andar para trás se estiver muito perto da parede
            if (tentaComando == 'B')
            {
                moverTras();
            }
        }
    }
    // 3. Controle Normal via Bluetooth
    else
    {
        if (BT.available() > 0)
        {
            comandoBT = BT.read();

            switch (comandoBT)
            {
            case 'F':
                moverFrente();
                break;
            case 'B':
                moverTras();
                break;
            case 'L':
                virarEsquerda();
                break;
            case 'R':
                virarDireita();
                break;
            case 'S':
                pararMotores();
                break;
            }
        }
    }
    delay(50);
}

// =================================================================
// FUNÇÕES DE CONTROLE DE MOVIMENTO E SENSORES
// =================================================================

int lerDistancia()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duracao = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout para evitar travamentos
    if (duracao == 0)
        return 999;
    return duracao * 0.034 / 2;
}

void moverFrente()
{
    digitalWrite(IN1_ESQ, HIGH);
    digitalWrite(IN2_ESQ, LOW);
    digitalWrite(IN1_DIR, HIGH);
    digitalWrite(IN2_DIR, LOW);
}

void moverTras()
{
    digitalWrite(IN1_ESQ, LOW);
    digitalWrite(IN2_ESQ, HIGH);
    digitalWrite(IN1_DIR, LOW);
    digitalWrite(IN2_DIR, HIGH);
}

void virarEsquerda()
{
    digitalWrite(IN1_ESQ, LOW);
    digitalWrite(IN2_ESQ, HIGH);
    digitalWrite(IN1_DIR, HIGH);
    digitalWrite(IN2_DIR, LOW);
}

void virarDireita()
{
    digitalWrite(IN1_ESQ, HIGH);
    digitalWrite(IN2_ESQ, LOW);
    digitalWrite(IN1_DIR, LOW);
    digitalWrite(IN2_DIR, HIGH);
}

void pararMotores()
{
    digitalWrite(IN1_ESQ, LOW);
    digitalWrite(IN2_ESQ, LOW);
    digitalWrite(IN1_DIR, LOW);
    digitalWrite(IN2_DIR, LOW);
}