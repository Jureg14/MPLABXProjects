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
#include <stdio.h>
#include <stdlib.h>
#include "nxlcd.h"

#define _XTAL_FREQ 20000000    // Frequ�ncia do cristal (20MHz)

#define ROWS 4
#define COLS 4
#define DEBOUNCE_DELAY 50  // Debounce time in milliseconds
#define KEY_WAIT_PERIOD 300  // Wait period after a key press in milliseconds

const char keymap[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'F', '0', 'E', 'D'}
};


volatile unsigned long milliseconds = 0;
volatile unsigned char keyPressed = 0;
volatile int tak;
int Tlim = 30;  // Default temperature limit
int Hlim = 50;  // Default humidity limit
int Glim = 20;  // Default gas limit

// Redeclare all original function prototypes
char readKey();
unsigned int readAnalog(unsigned char pin);
void displayStuff(int row, int column, char *stuff);
void configureIO();
void configureADC();
void configureLCD();
void configureInterrupt();
void configureTimer();
unsigned long millis();
int readTwoDigitValue(char* prompt);
void displayMenu(int menuIndex);
int itoa(int value, char *ptr);
void floatToString(float value, char *buffer, int precision);
float tempRead();
float humidade();
float gasRead();
void setCoolerSpeed(int PWMset);
void setMoist(int moistSet);
void buzzer(int buzzerStatus);

void main(void) {
    // Execute configuration functions
    configureIO();
    configureADC();
    configureLCD();
    configureTimer();
    configureInterrupt();
    
    TRISDbits.TRISD0 = 0; // Configure pins RD0, RD1, RD2
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    
    char lastKey = '\0';  // Fixed null character initialization
    int menuIndex = 0;
    unsigned long lastDisplayUpdate = 0;  // Add a timer for display updates

    while (1) {
        char pressed_key = readKey();
        unsigned long currentTime = millis();  // Get current time

        // Periodic display update (every 500ms)
        if (currentTime - lastDisplayUpdate >= 500) {
            displayMenu(menuIndex);
            lastDisplayUpdate = currentTime;
        }
        
        // Menu navigation with 'E' key
        if (pressed_key == 'E' && lastKey != 'E') {
            menuIndex++;
            if (menuIndex > 4) { // Define number of menu pages
                menuIndex = 0;
            }
            displayMenu(menuIndex);  // Immediate update when changing menu
        }
        
        // Temperature limit configuration with 'A' key
        if (pressed_key == 'A' && lastKey != 'A') {
            int tempLimit = readTwoDigitValue("Limite Temp:");
            if (tempLimit != -1) {
                Tlim = tempLimit;
                displayStuff(1, 0, "Temp Salva!");
                __delay_ms(10);
            }else if (tempLimit == -1){menuIndex = 0;displayMenu(menuIndex);}
        }
        
        // Humidity limit configuration with 'B' key
        if (pressed_key == 'B' && lastKey != 'B') {
            int humLimit = readTwoDigitValue("Limite Hum:");
            if (humLimit != -1) {
                Hlim = humLimit;
                displayStuff(1, 0, "Humid Salvo!");
                __delay_ms(10);
            }
        }
        
        // Gas limit configuration with 'C' key
        if (pressed_key == 'C' && lastKey != 'C') {
            int gasLimit = readTwoDigitValue("Limite Gas");
            if (gasLimit != -1) {
                Glim = gasLimit;
                displayStuff(1, 0, "Gas Salvo!");
                __delay_ms(10);
            }
        }
        
        // Reset or special function with 'D' key
        if (pressed_key == 'D' && lastKey != 'D') {
            // Reset all limits to default
            Tlim = 30;  
            Hlim = 50;  
            Glim = 20;  
            displayStuff(0, 0, "Lim resetados!");
            __delay_ms(10);
        }

        // Armazena localmente as variaveis para comparar nos 'if' abaixo
        float currentTemp = tempRead();
        float currentHumid = humidade();
        float currentGas = gasRead();

        // Alarme caso algum limite seja excedido
        if (currentTemp > Tlim || currentHumid > Hlim || currentGas > Glim) {
            buzzer(1);  // Activate buzzer if any limit is exceeded
        } else {
            buzzer(0);  // Deactivate buzzer
        }
        if (currentTemp > Tlim){
            PORTDbits.RD0 = 1;
            PORTCbits.RC2 = 1; //Liga a ventoinha
        }else {
            PORTDbits.RD0 = 0;
            PORTCbits.RC2 = 0;//desliga a ventoinha
        }
        if (currentHumid > Hlim){
            PORTDbits.RD1 = 1;
        }else {
            PORTDbits.RD1 = 0;
        }
        if (currentGas > Glim){
            PORTDbits.RD2 = 1;
        }else {
            PORTDbits.RD2 = 0;
        }
        

        // Update last key read
        lastKey = pressed_key;
    }
}

