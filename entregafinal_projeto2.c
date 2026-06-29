//ENTREGA FINAL - PROJETO 2
//ALEXSANDRA PAVANI XAVIER NUSP 14681372


//Configuração de Hardware e variáveis
//Definição das conexões do LCD
sbit LCD_RS at RB2_bit;
sbit LCD_EN at RB3_bit;
sbit LCD_D4 at RB4_bit;
sbit LCD_D5 at RB5_bit;
sbit LCD_D6 at RB6_bit;
sbit LCD_D7 at RB7_bit;

sbit LCD_RS_Direction at TRISB2_bit;
sbit LCD_EN_Direction at TRISB3_bit;
sbit LCD_D4_Direction at TRISB4_bit;
sbit LCD_D5_Direction at TRISB5_bit;
sbit LCD_D6_Direction at TRISB6_bit;
sbit LCD_D7_Direction at TRISB7_bit;

//Definições
#define LED_RESISTENCIA LATD.F0

//Variáveis globais
volatile unsigned int tempo_restante = 0;
volatile unsigned char t0_counter = 0;
volatile unsigned char t1_counter = 0;
volatile unsigned char em_execucao = 0;

//Estado da seleção de tempo: 0 = Longo (60s) | 1 = Curto (10s)
volatile unsigned char modo_selecionado = 0;

//Funções
void configurar_ADC();
void configurar_interrupcoes_externas();
void configurar_Timer0();
void configurar_Timer1();
void exibir_temperatura(unsigned int adc_val);
void exibir_tempo(unsigned int tempo);

//Inicialização e Loop principal

void main() {
    //Configurações de entrada e saída
    TRISD = 0x00;       // PORTD como saída (LED)
    LATD  = 0x00;       // garante LED da resistência desligado ao iniciar
    TRISB = 0x03;       // RB0 (INT0) e RB1 (INT1) como entradas para os botões

    //Executa a configuração do ADC
    configurar_ADC();

    //Inicialização e limpeza do display LCD
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);

    //Inicializa periféricos externos
    configurar_interrupcoes_externas();
    configurar_Timer0();
    configurar_Timer1();

    //Habilita as chaves gerais de interrupção
    PEIE_bit = 1;       //habilita interrupções dos timers
    GIE_bit = 1;        //habilita a chave geral de interrupções

    // Loop Principal
    while(1) {
        if (em_execucao) {
            unsigned int leitura = ADC_Get_Sample(0); //canal AN0 do potenciômetro

            //chamada das funções gráficas
            exibir_temperatura(leitura);
            exibir_tempo(tempo_restante);

            Delay_ms(200); //delay de estabilização do LCD
        } else {
            //modo de espera ativa
            LED_RESISTENCIA = 0;
            Lcd_Out(1, 1, "Modo Selecionado");
            if (modo_selecionado == 0) {
                Lcd_Out(2, 1, "Longo: 60s (RB1) ");
            } else {
                Lcd_Out(2, 1, "Curto: 10s (RB1) ");
            }
        }
    }
}

//Funções do LCD

void exibir_temperatura(unsigned int adc_val) {
    unsigned long temp_scaled;
    unsigned char dezenas, unidades, decimais;
    char txt_temp[11];

    temp_scaled = ((unsigned long)adc_val * 1000) / 1023;

    if (temp_scaled >= 1000) {
        Lcd_Out(1, 1, "Temp: 100.0 °C  ");
    } else {
        dezenas  = (temp_scaled / 100) % 10;
        unidades = (temp_scaled / 10) % 10;
        decimais = temp_scaled % 10;

        txt_temp[0] = dezenas + '0';
        txt_temp[1] = unidades + '0';
        txt_temp[2] = '.';
        txt_temp[3] = decimais + '0';
        txt_temp[4] = ' ';
        txt_temp[5] = 0xDF;
        txt_temp[6] = 'C';
        txt_temp[7] = ' ';
        txt_temp[8] = ' ';
        txt_temp[9] = ' ';
        txt_temp[10] = '\0';

        Lcd_Out(1, 1, "Temp: ");
        Lcd_Out(1, 7, txt_temp);
    }

    if (temp_scaled > 500) {
        LED_RESISTENCIA = 1;
    } else {
        LED_RESISTENCIA = 0;
    }
}

