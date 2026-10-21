# Carrinho Robótico com ESP32, Wi-Fi e Sensor Ultrassônico

## Integrantes

| Integrante | RM |
|---|---|
| Guilherme Daher | 98611 |
| Gabriel Freitas | 550187 |
| Heitor Nobre | 551539 |
| Vinicius Yamashita | 550908 |
| Lucca Alexandre | 99700 |

## Sobre o projeto

Carrinho robótico de duas rodas motorizadas, com controle pelo navegador e leitura de distância pelo sensor ultrassônico **HC-SR04**.

O firmware atual utiliza **ESP32**, uma ponte H para controlar dois motores DC e comunicação por **Wi-Fi**. A documentação inicial descrevia Arduino Uno e Bluetooth HC-05; esses componentes não são utilizados pelo firmware ESP32 apresentado nesta atualização.

O projeto também foi preparado para o CP2: coletar medições reais, gerar arquivos CSV, treinar um modelo no Edge Impulse e utilizar a classificação embarcada para alterar o comportamento do carrinho.

![Montagem do carrinho](docs/carrinho-robotico.png)

## Evolução do projeto

| Etapa | Características |
|---|---|
| Versão inicialmente documentada | Arduino Uno, Bluetooth HC-05 e ponte H |
| Código do CP1 fornecido | ESP32, controle Wi-Fi, HC-SR04 e bloqueio de avanço a até 20 cm |
| Evolução para o CP2 | Coleta em CSV, diagnóstico do sensor no Serial e código preparado para inferência TinyML |

Os arquivos e commits anteriores devem ser preservados para demonstrar a evolução CP1 → CP2.

## Componentes

| Componente | Quantidade | Função |
|---|---:|---|
| ESP32 | 1 | Controlador e ponto de acesso Wi-Fi |
| Motor DC com redução | 2 | Movimentação |
| Rodas | 2 | Tração |
| Ponte H de dois canais | 1 | Acionamento dos motores |
| HC-SR04 | 1 | Medição de distância frontal |
| Bateria e circuito de alimentação | Conforme montagem | Alimentação do conjunto |
| Chassi, fios e conectores | Conforme montagem | Estrutura e interligações |

**Identificação da ponte H:** o texto anterior cita DRV8833 e o código fornecido identifica L298N nos comentários. O modelo instalado deve ser confirmado na montagem. Os pinos abaixo representam os quatro sinais de controle usados no firmware.

## Ligações usadas no código

| Sinal | GPIO do ESP32 |
|---|---:|
| IN1 | 18 |
| IN2 | 5 |
| IN3 | 12 |
| IN4 | 14 |
| HC-SR04 TRIG | 27 |
| HC-SR04 ECHO | 26 |
| GND | Terra comum entre os circuitos |

A alimentação deve respeitar as especificações de cada módulo. O ECHO do HC-SR04 padrão de 5 V precisa de adaptação de nível para a entrada do ESP32. Os motores devem ser alimentados pelo circuito de potência da ponte H.

## Controle por Wi-Fi

O ESP32 cria a rede:

- **Rede:** `Carrinho-ESP32`
- **Senha:** `12345678`
- **Página:** `http://192.168.4.1`, ou o endereço exibido no Serial

O celular ou computador conecta-se a essa rede e acessa a página pelo navegador. Não é necessário um aplicativo Bluetooth.

| Comando | Rota HTTP |
|---|---|
| Frente | `/frente` |
| Ré | `/tras` |
| Esquerda | `/esquerda` |
| Direita | `/direita` |
| Parar | `/parar` |

Na versão CP2, o botão deve permanecer pressionado para autorizar o movimento. Soltar o botão envia a parada. A autorização expira após 700 ms sem renovação dos comandos.

## Integração do sensor ultrassônico

O firmware envia um pulso de aproximadamente 10 microssegundos ao TRIG e mede a duração do retorno no ECHO.

```cpp
distancia_cm = duracao_us * 0.0343 / 2.0;
```

Foram implementados:

- leitura periódica a cada 100 ms, equivalente a 10 Hz;
- espera máxima de 25 ms pelo pulso de retorno;
- validação da distância entre 2 e 300 cm;
- descarte de leituras inválidas;
- proteção frontal independente do modelo a até 20 cm;
- apresentação da distância na página web;
- diagnóstico do pulso ECHO no Monitor Serial.

### Monitoramento no Serial

Abra o Monitor Serial em **115200 baud**. No modo de coleta, cada tentativa de leitura informa o pulso recebido e a distância calculada.

Exemplo ilustrativo de uma leitura:

```text
SENSOR | ECHO=1749 us | distancia bruta=30.0 cm
```

Quando não é medido um pulso de retorno:

```text
SENSOR | ECHO=0 us | TIMEOUT: pulso nao medido
```

Uma leitura inválida durante a gravação aborta a coleta. O alvo, as conexões e a alimentação devem ser verificados antes de repetir a gravação.

## Organização do firmware CP2

| Arquivo | Responsabilidade |
|---|---|
| `carrinho_cp2.ino` | Inicialização, servidor, sensor, coleta e chamada do classificador |
| `Config.h` | Pinos, modo de execução, tempos e PWM |
| `Controle.h` | Escolha da ação dos motores a partir dos comandos, proteções e classe |
| `Pagina.h` | Interface do navegador |

Os quatro arquivos devem permanecer juntos em `CP2/firmware/carrinho_cp2/`.

## Modo de coleta e dataset gerado

Para coletar dados, mantenha em `Config.h`:

```cpp
#define USAR_TINYML 0
```

Nesse modo, os motores ficam desabilitados. Na página web, o usuário escolhe a classe, o conjunto (`treino` ou `teste`) e o identificador da sessão.

