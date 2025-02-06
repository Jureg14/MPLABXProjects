// Revised code for temperature, humidity and CO2 control using PIC18F4550
// Adhering to C89 Standard and using // comments

/*
 * File:   main.c
 * Author: JeeJ (Revised by AI Assistant)
 *
 * Created on 16 de Novembro de 2024, 09:55
 * Revised on 17 de Novembro de 2024 by AI Assistant
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

// Include necessary header files
#include <xc.h>         // XC8 compiler specific header file
#include <stdio.h>      // Standard input/output functions
#include <stdlib.h>     // Standard library functions
#include "nxlcd.h"      // Header file for LCD library (ensure this is in your project)

// Define crystal frequency for delay functions
#define _XTAL_FREQ 20000000    // Crystal frequency is 20MHz

// Define keypad dimensions
#define ROWS 4
#define COLS 4
#define DEBOUNCE_DELAY 50  // Debounce time in milliseconds
#define KEY_WAIT_PERIOD 300  // Wait period after a key press in milliseconds

// Keypad mapping for 4x4 matrix keypad
const char keymap[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'F', '0', 'E', 'D'}
};

// Global variables
volatile unsigned long milliseconds = 0; // Millisecond counter, incremented by timer interrupt
volatile unsigned char keyPressed = 0;     // Flag to indicate a key press (not used in current code, can be removed if not needed)
volatile int tak;                           // Variable toggled by timer interrupt (purpose unclear from original code, might be for blinking or periodic tasks)
int Tlim = 30;                              // Default temperature limit
int Hlim = 50;                              // Default humidity limit
int Glim = 20;                              // Default gas limit


// Function prototypes (declared according to C89 standard at the beginning)
char readKey(void);
unsigned int readAnalog(unsigned char pin);
void displayStuff(int row, int column, char *stuff);
void configureIO(void);
void configureADC(void);
void configureLCD(void);
void configureInterrupt(void);
void configureTimer(void);
unsigned long millis(void);
int readTwoDigitValue(char* prompt);
void displayMenu(int menuIndex);
int itoa(int value, char *ptr);
void floatToString(float value, char *buffer, int precision);
float tempRead(void);
float humidade(void);
float gasRead(void);
void setCoolerSpeed(int PWMset); // PWM control function for cooler (not implemented)
void setMoist(int moistSet);     // Function to control humidifier (not implemented)
void buzzer(int buzzerStatus);     // Function to control buzzer (not implemented)
void ISR(void);                 // Interrupt Service Routine prototype


// Main function - program entry point
void main(void) {
    char lastKey = '\0';          // Stores the last key pressed, initialized to null character
    int menuIndex = 0;             // Current menu index, starts at 0
    unsigned long lastDisplayUpdate = 0;  // Time of last display update, used for periodic updates
    char pressed_key;             // Variable to store the key currently pressed
    unsigned long currentTime;      // Variable to store current time in milliseconds
    float currentTemp;              // Variable to store current temperature reading
    float currentHumid;             // Variable to store current humidity reading
    float currentGas;               // Variable to store current gas concentration reading


    // --- System Configuration ---
    configureIO();         // Configure Input/Output pins
    configureADC();        // Configure Analog-to-Digital Converter
    configureLCD();        // Configure LCD module
    configureTimer();      // Configure Timer 0 for millisecond counting
    configureInterrupt();  // Configure Interrupts (Timer 0 and PORTB)


    // --- Output Pin Configuration (LEDs) ---
    TRISDbits.TRISD0 = 0; // Configure RD0 as output (LED for temperature alarm)
    TRISDbits.TRISD1 = 0; // Configure RD1 as output (LED for humidity alarm)
    TRISDbits.TRISD2 = 0; // Configure RD2 as output (LED for gas alarm)


    // --- Main Program Loop ---
    while (1) {
        pressed_key = readKey();         // Read current keypad input
        currentTime = millis();           // Get current time in milliseconds


        // --- Periodic Display Update ---
        if (currentTime - lastDisplayUpdate >= 500) { // Update display every 500 milliseconds
            displayMenu(menuIndex);                // Update LCD display based on current menu
            lastDisplayUpdate = currentTime;        // Reset last display update time
        }


        // --- Menu Navigation ---
        if (pressed_key == 'E' && lastKey != 'E') { // If 'E' is pressed and was not the last key
            menuIndex++;                             // Increment menu index to move to next menu
            if (menuIndex > 4) {                      // Wrap around menu index if it exceeds the last menu page (0-4, so 5 pages)
                menuIndex = 0;                         // Reset to the first menu page
            }
            displayMenu(menuIndex);                  // Update display immediately after menu change
        }


        // --- Limit Configuration ---
        if (pressed_key == 'A' && lastKey != 'A') { // If 'A' is pressed and was not the last key (Set Temperature Limit)
            int tempLimit = readTwoDigitValue("Limite Temp:"); // Prompt user to enter temperature limit
            if (tempLimit != -1) {                      // If a valid limit is entered (not cancelled -1)
                Tlim = tempLimit;                         // Update temperature limit
                displayStuff(1, 0, "Temp Salva!");       // Display confirmation message
                __delay_ms(1000);                        // Display message for 1 second
                menuIndex = 0;                           // Return to main menu
                displayMenu(menuIndex);                  // Update display to main menu
            } else if (tempLimit == -1) {               // If user cancelled input
                menuIndex = 0;                           // Return to main menu
                displayMenu(menuIndex);                  // Update display to main menu
            }
        }


        if (pressed_key == 'B' && lastKey != 'B') { // If 'B' is pressed and was not the last key (Set Humidity Limit)
            int humLimit = readTwoDigitValue("Limite Hum:"); // Prompt user to enter humidity limit
            if (humLimit != -1) {                      // If a valid limit is entered
                Hlim = humLimit;                         // Update humidity limit
                displayStuff(1, 0, "Humid Salvo!");      // Display confirmation
                __delay_ms(1000);                        // Display message for 1 second
                menuIndex = 0;                           // Return to main menu
                displayMenu(menuIndex);                  // Update display to main menu
            } else if (humLimit == -1) {               // If user cancelled input
                menuIndex = 0;                           // Return to main menu
                displayMenu(menuIndex);                  // Update display to main menu
            }
        }


        if (pressed_key == 'C' && lastKey != 'C') { // If 'C' is pressed and was not the last key (Set Gas Limit)
            int gasLimit = readTwoDigitValue("Limite Gas"); // Prompt for gas limit
            if (gasLimit != -1) {                      // If valid limit entered
                Glim = gasLimit;                         // Update gas limit
                displayStuff(1, 0, "Gas Salvo!");        // Confirmation message
                __delay_ms(1000);                        // Display message for 1 second
                menuIndex = 0;                           // Return to main menu
                displayMenu(menuIndex);                  // Update display to main menu
            } else if (gasLimit == -1) {               // If user cancelled input
                menuIndex = 0;                           // Return to main menu
                displayMenu(menuIndex);                  // Update display to main menu
            }
        }


        if (pressed_key == 'D' && lastKey != 'D') { // If 'D' is pressed and was not the last key (Reset Limits)
            Tlim = 30;                                 // Reset temperature limit to default
            Hlim = 50;                                 // Reset humidity limit to default
            Glim = 20;                                 // Reset gas limit to default
            displayStuff(0, 0, "Lim resetados!");     // Display reset confirmation
            __delay_ms(1000);                        // Display message for 1 second
            menuIndex = 0;                           // Return to main menu
            displayMenu(menuIndex);                  // Update display to main menu
        }


        // --- Sensor Readings ---
        currentTemp = tempRead();   // Read current temperature from sensor
        currentHumid = humidade();   // Read current humidity from sensor
        currentGas = gasRead();     // Read current gas concentration from sensor


        // --- Alarm and Actuator Control Logic ---
        if (currentTemp > Tlim || currentHumid > Hlim || currentGas > Glim) { // Check if any sensor value exceeds its limit
            buzzer(1);  // Activate buzzer if any limit is exceeded (buzzer function needs to be implemented)
        } else {
            buzzer(0);  // Deactivate buzzer if all values are within limits
        }


        if (currentTemp > Tlim) { // If temperature is above limit
            PORTDbits.RD0 = 1;    // Turn on temperature alarm LED (RD0)
            PORTCbits.RC2 = 1;    // Turn on cooler (RC2 - assuming RC2 controls cooler)
        } else {
            PORTDbits.RD0 = 0;    // Turn off temperature alarm LED
            PORTCbits.RC2 = 0;    // Turn off cooler
        }


        if (currentHumid > Hlim) { // If humidity is above limit
            PORTDbits.RD1 = 1;    // Turn on humidity alarm LED (RD1)
            setMoist(1);          // Activate humidifier (setMoist function needs implementation)
        } else {
            PORTDbits.RD1 = 0;    // Turn off humidity alarm LED
            setMoist(0);          // Deactivate humidifier
        }


        if (currentGas > Glim) {   // If gas concentration is above limit
            PORTDbits.RD2 = 1;    // Turn on gas alarm LED (RD2)
            setCoolerSpeed(255); // Example: Set cooler to max speed as exhaust (setCoolerSpeed function needs PWM implementation)
        } else {
            PORTDbits.RD2 = 0;    // Turn off gas alarm LED
            setCoolerSpeed(0);   // Set cooler speed to 0 when gas is within limit
        }


        lastKey = pressed_key;      // Update last key pressed for next iteration to detect key press events
    } // End of main loop
} // End of main function


// ------ Function Definitions ------

// Function to display menu options on LCD based on menu index
void displayMenu(int menuIndex) {
    char valueStr[10];                   // String buffer to hold integer/float values for display
    static unsigned long lastUpdateTime = 0; // Static variable to track last display update time for rate limiting
    unsigned long currentTime = millis();     // Get current time for display update rate limiting
    float currentTemp, currentHumid, currentGas; // Local variables to store sensor readings


    // --- Rate Limit Display Updates ---
    if (currentTime - lastUpdateTime < 500) { // Limit display updates to every 500 milliseconds
        return;                               // Exit function if not enough time has passed since last update
    }
    lastUpdateTime = currentTime;               // Update last display update time


    WriteCmdXLCD(0x01);                     // Clear LCD display
    __delay_ms(2);                            // Small delay after LCD command


    // --- Read Sensor Values for Display ---
    currentTemp = tempRead();                 // Read current temperature
    currentHumid = humidade();                 // Read current humidity
    currentGas = gasRead();                   // Read current gas concentration


    // --- Menu Content based on menuIndex ---
    switch (menuIndex) {
        case 0: // Menu Page 0: Temperature Display
            displayStuff(0, 0, "Temp:");         // Display "Temp:" on first row, first column
            floatToString(currentTemp, valueStr, 1); // Convert temperature to string with 1 decimal place
            displayStuff(0, 6, valueStr);         // Display temperature value
            displayStuff(0, 11, "C");           // Display "C" for Celsius unit
            displayStuff(1, 0, "Lim:");          // Display "Lim:" on second row
            itoa(Tlim, valueStr);                // Convert temperature limit to string
            displayStuff(1, 4, valueStr);         // Display temperature limit value
            break;


        case 1: // Menu Page 1: Humidity Display
            displayStuff(0, 0, "Humid:");        // Display "Humid:"
            floatToString(currentHumid, valueStr, 1); // Convert humidity to string with 1 decimal
            displayStuff(0, 7, valueStr);         // Display humidity value
            displayStuff(0, 12, "%");          // Display "%" for percentage unit
            displayStuff(1, 0, "Lim:");          // Display "Lim:"
            itoa(Hlim, valueStr);                // Convert humidity limit to string
            displayStuff(1, 4, valueStr);         // Display humidity limit value
            break;


        case 2: // Menu Page 2: Gas Concentration Display
            displayStuff(0, 0, "Gas:");          // Display "Gas:"
            floatToString(currentGas, valueStr, 1); // Convert gas to string with 1 decimal
            displayStuff(0, 5, valueStr);         // Display gas value
            displayStuff(0, 11, "ppm");          // Display "ppm" for parts per million unit
            displayStuff(1, 0, "Lim:");          // Display "Lim:"
            itoa(Glim, valueStr);                // Convert gas limit to string
            displayStuff(1, 4, valueStr);         // Display gas limit value
            break;


        case 3: // Menu Page 3: Sensor Readings Status
            displayStuff(0, 0, "Leituras");      // Display "Leituras" (Readings in Portuguese)
            if (currentTemp <= Tlim && currentHumid <= Hlim && currentGas <= Glim) { // Check if all readings are within limits
                displayStuff(1, 0, "OK");        // Display "OK" if all within limits
            } else { // If any reading exceeds limit, cycle through error messages
                static int errorCycle = 0;        // Static variable to cycle through error messages
                errorCycle++;                      // Increment error cycle counter

                if (errorCycle > 3) errorCycle = 0; // Reset error cycle after displaying all error types

                switch (errorCycle) {              // Display different error messages in cycle
                    case 0:
                        if (currentTemp > Tlim) { // If temperature limit exceeded
                            displayStuff(1, 0, "T_lim excedida!"); // Display "T_lim excedida!" (Temp limit exceeded)
                        }
                        break;
                    case 1:
                        if (currentHumid > Hlim) { // If humidity limit exceeded
                            displayStuff(1, 0, "H_lim excedida!"); // Display "H_lim excedida!" (Humidity limit exceeded)
                        }
                        break;
                    case 2:
                        if (currentGas > Glim) {   // If gas limit exceeded
                            displayStuff(1, 0, "G_lim excedido!"); // Display "G_lim excedido!" (Gas limit exceeded)
                        }
                        break;
                }
            }
            break;


        case 4: // Menu Page 4: System Status (Actuators ON/OFF)
            displayStuff(0, 0, "Sistemas:");      // Display "Sistemas:" (Systems in Portuguese)
            if (currentTemp <= Tlim && currentHumid <= Hlim && currentGas <= Glim) { // Check if all readings are within limits
                displayStuff(1, 0, "OK");        // Display "OK" if all systems are nominal
            } else { // If any limit exceeded, cycle through system status messages (actuators ON)
                static int systemCycle = 0;       // Static variable for system cycle
                systemCycle++;                     // Increment system cycle counter

                if (systemCycle > 3) systemCycle = 0; // Reset system cycle after all system statuses displayed

                switch (systemCycle) {             // Cycle through different system statuses
                    case 0:
                        if (currentTemp > Tlim) { // If temperature limit exceeded
                            displayStuff(1, 0, "Resfriador ON"); // Display "Resfriador ON" (Cooler ON)
                        }
                        break;
                    case 1:
                        if (currentHumid > Hlim) { // If humidity limit exceeded
                            displayStuff(1, 0, "Humidificador ON"); // Display "Humidificador ON" (Humidifier ON)
                        }
                        break;
                    case 2:
                        if (currentGas > Glim) {   // If gas limit exceeded
                            displayStuff(1, 0, "Exaustor ON");     // Display "Exaustor ON" (Exhaust fan ON)
                        }
                        break;
                }
            }
            break;


        default: // Default case for invalid menu index
            displayStuff(0, 0, "Invalid Menu");  // Display "Invalid Menu" for error case
            break;
    }
} // End of displayMenu function



// Function to read a two-digit value from keypad, with a prompt on LCD
int readTwoDigitValue(char* prompt) {
    char input[3] = {0};        // Character array to store input digits, null-terminated
    int digitCount = 0;         // Counter for number of digits entered
    char key;                   // Variable to store keypad input

    WriteCmdXLCD(0x01);        // Clear LCD display
    __delay_ms(2);              // Delay after LCD command
    WriteCmdXLCD(0x02);        // Return cursor to home position
    __delay_ms(2);              // Delay after LCD command

    displayStuff(0, 0, prompt); // Display the prompt message on the first line of LCD

    while (digitCount < 2) {    // Loop to read two digits
        key = readKey();         // Read a key from keypad

        if (key >= '0' && key <= '9') { // If the key is a numeric digit
            input[digitCount] = key;     // Store the digit in input array
            displayStuff(1, digitCount, &input[digitCount]); // Display digit on the second line of LCD
            digitCount++;                // Increment digit count
            __delay_ms(200);             // Short delay for visual feedback
        }

        if (key == 'F') {             // If 'F' key is pressed (Cancel)
            return -1;                 // Return -1 to indicate cancellation
        }
    }

    // Convert the two-digit string to an integer
    return (input[0] - '0') * 10 + (input[1] - '0'); // Convert ASCII digits to integer value
} // End of readTwoDigitValue function



// Interrupt Service Routine (ISR) - handles Timer 0 interrupts
void __interrupt() ISR() {
    if (INTCONbits.TMR0IF) { // Check if Timer 0 interrupt flag is set (Timer 0 overflow occurred)
        TMR0L = 100;          // Reload Timer 0 low byte to create 1ms overflow (adjust based on prescaler and clock)
        milliseconds++;       // Increment millisecond counter
        if (milliseconds % 1000 == 0) { // Check if a second has passed
            if (tak > 1) {      // Toggle 'tak' variable every second (purpose unclear, might be for blinking)
                tak = 0;
            } else {
                tak = 1;
            }
        }
        INTCONbits.TMR0IF = 0;  // Clear Timer 0 interrupt flag to allow next interrupt
    }
} // End of ISR function



// Function to get the current millisecond count
unsigned long millis(void) {
    unsigned long ms;         // Local variable to hold millisecond value

    INTCONbits.GIE = 0;      // Disable global interrupts to ensure atomic read of milliseconds
    ms = milliseconds;       // Read the millisecond counter value
    INTCONbits.GIE = 1;      // Re-enable global interrupts

    return ms;                // Return the millisecond value
} // End of millis function



// Function to configure Input/Output pins
void configureIO(void) {
    TRISB = 0xF0;           // Configure PORTB: RB4-RB7 as inputs (keypad rows), RB0-RB3 as outputs (keypad columns)
    TRISAbits.RA3 = 1;      // Configure RA3/AN3 as analog input (humidity sensor)
    TRISAbits.RA0 = 1;      // Configure RA0/AN0 as analog input (temperature sensor)
    TRISAbits.RA2 = 1;      // Configure RA2/AN2 as analog input (gas sensor)
    TRISDbits.TRISD0 = 0;   // Configure RD0 as output (Temperature alarm LED)
    TRISDbits.TRISD1 = 0;   // Configure RD1 as output (Humidity alarm LED)
    TRISCbits.TRISC2 = 0;   // Configure RC2 as output (Cooler control - assuming RC2 controls the cooler)
    PORTDbits.RD0 = 0;        // Initialize RD0 LED to off
    PORTDbits.RD1 = 0;        // Initialize RD1 LED to off
    PORTCbits.RC2 = 0;        // Initialize cooler control pin to off
} // End of configureIO function



// Function to configure LCD module
void configureLCD(void) {
    OpenXLCD(FOUR_BIT & LINES_5X7); // Initialize LCD in 4-bit mode, 5x7 font, and 2 lines
    WriteCmdXLCD(0x01);             // Clear LCD display
    __delay_ms(10);                 // Delay after clear command
    WriteCmdXLCD(0x02);             // Return cursor to home position
    __delay_ms(2);                  // Delay after home command
    WriteCmdXLCD(0x0C);             // Turn display on, cursor off, no blink
    __delay_ms(2);                  // Delay after display command
} // End of configureLCD function



// Function to configure interrupts
void configureInterrupt(void) {
    RCONbits.IPEN = 1;          // Enable interrupt priority levels
    INTCONbits.GIE = 1;         // Enable global interrupts (high priority and low priority)
    INTCONbits.PEIE = 1;        // Enable peripheral interrupts (for Timer 0)

    // Timer 0 interrupt configuration
    INTCON2bits.TMR0IP = 1;     // Set Timer 0 interrupt to high priority
    INTCONbits.TMR0IE = 1;      // Enable Timer 0 interrupt
    INTCONbits.TMR0IF = 0;      // Clear Timer 0 interrupt flag

    // PORTB interrupt configuration (not used in current code effectively for keypad, polling is used instead)
    INTCON2bits.RBIP = 0;       // Set PORTB change interrupt to low priority (not actively used in current keypad implementation)
    INTCONbits.RBIE = 0;       // Disable PORTB change interrupt (polling is used for keypad)
    INTCONbits.RBIF = 0;       // Clear PORTB change interrupt flag
} // End of configureInterrupt function



// Function to configure Timer 0 for millisecond timer
void configureTimer(void) {
    T0CON = 0b11000100;         // Timer 0 configuration:
                                // 11 - Internal instruction cycle clock (FOSC/4)
                                // 00 - 8-bit timer/counter
                                // 0100 - Prescaler 1:32 (adjust prescaler for 1ms overflow with 20MHz clock)
    TMR0L = 100;              // Initial Timer 0 low byte value (for 1ms overflow - adjust this value based on prescaler)
    INTCONbits.TMR0IF = 0;      // Clear Timer 0 interrupt flag
} // End of configureTimer function



// Debounce state structure for keypad handling
typedef struct {
    char lastKey;               // Last registered stable key
    char currentKey;            // Currently pressed key (during debounce)
    unsigned char keyPressCount; // Count of key presses (not used in debounce logic directly here)
    unsigned long lastKeyPressTime; // Time of the last key press event
    unsigned long lastRepeatTime;   // Time of last key repeat (not implemented in this simple debounce)
} KeyState;

// Global debounce state variable
KeyState keyState = {0};



// Function to read a key press from the matrix keypad with debounce and key press interval
char readKey(void) {
    const int InvCol[COLS] = {3, 2, 1, 0}; // Inverse column mapping for PORTB pinout (RB3, RB2, RB1, RB0 for columns)
    static unsigned long lastDebounceTime = 0;  // Static variable to store last debounce time
    static unsigned long lastKeyPressTime = 0;  // Static variable to store last valid key press time
    static char lastStableKey = 0;           // Static variable to store the last stable key detected
    unsigned long currentTime = millis();      // Get current time in milliseconds
    int col, row;                            // Loop counters


    for (col = 0; col < COLS; col++) {       // Iterate through each column of the keypad
        LATB = ~(1 << col);                 // Activate current column by setting corresponding RB pin low (columns are active low)
        __delay_ms(5);                      // Short delay for column pin stabilization


        for (row = 0; row < ROWS; row++) {   // Iterate through each row of the keypad
            if (!(PORTB & (1 << (row + 4)))) { // Check if key in current row and column is pressed (rows are on RB4-RB7)
                                                // Key press detected (row pin is low when pressed)
                char pressedKey = keymap[row][InvCol[col]]; // Get the character associated with the pressed key from keymap


                if (pressedKey != lastStableKey) { // If the currently pressed key is different from the last stable key
                    lastDebounceTime = currentTime;   // Reset debounce timer as a new key event is detected
                    lastStableKey = pressedKey;      // Update the last stable key to the currently pressed key
                }


                if ((currentTime - lastDebounceTime) > DEBOUNCE_DELAY) { // Check if key press is stable for debounce duration
                    if ((currentTime - lastKeyPressTime) > KEY_WAIT_PERIOD) { // Check for key press interval to avoid rapid repeated inputs
                        lastKeyPressTime = currentTime;    // Update last key press event time
                        keyState.currentKey = pressedKey;  // Update current key state
                        keyState.lastKey = pressedKey;     // Update last key state
                        return pressedKey;                 // Return the detected and debounced key character
                    }
                }
                return 0; // Key is being pressed but either debouncing or waiting for key release, return 0 (no new key event)
            } // End of key press detection
        } // End of row loop
    } // End of column loop


    keyState.lastKey = 0;         // Reset last key state when no key is pressed
    lastStableKey = 0;            // Reset last stable key
    return 0;                     // No key press detected or processed in this scan, return 0
} // End of readKey function



// Function to configure ADC module
void configureADC(void) {
    ADCON1 = 0b00001011;        // ADC Configuration Register 1:
                                // 0000 - VREF+ = VDD, VREF- = VSS (Voltage references are VDD and VSS)
                                // 1011 - AN0-AN3 are analog inputs, all others are digital I/O
    ADCON2 = 0b10010101;        // ADC Configuration Register 2:
                                // 1 - Right justified result (ADRESH:ADRESL)
                                // 001 - Acquisition time 2 TAD
                                // 010 - ADC clock FOSC/32 (adjust ADC clock for optimal conversion rate)
                                // 1 -  TAD = 0.5 us (min) at 20MHz Fosc and Fosc/32 = 625kHz ADC clock, TAD = 1/625kHz = 1.6us > 0.7us (min for PIC18F4550)
    ADCON0bits.ADON = 1;        // Enable ADC module (turn ADC module ON)
} // End of configureADC function



// Function to read analog value from specified ADC pin
unsigned int readAnalog(unsigned char pin) {
    unsigned int result = 0;      // Variable to store ADC result
    if (pin > 12) {              // Validate ADC pin number
        return 0;                 // Return 0 for invalid pin number
    }

    ADCON0bits.CHS = pin;         // Select ADC channel (pin) for conversion (Channel Select bits)
    __delay_ms(2);              // Allow channel acquisition time (settling time for analog input)
    ADCON0bits.GO_DONE = 1;       // Start ADC conversion (Go/Done bit - initiates conversion)

    while (ADCON0bits.GO_DONE);    // Wait for ADC conversion to complete (GO_DONE bit will be cleared by hardware when done)

    result = (ADRESH << 8) | ADRESL; // Combine ADRESH and ADRESL to get 10-bit ADC result (Right justified)
    return result;                 // Return the 10-bit ADC result
} // End of readAnalog function



// Function to read temperature from LM35 sensor connected to AN0/RA0
float tempRead(void) {
    unsigned int rawAnalog = readAnalog(0); // Read raw analog value from AN0 (temperature sensor)
    float TempC = ((rawAnalog * 5.0) / 1023.0) / 0.01; // Convert raw ADC value to temperature in Celsius:
                                                        // (rawAnalog * Vref / 1023) gives voltage, LM35 outputs 10mV/degree C (0.01V/degree C)
    return TempC;                              // Return temperature in Celsius
} // End of tempRead function



// Function to read humidity from sensor connected to AN3/RA3 (using a potentiometer simulation for humidity sensor in original code)
float humidade(void) {
    unsigned int rawAnalog = readAnalog(3); // Read raw analog value from AN3 (humidity sensor)
    float Humid = ((rawAnalog * 5.0) / 1023.0) * 20; // Convert raw ADC value to humidity percentage (0-100%):
                                                        // Assuming sensor output voltage range corresponds to 0-100% humidity linearly in 0-5V range.
                                                        // This conversion might need to be adjusted based on the actual humidity sensor characteristics.
    return Humid;                               // Return humidity percentage
} // End of humidade function



// Function to read gas concentration from sensor connected to AN2/RA2
float gasRead(void) {
    unsigned int rawAnalog = readAnalog(2); // Read raw analog value from AN2 (gas sensor)
    float gas = ((rawAnalog * 5.0) / 1023.0) * 20;  // Convert raw ADC value to gas concentration (in 'ppm' unit - unit is assumed and might need calibration):
                                                        // Similar linear scaling as humidity, might need sensor-specific conversion and calibration.
    return gas;                                 // Return gas concentration value
} // End of gasRead function



// Function to simplify LCD writing at specified row and column
void displayStuff(int row, int column, char *stuff) {
    int LCDWritePosition = (row == 0 ? 0x80 : 0xC0) + column; // Calculate LCD DDRAM address based on row and column:
                                                                // Row 0 starts at address 0x80, Row 1 starts at 0xC0

    WriteCmdXLCD(LCDWritePosition);             // Set LCD cursor position
    while (*stuff) {                             // Loop through each character in the input string
        WriteDataXLCD(*stuff);                   // Write character to LCD data register
        stuff++;                                 // Move to the next character in the string
    }
} // End of displayStuff function



// Placeholder function for setting cooler speed using PWM (PWM implementation needed)
void setCoolerSpeed(int PWMset) {
    // PWM control implementation for cooler speed control would go here
    // PWMset parameter would control the duty cycle or PWM value to set the cooler speed
} // End of setCoolerSpeed function



// Placeholder function to control humidifier (On/Off control - implementation needed)
void setMoist(int moistSet) {
    if (moistSet == 1) {                        // If moistSet is 1, turn humidifier ON
        // Code to turn humidifier ON (e.g., set an output pin high) would go here
    } else {                                    // If moistSet is not 1 (e.g., 0), turn humidifier OFF
        // Code to turn humidifier OFF (e.g., set an output pin low) would go here
    }
} // End of setMoist function



// Placeholder function to control buzzer (On/Off control - implementation needed)
void buzzer(int buzzerStatus) {
    if (buzzerStatus == 1) {                      // If buzzerStatus is 1, activate buzzer
        // Code to activate buzzer (e.g., set an output pin high to drive a buzzer) would go here
    } else {                                      // If buzzerStatus is not 1, deactivate buzzer
        // Code to deactivate buzzer (e.g., set buzzer control pin low) would go here
    }
} // End of buzzer function



// Function to convert integer to string (C89 compatible itoa implementation)
int itoa(int value, char *ptr) {
    int count = 0, temp;                         // Initialize variables
    char *start = ptr;                           // Store starting pointer for return calculation

    if (ptr == NULL)                             // Check for null pointer input
        return 0;                                // Return 0 if pointer is NULL

    if (value == 0) {                             // Handle case of value being 0
        *ptr = '0';                              // Set string to "0"
        ptr[1] = '\0';                           // Null-terminate string
        return 1;                                // Return string length 1
    }

    if (value < 0) {                             // Handle negative numbers
        value = -value;                           // Make value positive for conversion
        *ptr++ = '-';                             // Add '-' sign to string
        count++;                                 // Increment count for '-' sign
        start = ptr;                             // Update start pointer to after '-' sign
    }

    temp = value;                               // Temporary variable to count digits
    while (temp > 0) {                           // Count number of digits in value
        temp /= 10;                             // Integer division to remove last digit
        count++;                                 // Increment digit count
    }

    ptr[count] = '\0';                          // Null-terminate the string

    temp = value;                               // Reset temp to original value for digit conversion
    while (temp > 0) {                           // Convert integer digits to characters from right to left
        ptr[--count] = temp % 10 + '0';         // Get last digit, convert to char, and place in string from right
        temp /= 10;                             // Remove last digit
    }

    return count + (start != ptr);              // Return length of converted string (including '-' sign if any)
} // End of itoa function



// Function to convert float to string with specified precision (C89 compatible float to string)
void floatToString(float value, char *buffer, int precision) {
    int intPart;                                 // Integer part of the float
    int i, index = 0;                            // Loop counters and index variable
    char intStr[12];                             // Temporary string buffer for integer part


    if (value < 0) {                             // Handle negative values
        *buffer++ = '-';                           // Add '-' sign to buffer
        value = -value;                           // Make value positive for conversion
    }

    intPart = (int)value;                       // Extract integer part of float
    value -= intPart;                           // Get fractional part


    if(intPart == 0) {                           // Handle case of integer part being 0
        intStr[index++] = '0';                   // Store '0' for integer part if value is less than 1
    } else {                                    // Convert integer part to string (similar to itoa for positive ints)
        int temp = intPart;                       // Temporary variable for conversion
        index = 0;                               // Reset index for integer string conversion
        do {                                    // Convert digits of integer part to string
            intStr[index++] = (temp % 10) + '0';   // Get last digit and convert to character
            temp /= 10;                         // Remove last digit
        } while (temp > 0);
    }


    for (i = 0; i < index; i++) {               // Reverse the integer string (digits were added in reverse order)
        buffer[i] = intStr[index - i - 1];         // Copy reversed integer string to output buffer
    }
    buffer += index;                             // Update buffer pointer to end of integer string

    *buffer++ = '.';                             // Add decimal point to buffer


    for (i = 0; i < precision; i++) {           // Convert fractional part to string up to specified precision
        value *= 10;                             // Multiply fractional part by 10 to get next digit
        int digit = (int)value;                   // Extract integer part as next digit
        *buffer++ = digit + '0';                 // Convert digit to character and add to buffer
        value -= digit;                           // Update fractional part for next digit
    }

    *buffer = '\0';                              // Null-terminate the output string
} // End of floatToString function