// Certas partes do codigo foram separadas em fun��es para facilidar a leitura

void displayMenu(int menuIndex) {
    char valueStr[10];
    static unsigned long lastUpdateTime = 0;
    unsigned long currentTime = millis();
    
    // Update display every 500ms
    if (currentTime - lastUpdateTime < 500) {
        return;  // Exit if not enough time has passed
    }
    lastUpdateTime = currentTime;
    
    // Clear the LCD before displaying each menu screen
    WriteCmdXLCD(0x01);
    __delay_ms(2);
    
    // Read current sensor values
    float currentTemp = tempRead();
    float currentHumid = humidade();
    float currentGas = gasRead();
    
    switch(menuIndex)
    {
        case 0:
            // Temperature display
            displayStuff(0, 0, "Temp:");
            floatToString(currentTemp, valueStr, 1);
            displayStuff(0, 6, valueStr);
            displayStuff(0, 11, "C");
            
            // Show temperature limit
            displayStuff(1, 0, "Lim:");
            itoa(Tlim, valueStr);
            displayStuff(1, 4, valueStr);
            break;
        
        case 1:
            // Humidity display
            displayStuff(0, 0, "Humid:");
            floatToString(currentHumid, valueStr, 1);
            displayStuff(0, 7, valueStr);
            displayStuff(0, 12, "%");
            
            // Show humidity limit
            displayStuff(1, 0, "Lim:");
            itoa(Hlim, valueStr);
            displayStuff(1, 4, valueStr);
            break;
        
        case 2:
            // Gas concentration display
            displayStuff(0, 0, "Gas:");
            floatToString(currentGas, valueStr, 1);
            displayStuff(0, 5, valueStr);
            displayStuff(0, 11, "ppm");
            
            // Show gas limit
            displayStuff(1, 0, "Lim:");
            itoa(Glim, valueStr);
            displayStuff(1, 4, valueStr);
            break;
        
        case 3:
            displayStuff(0, 0, "Leituras");
    
            if(currentTemp <= Tlim && currentHumid <= Hlim && currentGas <= Glim){
                displayStuff(1, 0, "OK");
            } else {
                static int errorCycle = 0;
                errorCycle++;
                
                if (errorCycle > 3) errorCycle = 0;
                
                switch(errorCycle) {
                    case 0:
                        if(currentTemp > Tlim){
                            displayStuff(1, 0, "T_lim excedida!");
                        }
                        break;
                    case 1:
                        if(currentHumid > Hlim){
                            displayStuff(1, 0, "H_lim excedida!");
                        }
                        break;
                    case 2:
                        if(currentGas > Glim){
                            displayStuff(1, 0, "G_lim excedido!");
                        }
                        break;
                }
            }
            break;
        
        case 4:
            // Additional menu screen
            displayStuff(0, 0, "Sistemas:");
            if(currentTemp <= Tlim && currentHumid <= Hlim && currentGas <= Glim){
                displayStuff(1, 0, "OK");
            } else {
                static int systemCycle = 0;
                systemCycle++;
                
                if (systemCycle > 3) systemCycle = 0;
                
                switch(systemCycle) {
                    case 0:
                        if(currentTemp > Tlim){
                            displayStuff(1, 0, "Resfriador ON");
                        }
                        break;
                    case 1:
                        if(currentHumid > Hlim){
                            displayStuff(1, 0, "Humidificador ON");
                        }
                        break;
                    case 2:
                        if(currentGas > Glim){
                            displayStuff(1, 0, "Exaustor ON");
                        }
                        break;
                }
            }
            break;
        
        default:
            // Fallback for unexpected menu index
            displayStuff(0, 0, "Invalid Menu");
            break;
    }
}
// Modified to return -1 if cancelled
int readTwoDigitValue(char* prompt) {
    char input[3] = {0}; // Array to store digits
    int digitCount = 0;
    char key;

    // Clear LCD
        WriteCmdXLCD(0x01);  // Clear LCD
         __delay_ms(2);
         WriteCmdXLCD(0x02);  // Return home
         __delay_ms(2);
    
    // Show prompt
    displayStuff(0, 0, prompt);

    while (digitCount < 2) {
        key = readKey();

        // Numeric key
        if (key >= '0' && key <= '9') {
            input[digitCount] = key;
            displayStuff(1, digitCount, &input[digitCount]);
            digitCount++;
            __delay_ms(200);
        }
        
        // Cancel with 'F' key
        if (key == 'F') {
            return -1; // Indicates cancellation
        }
    }

    // Convert two-digit string to integer
    return (input[0] - '0') * 10 + (input[1] - '0');
}