Cada gravação concluída gera **50 medições**, aproximadamente cinco segundos a 10 Hz. O download produz um CSV com **apenas duas colunas**:

```csv
timestamp,distancia_cm
```

- `timestamp`: tempo na grade nominal, em milissegundos, de 0 até 4900, com passo de 100;
- `distancia_cm`: leitura do sensor em centímetros, com ponto decimal.

A classe não é acrescentada como coluna pelo firmware: ela aparece no nome do arquivo. O padrão é:

```text
{classe}.{conjunto}_{sessao}_{id}_{sequencia}.csv
```

O rótulo da coleta é escolhido pelo grupo de acordo com a situação física. A classificação pelo modelo ocorre posteriormente, no modo TinyML.

**100 medições reais correspondem a duas gravações de 50 linhas.** É necessário repetir a coleta para representar todas as classes e reservar sessões diferentes para teste. Uma gravação com atraso de amostragem maior que 20 ms também é abortada.

Baixe cada arquivo antes da próxima gravação ou de reiniciar o ESP32. Os arquivos são mantidos temporariamente na memória da placa.

Dados simulados servem para demonstração do formato e do treinamento. O dataset da entrega deve ser coletado pelo próprio grupo com o sensor do carrinho.

## TinyML com Edge Impulse

O fluxo previsto para o CP2 é:

**Sensor → coleta real → CSV → Edge Impulse → treinamento → testes → biblioteca → ESP32 → inferência → ação.**

### Classes propostas

| Classe | Faixa de referência para coleta | Ação prevista ao solicitar frente |
|---|---|---|
| `perto` | 5 a 35 cm | Parar |
| `medio` | Acima de 35 até 80 cm | Avançar com PWM 150 |
| `longe` | Acima de 80 até 200 cm | Avançar com PWM 190 |

As faixas orientam a rotulagem do dataset. No modo TinyML, a classe deve vir da inferência do modelo, e não de comparações com esses limites no código de movimentação.

O firmware foi preparado para um eixo de entrada (`distancia_cm`), 10 Hz e uma janela de 1000 ms com 10 leituras. A janela é reavaliada a cada duas novas leituras, aproximadamente 200 ms.

A integração utiliza:

```cpp
ei::numpy::signal_from_buffer(entradas, cfg::LEITURAS_JANELA, &sinal);
run_classifier(&sinal, &resultado, false);
```

O avanço também depende de leitura válida, inferência recente e escore mínimo de 0,80. A proteção de 20 cm permanece independente do modelo. Ré e giros continuam sendo manobras manuais; o sensor observa a frente.

### Ativação após treinar

1. Importe os CSVs de treino e teste nos conjuntos correspondentes do Edge Impulse.
2. Configure o Impulse, gere as features, treine e avalie o modelo.
3. Exporte a biblioteca Arduino e instale o ZIP na Arduino IDE.
4. Substitua `SEU_PROJETO_inferencing.h` pelo cabeçalho real da biblioteca exportada.
5. Altere `USAR_TINYML` para `1` e grave novamente o ESP32.
6. Verifique a inferência no Serial e sua ação física nos motores.

Para a configuração proposta, utilize Raw data com escala `0.01`. O firmware fornece centímetros brutos; a biblioteca exportada aplica o processamento configurado.

## Como executar a coleta

1. Abra `CP2/firmware/carrinho_cp2/carrinho_cp2.ino` na Arduino IDE.
2. Confira os três arquivos `.h` na mesma pasta e `USAR_TINYML=0`.
3. Selecione a placa ESP32 correspondente à montagem e a porta USB.
4. Envie o código e abra o Monitor Serial em 115200 baud.
5. Se necessário, pressione EN/RESET para ver a inicialização.
6. Conecte o celular à rede `Carrinho-ESP32` e abra o IP exibido.
7. Verifique se a distância muda corretamente ao mover um objeto à frente do sensor.
8. Escolha classe, conjunto e sessão. Clique em **Gravar 5 segundos**.
9. Após a conclusão, clique em **Baixar CSV**.
10. Organize os arquivos em `CP2/dados/treino/` e `CP2/dados/teste/`.

O guia detalhado está em [CP2/README.md](CP2/README.md).

## Estado atual e entrega

| Item | Situação |
|---|---|
| Inicialização do ESP32 e servidor Wi-Fi | Confirmada durante o acompanhamento |
| Leitura, validação e diagnóstico do HC-SR04 | Implementados; obtenção de leituras válidas ainda em verificação |
| Geração e download de CSV | Implementados; dataset real ainda deve ser coletado |
| Rotina de inferência e ações por classe | Preparadas no código, dependentes da biblioteca treinada |
| Treinamento, métricas e matriz de confusão | Pendentes |
| Biblioteca real embarcada e teste físico TinyML | Pendentes |
| Vídeo da demonstração final | Pendente |

Para concluir a entrega:

- [ ] Coletar e publicar os CSVs reais, com contagens e protocolo.
- [ ] Separar treino e teste por gravações e sessões.
- [ ] Documentar configuração, treinamento, métricas e testes no Edge Impulse.
- [ ] Adicionar prints reais do processo.
- [ ] Exportar e integrar a biblioteca treinada no ESP32.
- [ ] Demonstrar parada e alteração de velocidade causadas pela classe inferida.
- [ ] Gravar e disponibilizar o vídeo final.
- [ ] Preservar o CP1 e registrar commits reais da evolução para o CP2.

Uma demonstração da ação do modelo deve mostrar, por exemplo, a parada por `perto` a aproximadamente 30 cm, acima do limite fixo de emergência de 20 cm.

## Uso do projeto

Projeto desenvolvido para fins acadêmicos, educacionais e de prototipagem.
