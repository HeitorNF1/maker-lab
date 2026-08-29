# Planejamento Ágil - Projeto Robô 2WD

## 1. MVP e Conclusão do Projeto
**Produto Mínimo Viável (MVP):** O projeto estará minimamente funcional quando o chassi base estiver montado (motores, ponte H, Arduino e módulo Bluetooth) e o robô for capaz de se movimentar pela pista recebendo comandos de direção básicos (frente, trás, esquerda, direita) via smartphone, alimentado de forma independente pela bateria 18650.

**Conclusão do Projeto:** O projeto será considerado concluído quando a integração total for alcançada: o sensor ultrassônico atuando ativamente como sistema anti-colisão, o sistema de energia estabilizado (com o regulador step-up e capacitores devidamente soldados), e todo o cabeamento organizado e fixado de forma definitiva e segura no chassi.

## 2. Matriz MoSCoW
| Prioridade | Funcionalidades do Robô 2WD |
| :--- | :--- |
| **Must Have** (Deve ter) | Movimentação mecânica, Controle remoto via Bluetooth (HC-05), Alimentação independente por bateria 18650. |
| **Should Have** (Deveria ter) | Sistema anti-colisão (HC-SR04), Estabilização de tensão da lógica (Regulador Boost 5V), Chave liga/desliga. |
| **Could Have** (Poderia ter) | LEDs indicadores de direção/status (faróis/setas) e um Buzzer para emissão de alertas sonoros. |
| **Won't Have** (Não terá agora) | Modos autônomos complexos (como seguidor de linha IR), Câmera FPV, Conectividade Wi-Fi/Internet. |

## 3. Backlog e Responsáveis
| ID | Descrição da Tarefa | Responsável |
| :--- | :--- | :--- |
| **T1** | Montagem mecânica (fixação dos motores TT, rodízio e chassi de acrílico/MDF). | Guilherme Daher |
| **T2** | Circuito de potência (soldagem/ligação da bateria, step-up, chave gangorra e capacitores). | Gabriel Freitas |
| **T3** | Circuito lógico (ligação do Arduino com Ponte H, pinos RX/TX do Bluetooth e Ultrassônico). | Guilherme Daher |
| **T4** | Desenvolvimento e upload do código de controle (Bluetooth + lógica anti-colisão). | Heitor Nobre |
| **T5** | Modelagem (OpenSCAD/CAD) e impressão 3D de proteções para os módulos. | Heitor Nobre |
| **T6** | Testes integrados de pista e calibração de sensibilidade do HC-SR04. | Gabriel Freitas |

## 4. Dependências
* A **Tarefa T3 (Circuito lógico)** só pode ser finalizada após a **T2 (Circuito de potência)** garantir que as tensões estão corretas para evitar danos ao Arduino.
* A **Tarefa T6 (Testes integrados)** é estritamente dependente da conclusão da **T4 (Código)** e da **T1 (Montagem mecânica)**.

## 5. Fluxo Kanban (Status Inicial)
| A Fazer (To Do) | Em Progresso (Doing) | Em Teste (Testing) | Concluído (Done) |
| :--- | :--- | :--- | :--- |
| **T3** Circuito lógico<br>**T4** Código v0.2<br>**T5** Modelagem 3D<br>**T6** Testes de pista | **T1** Montagem mecânica<br>**T2** Circuito de potência | *(Nenhuma tarefa)* | **Doc** Planilha de Custos<br>**Doc** Diagrama Elétrico |