void __interrupt() ISR() {
  
    if (INTCONbits.TMR0IF) { //interrupt do Timer 0

        TMR0L = 100;// overflow a cada 1ms
        milliseconds++;
        if(milliseconds%1000==0){
            if(tak>1){
                tak=0;
            }else{
               tak=1;
            }
        }
        INTCONbits.TMR0IF = 0; // Limpa a flag do Interrupt
    }

 
    //if (INTCONbits.RBIF) { //interrupt do PORTB
       // PORTDbits.RD1 = 1; // Turn on LED to indicate key press, debug
    //    char tecla = readKey(); //char temporaria  armazena a tecla teclada do teclado
    //    if((tecla-'0')<10 && (tecla-'0') >=0){
           // tempuu = tecla-'0';   // transforma char para int
    //    }
    //    INTCONbits.RBIF = 0; // Limpa a flag do Interrupt
    //}
}

// Function to get current milliseconds
unsigned long millis() {
    // Disable interrupts to prevent race condition
    INTCONbits.GIE = 0;
    
    // Safely read the milliseconds value
    unsigned long ms = milliseconds;
    
    // Re-enable interrupts
    INTCONbits.GIE = 1;
    
    return ms;
}

// Configura todos os pinos de uma s� vez
void configureIO() {
    TRISB = 0xF0;  // RB4-RB7 como entrada, RB0-RB3 como sa�da
    TRISAbits.RA3 = 1;  // AN3 como entrada anal�gica
    TRISDbits.TRISD0 = 0; // RD0 como sa�da
    TRISDbits.TRISD1 = 0; // RD1 como sa�da
    TRISCbits.TRISC2 = 0; // RD1 como sa�da
    PORTDbits.RD0 = 0;    // LED inicialmente apagado
    PORTDbits.RD1 = 0;    // LED inicialmente apagado
    
}

// Fun��o para configurar o LCD
void configureLCD() {
    OpenXLCD(FOUR_BIT & LINES_5X7);
    WriteCmdXLCD(0x01);  // Clear LCD
    __delay_ms(10);      // Add a small delay after clearing
    
    // Try adding these initialization commands
    WriteCmdXLCD(0x02);  // Return home
    __delay_ms(2);
    WriteCmdXLCD(0x0C);  // Display on, cursor off
    __delay_ms(2);

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
    INTCONbits.RBIE = 1;    // Habilita interrup��o no PORTB
    INTCONbits.RBIF = 0;    // Limpa a flag de interrup��o do PORTB
    
}

//
void configureTimer() {
    T0CON = 0b11000100; // Enable Timer 0 with:
    // TMR0ON = 1 (ativa Timer 0)
    // T08BIT = 1 (8-bit)
    // PSA = 0 (prescaler ligado)
    // T0PS = 100 (prescaler 1:32)

    TMR0L = 100; // cria overflow a cada 1ms 

    INTCONbits.TMR0IF = 0; // Clear the Timer 0 interrupt flag
}

// Fun��o para ler o teclado matricial
// Debounce state structure
typedef struct {
    char lastKey;
    char currentKey;
    unsigned char keyPressCount;
    unsigned long lastKeyPressTime;
    unsigned long lastRepeatTime;
} KeyState;

// Global debounce state
KeyState keyState = {0};

char readKey() {
    const int InvCol[COLS] = {3, 2, 1, 0};
    static unsigned long lastDebounceTime = 0;
    static unsigned long lastKeyPressTime = 0;
    static char lastStableKey = 0;
    unsigned long currentTime = millis(); 
    
    for (int col = 0; col < COLS; col++) {
        LATB = ~(1 << col); // Activate current column
        __delay_ms(5);  // Short delay for column stabilization
        
        for (int row = 0; row < ROWS; row++) {
            if (!(PORTB & (1 << (row + 4)))) { // Key pressed
                char pressedKey = keymap[row][InvCol[col]];
                
                // Check if the key state is stable
                if (pressedKey != lastStableKey) {
                    lastDebounceTime = currentTime;
                    lastStableKey = pressedKey;
                }
                
                if ((currentTime - lastDebounceTime) > DEBOUNCE_DELAY) {
                    // Check if enough time has passed since the last key press
                    if ((currentTime - lastKeyPressTime) > KEY_WAIT_PERIOD) {
                        lastKeyPressTime = currentTime;
                        
                        // Register the key press
                        keyState.currentKey = pressedKey;
                        keyState.lastKey = pressedKey;
                        return pressedKey;
                    }
                }
                
                return 0;  // No new key event
            }
        }
    }
    
    // Reset state when no key is pressed
    keyState.lastKey = 0;
    lastStableKey = 0;
    return 0;
}

