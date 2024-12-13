/*
 * File:   main.c
 * Author: JeeJ
 *
 * Created on 16 de Novembro de 2024, 09:55
 */



// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = OFF     // CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON      // MCLR Pin Enable bit (RE3 input pin enabled; MCLR pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "nxlcd.h"


#define _XTAL_FREQ 20000000    // Frequência do cristal (20MHz)

// Definições dos pinos do teclado matricial
#define ROWS 4
#define COLS 4
//um método mais elegante (e correto?) de definir os botoes do teclado
const char keymap[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'F', '0', 'E', 'D'}
};

volatile unsigned int tempuu = 9; // variavel de Countdown, garante que começa a contagem regressiva em 9
volatile unsigned char keyPressed = 0;

//para não colocar o void main no fundo do codigo, as funções devem ser declaradas antes e definidas depois.
char readKey();
float readAnalog();
void displayStuff(char key, float V_analog);
void configureIO();
void configureADC();
void configureLCD();
void configureInterrupt();
void configureTimer();



int Vmax = 3; //pq sim
int intTempo;
int tak=0;

void main(void) {
    char key  ;  // Tecla pressionada
    int intKey = (key - '0');
    float V_analog;  // Valor da tensão analógica

    INTCONbits.TMR0IF = 0;// ter certeza de que a flag ta limpa
    
    //executa todas as funções de configuração
    configureIO();
    configureADC();
    configureLCD();
    configureTimer();
    configureInterrupt();

    while (1) {

        key = readKey();             // Ler tecla pressionada
       
        V_analog = readAnalog();     // Ler tensão analógica
        displayStuff(key, V_analog); // Exibir valores no LCD

        // Atualizar estado do LED com base na comparação
        if (V_analog < 3) {   //if (V_analog < (key - '0')) {
            PORTDbits.RD0 = 1;
            WriteCmdXLCD(0xC0);
            putsXLCD("LED:On ");
        } else {
            PORTDbits.RD0 = 0;
            WriteCmdXLCD(0xC0);
            putsXLCD("LED:Off");
        }
        //diminui a contagem toda vez que a flag tak é levantada
        if(tak!=0 && tempuu>0){
            tempuu--;
            tak=0;//abaixa a flag
        }
        
        //evita problemas
        if(tempuu>9){  
            tempuu = 9;
        }
       
        WriteCmdXLCD(0x80);
        putsXLCD("Countdown:");
        
        char stringTempo[4]; //transforma a int tempuu em string para mostrar no LCD
        stringTempo[0] = (tempuu / 10) + '0'; // Dezena
        stringTempo[1] = (tempuu % 10) + '0'; // Unidade          
        WriteCmdXLCD(139); //139 decimal ou 8B hex
        putsXLCD(stringTempo);
        

        
    }
}

// Certas partes do codigo foram separadas em funções para facilidar a leitura

void __interrupt() ISR() {
  
    if (INTCONbits.TMR0IF) { //interrupt do Timer 0
        static unsigned int tik = 0;
        //TMR0 da overflow a cada 0.1s, porque ai o valor do prescaler fica legal e bonito (sem decimal)
        //
        TMR0H = 0xC2; // Reload Timer 0
        TMR0L = 0xF7;

        tik++; //10*0.1=1s , logo a cada 10 tik a flag tak avisa que se passou 1 segundo
        if (tik >= 10) { // 10 overflows = 1 second
            tik = 0;
            tak=1;  //flag do shopee que na verdade é int
        }
        INTCONbits.TMR0IF = 0; // Limpa a flag do Interrupt
    }

 
    if (INTCONbits.RBIF) { //interrupt do PORTB
       // PORTDbits.RD1 = 1; // Turn on LED to indicate key press, debug
        char tecla = readKey(); //char temporaria  armazena a tecla teclada do teclado
        if((tecla-'0')<10 && (tecla-'0') >=0){
            tempuu = tecla-'0';   // transforma char para int
        }
        INTCONbits.RBIF = 0; // Limpa a flag do Interrupt
    }
}



// Configura todos os pinos de uma só vez
void configureIO() {
    TRISB = 0xF0;  // RB4-RB7 como entrada, RB0-RB3 como saída
    TRISAbits.RA3 = 1;  // AN3 como entrada analógica
    TRISDbits.TRISD0 = 0; // RD0 como saída
    TRISDbits.TRISD1 = 0; // RD1 como saída
    PORTDbits.RD0 = 0;    // LED inicialmente apagado
    PORTDbits.RD1 = 0;    // LED inicialmente apagado
    
}

