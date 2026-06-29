# Projeto 2 - Entrega Final
Entrega Final do Projeto 2 - Aferidor de Temperatura de Forno Industrial da disciplina SEL0433 - Aplicação de Microprocessadores

ENTREGA FINAL - PROJETO 2: AFERIDOR DE TEMPERATURA DE FORNO INDUSTRIAL 

Alexsandra Pavani Xavier N°USP 14681372

1. Introdução
O projeto 2 tem o objetivo de desenvolver um dispositivo de aferição de temperatura e tempo para um forno industrial. O projeto contou com o desenvolvimento do firmware e do esquemático eletrônico, com o intuito de monitorar continuamente a temperatura interna do forno e exibir uma contagem regressiva baseada no tempo previamente escolhido pelo operador. O núcleo do sistema baseia-se no microcontrolador PIC18F4550, programado em linguagem C utilizando a IDE MikroC PRO for PIC. A validação da simulação foi realizada no simulador SimulIDE, respeitando as conexões elétricas e o pinout do Kit EasyPIC v7.


2. Requisitos e Arquitetura do Sistema
O sistema foi projetado para atender aos seguintes critérios de funcionamento:
a) Medição de Temperatura: Leitura analógica realizada por meio de um sensor LM35 (emulado via potenciômetro) conectado ao canal analógico `AN0` (pino RA0). A faixa de operação é de 0°C a 100°C.
b) Referência de Tensão Externa: Utilização de uma tensão externa de referência de 1V conectada ao pino `Vref+` (RA3), otimizando a resolução de 10 bits do conversor A/D para a sensibilidade do LM35.
c) Seleção de Tempo: Um botão realiza a escolha entre uma aferição de curta duração (10 segundos) ou longa duração (60 segundos).
d) Controle de Processo: Um segundo botão é responsável por iniciar a contagem regressiva e habilitar a atualização da leitura de temperatura.
e) Interface Visual: Display LCD 16x2 opera em modo de comunicação paralela de 4 bits (PORTB), exibindo continuamente a temperatura formatada no padrão "XX.X °C" e o tempo restante.
f) Atuador (Resistência - LED): Um LED conectado ao pino RD0 simula o acionamento da resistência do forno, acendendo automaticamente quando a temperatura ultrapassa os 50°C.


3. Implementação do Firmware
O código foi estruturado de forma eficiente para evitar o consumo excessivo de memória de dados, tratando as variáveis de temperatura de forma inteira e escalada.

A temporização do sistema utiliza os periféricos internos do PIC:
a) Timer0: Configurado com interrupção para gerar a base de tempo exata de 1 segundo necessária para a contagem regressiva de longa duração.
b) Timer1: Configurado com interrupção para gerar os intervalos da contagem de curta duração.
c) Tratamento de Bouncing: Os acionamentos dos botões foram tratados via software por meio de flags auxiliares e detecção por borda de subida, mitigando leituras falsas causadas pelo efeito bouncing mecânico.

A inicialização do módulo ADC foi programada de forma que o registrador ADCON1 seja configurado somente após as funções de inicialização da biblioteca nativa do MikroC, garantindo que os bits de configuração de Vref externa operem corretamente na simulação.


4. Resultados 
O firmware  obteve sucesso na compilação na IDE MikroC PRO for PIC, gerando o arquivo executável '.hex' sem erros para a simulação. Na simulação realizada no SimulIDE, o circuito respondeu perfeitamente aos estímulos:
a) Em modo de espera, o display exibe as configurações selecionadas.
b) Ao acionar o botão de início, a contagem regressiva decresce de forma precisa de acordo com o timer selecionado.
c) Ao variar o potenciômetro de entrada, o LCD atualiza em tempo real a temperatura calculada.
d) Quando o valor lido ultrapassa os 50.0°C, o LED indicador conectado em RD0 acende instantaneamente, demonstrando o controle correto da lógica da resistência.


5. Conclusão
O projeto atingiu todos os objetivos propostos na entrega final. A integração entre os módulos de conversão analógico-digital (ADC), temporizadores por interrupção (Timer0/Timer1) e controle de periféricos externos (LCD e LED) permitiu consolidar a compreensão prática sobre a arquitetura RISC e a robustez da família PIC18 em aplicações embarcadas industriais.
O projeto atingiu todos os objetivos propostos na entrega final. A integração entre os módulos de conversão analógico-digital (ADC), temporizadores por interrupção (Timer0/Timer1) e controle de periféricos externos (LCD e LED) permitiu consolidar a compreensão prática sobre a arquitetura RISC e a robustez da família PIC18 em aplicações embarcadas industriais.