void exibir_tempo(unsigned int tempo) {
    char txt_tempo[10];

    txt_tempo[0] = (tempo / 10) + '0';
    txt_tempo[1] = (tempo % 10) + '0';
    txt_tempo[2] = ' ';
    txt_tempo[3] = 's';
    txt_tempo[4] = ' ';
    txt_tempo[5] = ' ';
    txt_tempo[6] = ' ';
    txt_tempo[7] = ' ';
    txt_tempo[8] = ' ';
    txt_tempo[9] = '\0';

    Lcd_Out(2, 1, "Tempo: ");
    Lcd_Out(2, 8, txt_tempo);
}

// Configuração do Conversor A/D

void configurar_ADC() {
    TRISA = 0x0D; // para RA0=AN0, RA2=Vref-, RA3=Vref+
    ADC_Init();
    ADCON1 = 0x3B;
}

// Configuração das interruopções externas

void configurar_interrupcoes_externas() {
    INTEDG0_bit = 1;    // INT0 (RB0) na borda de subida
    INTEDG1_bit = 1;    // INT1 (RB1) na borda de subida

    INT0IF_bit = 0;     // Limpa flags
    INT1IF_bit = 0;

    INT0IE_bit = 1;     // Habilita interrupções externas
    INT1IE_bit = 1;
}

// Configuração do Timer0 (timer de tempo longo - 60s)

void configurar_Timer0() {
    T0CON = 0x01;       //modo 16 bits,prescaler 1:4,desligado inicialmente

    //carga para estouro em 100ms com cristal de 8MHz
    TMR0H = 0x3C;
    TMR0L = 0xB0;

    TMR0IF_bit = 0;     //limpa flag
    TMR0IE_bit = 0;     //mantém interrupção desligada até o botão ser pressionado
}

//Configuração do Timer1(timer de tempo curto - 10s)

void configurar_Timer1() {
    T1CON = 0x30;       //prescaler 1:8,desligado inicialmente

    //carga para estouro em 250ms com cristal de 8MHz
    TMR1H = 0x0B;
    TMR1L = 0xDC;

    TMR1IF_bit = 0;     //limpa flag
    TMR1IE_bit = 0;     //desabilitado até o disparo
}

//Rotina de Interrupção

void interrupt() {
    //interrupção externa(pino RB0)para modo de seleção
    if (INT0IF_bit) {
        if (!em_execucao) {
            modo_selecionado = !modo_selecionado;
        }
        INT0IF_bit = 0;
    }

    //interrupção externa(pino RB1) pata iniciar medição
    if (INT1IF_bit) {
        if (!em_execucao) {
            t0_counter = 0;
            t1_counter = 0;

            if (modo_selecionado == 0) {
                tempo_restante = 60;
                TMR0H = 0x3C;
                TMR0L = 0xB0;
                T0CON.B7 = 1;        //liga o Timer0
                TMR0IE_bit = 1;      //habilita a interrupção do Timer0
            } else {
                tempo_restante = 10;
                TMR1H = 0x0B;
                TMR1L = 0xDC;
                TMR1ON_bit = 1;      //liga o Timer1
                TMR1IE_bit = 1;      //habilita a interrupção do Timer1
            }
            em_execucao = 1;
        }
        INT1IF_bit = 0;
    }

    //interrupção do timer0 - base de tempo de 10 s
    if (TMR0IF_bit) {
        TMR0H = 0x3C;
        TMR0L = 0xB0;
        t0_counter++;

        if (t0_counter >= 10) { // 10 * 100ms = 1s
            t0_counter = 0;
            if (tempo_restante > 0) {
                tempo_restante--;
            } else {
               T0CON.B7 = 0;
                TMR0IE_bit = 0;
                em_execucao = 0;
            }
        }
        TMR0IF_bit = 0;
    }

    // interrupção do timer0 - base de tempo de 250 s
    if (TMR1IF_bit) {
        TMR1H = 0x0B;
        TMR1L = 0xDC;
        t1_counter++;

        if (t1_counter >= 4) { // 4 * 250ms = 1s
            t1_counter = 0;
            if (tempo_restante > 0) {
                tempo_restante--;
            } else {
                TMR1ON_bit = 0;
                TMR1IE_bit = 0;
                em_execucao = 0;
            }
        }
        TMR1IF_bit = 0;
    }
}