void configureADC() {
    ADCON1 = 0b00001011; // Configure AN0 to AN3 as analog, others as digital
    ADCON2 = 0b10010101; // Right justified, FOSC/16, 12 TAD
    ADCON0bits.ADON = 1; // Enable ADC module
}

unsigned int readAnalog(unsigned char pin) {
    if (pin > 12) {
        return 0; // Invalid pin, return 0
    }

    // Select the desired analog channel
    ADCON0bits.CHS = pin;

    // Allow the channel to settle
    __delay_ms(2);

    // Start ADC conversion
    ADCON0bits.GO_DONE = 1;

    // Wait for conversion to complete
    while (ADCON0bits.GO_DONE);

    // Combine the result from ADRESH and ADRESL
    unsigned int result = (ADRESH << 8) | ADRESL;

    return result;
}


float tempRead(){
    unsigned int rawAnalog=readAnalog(0);//lm35 est� no pino RA0/AN0
    float TempC = ((rawAnalog * 5.0) / 1023.0)/0.01; // Convert to voltage 
 return TempC;   
}
float humidade(){
    unsigned int rawAnalog=readAnalog(3);//POT est� no pino RA3/AN3
    float Humid = ((rawAnalog * 5.0) / 1023.0)*20; // leitura de 0 a 5 para uma humidade de 0 a 100
 return Humid;  
}
float gasRead(){
    unsigned int rawAnalog=readAnalog(2);//est� no pino RA2/AN2 //� uma entrada de 0 a 10v, deve ter algum jeito de usar
    float gas = ((rawAnalog * 5.0) / 1023.0)*20; // leitura de 0 a 5 para uma humidade de 0 a 100
 return gas;   
}

// simplifica o uso do  LCD
void displayStuff(int row, int column, char *stuff) {
    // Calculate LCD write position
    int LCDWritePosition = (row == 0 ? 0x80 : 0xC0) + column;
    
    WriteCmdXLCD(LCDWritePosition);
    
    // Write each character explicitly
    while (*stuff) {
        WriteDataXLCD(*stuff);
        stuff++;
    }
}

void setCoolerSpeed(int PWMset){
    //pwm crap goes here
}
void setMoist(int moistSet){
    if (moistSet == 1){
        //set moist pin on
    }else{
        //set moist pin off
    }
}

void buzzer(int buzzerStatus){ 
    if (buzzerStatus == 1){
        //set moist pin on
    }else{
        //set moist pin off
    }
}

int itoa(int value, char *ptr) //changes int to string
{
    int count = 0, temp;
    char *start = ptr;

    if(ptr == NULL)
        return 0;

    if(value == 0)
    {
        *ptr = '0';
        ptr[1] = '\0';
        return 1;
    }

    // Handle negative numbers
    if(value < 0)
    {
        value = -value;
        *ptr++ = '-';
        count++;
        start = ptr;  // Start of actual number digits
    }

    // Count digits first
    temp = value;
    while(temp > 0)
    {
        temp /= 10;
        count++;
    }

    // Null-terminate first
    ptr[count] = '\0';

    // Convert digits from right to left
    temp = value;
    while(temp > 0)
    {
        ptr[--count] = temp % 10 + '0';
        temp /= 10;
    }

    return count + (start != ptr);
}

void floatToString(float value, char *buffer, int precision) {
    // Handle negative values
    if (value < 0) {
        *buffer++ = '-';
        value = -value;
    }

    // Extract the integer part
    int intPart = (int)value;
    value -= intPart;

    // Convert the integer part to a string
    int temp = intPart;
    char intStr[12];
    int index = 0;
    do {
        intStr[index++] = (temp % 10) + '0';
        temp /= 10;
    } while (temp > 0);

    // Reverse the integer string
    for (int i = 0; i < index; i++) {
        buffer[i] = intStr[index - i - 1];
    }
    buffer += index;

    // Add the decimal point
    *buffer++ = '.';

    // Convert the fractional part
    for (int i = 0; i < precision; i++) {
        value *= 10;
        int digit = (int)value;
        *buffer++ = digit + '0';
        value -= digit;
    }

    // Null-terminate the string
    *buffer = '\0';
}