// Configuração do ADC
void configureADC() {
    ADCON1 = 0b00001011;  // AN3 habilitado, Vref+ = Vcc, Vref- = Vss
    ADCON0 = 0b00001101;  // Seleciona AN3 e liga o módulo A/D
    ADCON2 = 0b10010101;  // Alinhamento à direita, TAD = FOSC/16
   // __delay_ms(10);       // Aguardar estabilização
}

// Função para configurar o LCD
void configureLCD() {
    OpenXLCD(FOUR_BIT & LINES_5X7);
    WriteCmdXLCD(0x01);  // Limpar o LCD
   //__delay_ms(10);      // Aguardar comando
}

//
void configureInterrupt()
{
    //config geral
    RCONbits.IPEN = 1;         // Enable interrupt priority
    INTCONbits.GIE = 1;        // Enable global interrupts
    INTCONbits.PEIE = 1;       // Enable peripheral interrupts
    //timer
    INTCON2bits.TMR0IP = 1; // High priority for Timer 0
    INTCONbits.TMR0IE = 1;  // Enable Timer 0 interrupt
    INTCONbits.TMR0IF = 0;  // Clear interrupt flag
    //pinos
    INTCON2bits.RBIP = 0;    // Low priority for PORTB
    INTCONbits.RBIE = 1;    // Habilita interrupção no PORTB
    INTCONbits.RBIF = 0;    // Limpa a flag de interrupção do PORTB
    
}

//
void configureTimer() {
    T0CON = 0b10000100; // Enable Timer 0 with:
    // TMR0ON = 1 (ativa Timer 0)
    // T08BIT = 0 (16-bit)
    // PSA = 0 (prescaler ligado)
    // T0PS = 100 (prescaler 1:32)

    TMR0H = 0xC2; // cria overflow a cada 0.1s
    TMR0L = 0xF7; // 

    INTCONbits.TMR0IF = 0; // Clear the Timer 0 interrupt flag
}

// Função para ler o teclado matricial
char readKey() { 
    static char currentKey; //"static char" não some apos a função ser executada ("char" some)
    const int InvCol[COLS] = {3, 2, 1, 0}; //metodo questionavel para inverter as colunas
    
    for (int col = 0; col < COLS; col++) {
        LATB = ~(1 << col); // Ativa coluna atual
        //LATB é um byte controla os estados de saída dos pinos do PORTB (quando configurados como output) (byte = 00000000)
        // "<<" desloca o bit de valor "1" de acordo com o valor de "COLS", ou seja, col começa em 0, logo LATB = 00000001, depois COLS=1 e LATB = 00000010, etc
        // "~" inverte o valor dos bits acima, ou seja, 00000001 vira 11111110
        __delay_ms(5); //tal do software debounce

        for (int row = 0; row < ROWS; row++) {
            if (!(PORTB & (1 << (row + 4)))) { // Verifica linha pressionada
                currentKey = keymap[row][InvCol[col]];       //mesma bruxaria do loop acima, mas dessa vez adiciona um offset de 4 bits para acionar as portas certas
                return currentKey;
            }
        }
    }
    return currentKey;  // quando nenhuma tecla pressionada, retorna o valor da ultima tecla pressionada
}

// Função para ler o valor analógico do potenciômetro
float readAnalog() {
    ADCON0bits.GO_DONE = 1;  // Inicia a conversão A/D
    while (ADCON0bits.GO_DONE);  // Aguarda a conclusão
    unsigned int result = (ADRESH << 8) | ADRESL;
    return (result / 1023.0) * 5.0; // Converte para tensão (0-5V)
}

// Função para exibir valores no LCD
void displayStuff(char key, float V_analog) {
    int inteiro = (int)V_analog;
    int decimal = (int)((V_analog - inteiro) * 100);

    //WriteCmdXLCD(0x80); desnecessario pro lab atual
    //putsXLCD("Vlim: ");
    //putcXLCD(key);

    WriteCmdXLCD(0xC7);
    putsXLCD(" Vin:");
    WriteCmdXLCD(0xCC);
    putcXLCD(inteiro + '0');
    putcXLCD('.');
    putcXLCD((decimal / 10) + '0');
    putcXLCD((decimal % 10) + '0');
}
