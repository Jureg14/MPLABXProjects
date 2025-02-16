/*
 * Arquivo:   main.c
 * Autor: JeeJ (Julio Cesar)
 *
 * Criado em 16 de Novembro de 2024, 09:55
 */

// Configurações de bits do PIC18F4550

// Declarações de configuração de linha 'C'

// CONFIG1L
#pragma config PLLDIV = 1       // Bits de seleção do prescaler PLL (Sem prescaler (entrada do oscilador de 4 MHz aciona o PLL diretamente))
#pragma config CPUDIV = OSC1_PLL2// Bits de seleção do pós-escaler do clock do sistema ([Fonte do oscilador primário: /1][Fonte do PLL de 96 MHz: /2])
#pragma config USBDIV = 1       // Bit de seleção do clock USB (usado apenas no modo USB Full-Speed; UCFG:FSEN = 1) (A fonte do clock USB vem diretamente do bloco do oscilador primário sem pós-escala)

// CONFIG1H
#pragma config FOSC = HS        // Bits de seleção do oscilador (Oscilador HS (HS))
#pragma config FCMEN = OFF      // Bit de habilitação do monitor de clock fail-safe (Monitor de clock fail-safe desabilitado)
#pragma config IESO = OFF       // Bit de alternância entre oscilador interno/externo (Modo de alternância de oscilador desabilitado)

// CONFIG2L
#pragma config PWRT = OFF       // Bit de habilitação do timer de power-up (PWRT desabilitado)
#pragma config BOR = OFF        // Bits de habilitação do reset por brown-out (Reset por brown-out desabilitado em hardware e software)
#pragma config BORV = 3         // Bits de tensão de reset por brown-out (Configuração mínima de 2.05V)
#pragma config VREGEN = OFF     // Bit de habilitação do regulador de tensão USB (Regulador de tensão USB desabilitado)

// CONFIG2H
#pragma config WDT = OFF        // Bit de habilitação do timer watchdog (WDT desabilitado (o controle é colocado no bit SWDTEN))
#pragma config WDTPS = 32768    // Bits de seleção do pós-escaler do timer watchdog (1:32768)

// CONFIG3H
#pragma config CCP2MX = OFF     // Bit de multiplexação do CCP2 (A entrada/saída do CCP2 é multiplexada com RB3)
#pragma config PBADEN = OFF     // Bit de habilitação do A/D no PORTB (Os pinos PORTB<4:0> são configurados como I/O digital no Reset)
#pragma config LPT1OSC = OFF    // Bit de habilitação do oscilador do timer 1 de baixa potência (Timer1 configurado para operação de alta potência)
#pragma config MCLRE = ON      // Bit de habilitação do pino MCLR (Pino de entrada RE3 habilitado; pino MCLR desabilitado)

// CONFIG4L
#pragma config STVREN = OFF     // Bit de habilitação do reset por estouro/underflow da pilha (Estouro/underflow da pilha não causará reset)
#pragma config LVP = OFF        // Bit de habilitação do ICSP de alimentação única (ICSP de alimentação única desabilitado)
#pragma config ICPRT = OFF      // Bit de habilitação da porta dedicada de depuração/programação em circuito (ICPORT) (ICPORT desabilitada)
#pragma config XINST = OFF      // Bit de habilitação do conjunto de instruções estendido (Extensão do conjunto de instruções e modo de endereçamento indexado desabilitados (Modo legado))

// CONFIG5L
#pragma config CP0 = OFF        // Bit de proteção de código (Bloco 0 (000800-001FFFh) não está protegido por código)
#pragma config CP1 = OFF        // Bit de proteção de código (Bloco 1 (002000-003FFFh) não está protegido por código)
#pragma config CP2 = OFF        // Bit de proteção de código (Bloco 2 (004000-005FFFh) não está protegido por código)
#pragma config CP3 = OFF        // Bit de proteção de código (Bloco 3 (006000-007FFFh) não está protegido por código)

// CONFIG5H
#pragma config CPB = OFF        // Bit de proteção de código do bloco de boot (Bloco de boot (000000-0007FFh) não está protegido por código)
#pragma config CPD = OFF        // Bit de proteção de código da EEPROM de dados (EEPROM de dados não está protegida por código)

// CONFIG6L
#pragma config WRT0 = OFF       // Bit de proteção de escrita (Bloco 0 (000800-001FFFh) não está protegido contra escrita)
#pragma config WRT1 = OFF       // Bit de proteção de escrita (Bloco 1 (002000-003FFFh) não está protegido contra escrita)
#pragma config WRT2 = OFF       // Bit de proteção de escrita (Bloco 2 (004000-005FFFh) não está protegido contra escrita)
#pragma config WRT3 = OFF       // Bit de proteção de escrita (Bloco 3 (006000-007FFFh) não está protegido contra escrita)

// CONFIG6H
#pragma config WRTC = OFF       // Bit de proteção de escrita dos registradores de configuração (Registradores de configuração (300000-3000FFh) não estão protegidos contra escrita)
#pragma config WRTB = OFF       // Bit de proteção de escrita do bloco de boot (Bloco de boot (000000-0007FFh) não está protegido contra escrita)
#pragma config WRTD = OFF       // Bit de proteção de escrita da EEPROM de dados (EEPROM de dados não está protegida contra escrita)

// CONFIG7L
#pragma config EBTR0 = OFF      // Bit de proteção de leitura de tabela (Bloco 0 (000800-001FFFh) não está protegido contra leituras de tabela executadas em outros blocos)
#pragma config EBTR1 = OFF      // Bit de proteção de leitura de tabela (Bloco 1 (002000-003FFFh) não está protegido contra leituras de tabela executadas em outros blocos)
#pragma config EBTR2 = OFF      // Bit de proteção de leitura de tabela (Bloco 2 (004000-005FFFh) não está protegido contra leituras de tabela executadas em outros blocos)
#pragma config EBTR3 = OFF      // Bit de proteção de leitura de tabela (Bloco 3 (006000-007FFFh) não está protegido contra leituras de tabela executadas em outros blocos)

// CONFIG7H
#pragma config EBTRB = OFF      // Bit de proteção de leitura de tabela do bloco de boot (Bloco de boot (000000-0007FFh) não está protegido contra leituras de tabela executadas em outros blocos)

// Incluir arquivos de cabeçalho necessários
#include <xc.h>         // Arquivo de cabeçalho específico do compilador XC8
#include <stdio.h>      // Funções padrão de entrada/saída
#include <stdlib.h>     // Funções padrão da biblioteca
#include "nxlcd.h"      // Arquivo de cabeçalho para a biblioteca LCD (certifique-se de que está no seu projeto)

// Definir frequência do cristal para funções de atraso
#define _XTAL_FREQ 20000000    // Frequência do cristal é 20MHz

// Definir dimensões do teclado
#define ROWS 4
#define COLS 4
#define DEBOUNCE_DELAY 50  // Tempo de debounce em milissegundos
#define KEY_WAIT_PERIOD 300  // Período de espera após uma tecla pressionada em milissegundos

// Mapeamento do teclado para teclado matricial 4x4
const char keymap[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'F', '0', 'E', 'D'}
};

// Variáveis globais
volatile unsigned long milliseconds = 0; // Contador de milissegundos, incrementado pela interrupção do timer
volatile int tak;                           // Variável alternada pela interrupção do timer
int Tlim = 30;                              // Limite padrão de temperatura
int Hlim = 50;                              // Limite padrão de umidade
int Glim = 20;                              // Limite padrão de gás

// Protótipos de funções
    // --- Funções que agrupam certas configurções para facilitar a leitura do codigo ---
void configureIO(void); //Agrupa quase todas as configurações de pinos de IO
void configureADC(void); // Configuração do conversor analógico-digital
void configureLCD(void); 
void configureInterrupt(void);
void configureTimer(void);
    // --- Funções de funcionalidades ---
char readKey(void);
unsigned int readAnalog(unsigned char pin); // Função para ler valores analógicos do pino especificado
void displayStuff(int row, int column, char *stuff); // Função para simplificar o uso do LCD
unsigned long millis(void);
int readTwoDigitValue(char* prompt);
void displayMenu(int menuIndex);
int itoa(int value, char *ptr);
void floatToString(float value, char *buffer, int precision);
float tempRead(void);
float humidade(void);
float gasRead(void);
void setCoolerSpeed(int PWMset); // Função de controle PWM para o cooler (não implementada)
void setMoist(int moistSet);     // Função para controlar o umidificador (não implementada)
void buzzer(int buzzerStatus);     // Função para controlar o buzzer (não implementada)
void ISR(void);                 // Protótipo da rotina de serviço de interrupção


// Função principal - ponto de entrada do programa
void main(void) {
    char lastKey = '\0';          // Armazena a última tecla pressionada, inicializada com caractere nulo
    int menuIndex = 0;             // Índice do menu atual, começa em 0
    unsigned long lastDisplayUpdate = 0;  // Tempo da última atualização do display, usado para atualizações periódicas
    char pressed_key;             // Variável para armazenar a tecla atualmente pressionada
    unsigned long currentTime;      // Variável para armazenar o tempo atual em milissegundos
    float currentTemp;              // Variável para armazenar a leitura atual da temperatura
    float currentHumid;             // Variável para armazenar a leitura atual da umidade
    float currentGas;               // Variável para armazenar a leitura atual da concentração de gás

    // --- Configuração do Sistema ---
    configureIO();         // Configurar pinos de entrada/saída
    configureADC();        // Configurar o conversor analógico-digital
    configureLCD();        // Configurar o módulo LCD
    configureTimer();      // Configurar o Timer 0 para contagem de milissegundos
    configureInterrupt();  // Configurar interrupções (Timer 0 e PORTB)

    // --- Configuração dos pinos de saída (LEDs) ---
    TRISDbits.TRISD0 = 0; // Configurar RD0 como saída (LED para alarme de temperatura)
    TRISDbits.TRISD1 = 0; // Configurar RD1 como saída (LED para alarme de umidade)
    TRISDbits.TRISD2 = 0; // Configurar RD2 como saída (LED para alarme de gás)

    // --- Loop Principal do Programa ---
    while (1) {
        pressed_key = readKey();         // Ler a entrada atual do teclado
        currentTime = millis();           // Obter o tempo atual em milissegundos

        // --- Atualização Periódica do Display ---
        if (currentTime - lastDisplayUpdate >= 500) { // Atualizar o display a cada 500 milissegundos
            displayMenu(menuIndex);                // Atualizar o display LCD com base no menu atual
            lastDisplayUpdate = currentTime;        // Reiniciar o tempo da última atualização do display
        }

        // --- Navegação no Menu ---
        if (pressed_key == 'E' && lastKey != 'E') { // Se 'E' for pressionado e não foi a última tecla
            menuIndex++;                             // Incrementar o índice do menu para ir para o próximo menu
            if (menuIndex > 4) {                      // Voltar ao início do menu se exceder a última página do menu (0-4, então 5 páginas)
                menuIndex = 0;                         // Reiniciar para a primeira página do menu
            }
            displayMenu(menuIndex);                  // Atualizar o display imediatamente após a mudança de menu
        }
        if (pressed_key == 'F' && lastKey != 'F'){
            menuIndex--;                             // Decrementar o índice do menu para ir para o menu anterior
            if (menuIndex < 0) {                      // Voltar ao final do menu se cair antes da primeira página do menu (0-4, então 5 páginas)
                menuIndex = 4;                         // Reiniciar para a última página do menu
            }
            displayMenu(menuIndex);                  // Atualizar o display imediatamente após a mudança de menu
        }

        // --- Configuração dos Limites ---
        if (pressed_key == 'A' && lastKey != 'A') { // Se 'A' for pressionado e não foi a última tecla (Definir Limite de Temperatura)
            int tempLimit = readTwoDigitValue("Limite Temp:"); // Solicitar ao usuário para inserir o limite de temperatura
            if (tempLimit != -1) {                      // Se um limite válido for inserido (não cancelado -1)
                Tlim = tempLimit;                         // Atualizar o limite de temperatura
                displayStuff(1, 0, "Temp Salva!");       // Exibir mensagem de confirmação
                __delay_ms(1000);                        // Exibir mensagem por 1 segundo
                menuIndex = 0;                           // Voltar ao menu principal
                displayMenu(menuIndex);                  // Atualizar o display para o menu principal
            } else if (tempLimit == -1) {               // Se o usuário cancelar a entrada
                menuIndex = 0;                           // Voltar ao menu principal
                displayMenu(menuIndex);                  // Atualizar o display para o menu principal
            }
        }

        if (pressed_key == 'B' && lastKey != 'B') { // Se 'B' for pressionado e não foi a última tecla (Definir Limite de Umidade)
            int humLimit = readTwoDigitValue("Limite Hum:"); // Solicitar ao usuário para inserir o limite de umidade
            if (humLimit != -1) {                      // Se um limite válido for inserido
                Hlim = humLimit;                         // Atualizar o limite de umidade
                displayStuff(1, 0, "Humid Salvo!");      // Exibir confirmação
                __delay_ms(1000);                        // Exibir mensagem por 1 segundo
                menuIndex = 0;                           // Voltar ao menu principal
                displayMenu(menuIndex);                  // Atualizar o display para o menu principal
            } else if (humLimit == -1) {               // Se o usuário cancelar a entrada
                menuIndex = 0;                           // Voltar ao menu principal
                displayMenu(menuIndex);                  // Atualizar o display para o menu principal
            }
        }

        if (pressed_key == 'C' && lastKey != 'C') { // Se 'C' for pressionado e não foi a última tecla (Definir Limite de Gás)
            int gasLimit = readTwoDigitValue("Limite Gas"); // Solicitar o limite de gás
            if (gasLimit != -1) {                      // Se um limite válido for inserido
                Glim = gasLimit;                         // Atualizar o limite de gás
                displayStuff(1, 0, "Gas Salvo!");        // Mensagem de confirmação
                __delay_ms(1000);                        // Exibir mensagem por 1 segundo
                menuIndex = 0;                           // Voltar ao menu principal
                displayMenu(menuIndex);                  // Atualizar o display para o menu principal
            } else if (gasLimit == -1) {               // Se o usuário cancelar a entrada
                menuIndex = 0;                           // Voltar ao menu principal
                displayMenu(menuIndex);                  // Atualizar o display para o menu principal
            }
        }

        if (pressed_key == 'D' && lastKey != 'D') { // Se 'D' for pressionado e não foi a última tecla (Resetar Limites)
            Tlim = 30;                                 // Resetar o limite de temperatura para o padrão
            Hlim = 50;                                 // Resetar o limite de umidade para o padrão
            Glim = 20;                                 // Resetar o limite de gás para o padrão
            displayStuff(0, 0, "Lim resetados!");     // Exibir confirmação de reset
            __delay_ms(1000);                        // Exibir mensagem por 1 segundo
            menuIndex = 0;                           // Voltar ao menu principal
            displayMenu(menuIndex);                  // Atualizar o display para o menu principal
        }

        // --- Leituras dos Sensores ---
        currentTemp = tempRead();   // Ler a temperatura atual do sensor
        currentHumid = humidade();   // Ler a umidade atual do sensor
        currentGas = gasRead();     // Ler a concentração de gás atual do sensor

        // --- Lógica de Controle de Alarme e Atuadores ---
        if (currentTemp > Tlim || currentHumid > Hlim || currentGas > Glim) { // Verificar se algum valor do sensor excede seu limite
            buzzer(1);  // Ativar o buzzer se algum limite for excedido (a função buzzer precisa ser implementada)
        } else {
            buzzer(0);  // Desativar o buzzer se todos os valores estiverem dentro dos limites
        }

        if (currentTemp > Tlim) { // Se a temperatura estiver acima do limite
            PORTDbits.RD0 = 1;    // Ligar o LED de alarme de temperatura (RD0)
            PORTCbits.RC2 = 1;    // Ligar o cooler (RC2 - assumindo que RC2 controla o cooler)
        } else if (currentTemp < Tlim - 10) { // Se a temperatura estiver abaixo do limite (com margem de 2 graus)
            PORTDbits.RD0 = 0;    // Desligar o LED de alarme de temperatura
            PORTCbits.RC2 = 0;    // Desligar o cooler
            //PORTCbits.RC1 = 1;    // Ligar o aquecedor (desabilitado por simplicidade)

        } else {
            PORTDbits.RD0 = 0;    // Desligar o LED de alarme de temperatura
            PORTCbits.RC2 = 0;    // Desligar o cooler
            //PORTCbits.RC1 = 0;   // Desligar o aquecedor  (desabilitado por simplicidade)
        }

        if (currentHumid > Hlim) { // Se a umidade estiver acima do limite
            PORTDbits.RD1 = 1;    // Ligar o LED de alarme de umidade (RD1)
            setMoist(1);          // Ativar o umidificador (a função setMoist precisa ser implementada)
        } else {
            PORTDbits.RD1 = 0;    // Desligar o LED de alarme de umidade
            setMoist(0);          // Desativar o umidificador
        }

        if (currentGas > Glim) {   // Se a concentração de gás estiver acima do limite
            PORTDbits.RD2 = 1;    // Ligar o LED de alarme de gás (RD2)
            setCoolerSpeed(255); // Exemplo: Definir o cooler para velocidade máxima como exaustor (a função setCoolerSpeed precisa de implementação PWM)
        } else {
            PORTDbits.RD2 = 0;    // Desligar o LED de alarme de gás
            setCoolerSpeed(0);   // Definir a velocidade do cooler para 0 quando o gás estiver dentro do limite
        }

        lastKey = pressed_key;      // Atualizar a última tecla pressionada para a próxima iteração para detectar eventos de tecla pressionada
    } // Fim do loop principal
} // Fim da função principal

// ------ Definições de Funções ------

// Função para exibir opções de menu no LCD com base no índice do menu
void displayMenu(int menuIndex) {
    char valueStr[10];                   // Buffer de string para armazenar valores inteiros/float para exibição
    static unsigned long lastUpdateTime = 0; // Variável estática para rastrear o tempo da última atualização do display para limitar a taxa
    unsigned long currentTime = millis();     // Obter o tempo atual para limitar a taxa de atualização do display
    float currentTemp, currentHumid, currentGas; // Variáveis locais para armazenar as leituras dos sensores

    // --- Limitar Atualizações do Display ---
    if (currentTime - lastUpdateTime < 500) { // Limitar as atualizações do display a cada 500 milissegundos
        return;                               // Sair da função se não tiver passado tempo suficiente desde a última atualização
    }
    lastUpdateTime = currentTime;               // Atualizar o tempo da última atualização do display

    WriteCmdXLCD(0x01);                     // Limpar o display LCD
    __delay_ms(2);                            // Pequeno atraso após o comando do LCD

    // --- Ler Valores dos Sensores para Exibição ---
    currentTemp = tempRead();                 // Ler a temperatura atual
    currentHumid = humidade();                 // Ler a umidade atual
    currentGas = gasRead();                   // Ler a concentração de gás atual

    // --- Conteúdo do Menu com base no menuIndex ---
    switch (menuIndex) {
        case 0: // Página do Menu 0: Exibição da Temperatura
            displayStuff(0, 0, "Temp:");         // Exibir "Temp:" na primeira linha, primeira coluna
            floatToString(currentTemp, valueStr, 1); // Converter a temperatura para string com 1 casa decimal
            displayStuff(0, 6, valueStr);         // Exibir o valor da temperatura
            displayStuff(0, 11, "C");           // Exibir "C" para unidade Celsius
            displayStuff(1, 0, "Lim:");          // Exibir "Lim:" na segunda linha
            itoa(Tlim, valueStr);                // Converter o limite de temperatura para string
            displayStuff(1, 4, valueStr);         // Exibir o valor do limite de temperatura
            break;

        case 1: // Página do Menu 1: Exibição da Umidade
            displayStuff(0, 0, "Humid:");        // Exibir "Humid:"
            floatToString(currentHumid, valueStr, 1); // Converter a umidade para string com 1 casa decimal
            displayStuff(0, 7, valueStr);         // Exibir o valor da umidade
            displayStuff(0, 12, "%");          // Exibir "%" para unidade de porcentagem
            displayStuff(1, 0, "Lim:");          // Exibir "Lim:"
            itoa(Hlim, valueStr);                // Converter o limite de umidade para string
            displayStuff(1, 4, valueStr);         // Exibir o valor do limite de umidade
            break;

        case 2: // Página do Menu 2: Exibição da Concentração de Gás
            displayStuff(0, 0, "Gas:");          // Exibir "Gas:"
            floatToString(currentGas, valueStr, 1); // Converter a concentração de gás para string com 1 casa decimal
            displayStuff(0, 5, valueStr);         // Exibir o valor da concentração de gás
            displayStuff(0, 11, "ppm");          // Exibir "ppm" para unidade de partes por milhão
            displayStuff(1, 0, "Lim:");          // Exibir "Lim:"
            itoa(Glim, valueStr);                // Converter o limite de gás para string
            displayStuff(1, 4, valueStr);         // Exibir o valor do limite de gás
            break;

        case 3: // Página do Menu 3: Status das Leituras dos Sensores
            displayStuff(0, 0, "Leituras");      // Exibir "Leituras" (Readings em português)
            if (currentTemp <= Tlim && currentHumid <= Hlim && currentGas <= Glim) { // Verificar se todas as leituras estão dentro dos limites
                displayStuff(1, 0, "OK");        // Exibir "OK" se todas estiverem dentro dos limites
            } else { // Se alguma leitura exceder o limite, alternar entre mensagens de erro
                static int errorCycle = 0;        // Variável estática para alternar entre mensagens de erro
                errorCycle++;                      // Incrementar o contador de ciclo de erro

                if (errorCycle > 3) errorCycle = 0; // Reiniciar o ciclo de erro após exibir todos os tipos de erro

                switch (errorCycle) {              // Exibir diferentes mensagens de erro no ciclo
                    case 0:
                        if (currentTemp > Tlim) { // Se o limite de temperatura for excedido
                            displayStuff(1, 0, "T_lim excedida!"); // Exibir "T_lim excedida!" (Limite de temperatura excedido)
                        }
                        break;
                    case 1:
                        if (currentHumid > Hlim) { // Se o limite de umidade for excedido
                            displayStuff(1, 0, "H_lim excedida!"); // Exibir "H_lim excedida!" (Limite de umidade excedido)
                        }
                        break;
                    case 2:
                        if (currentGas > Glim) {   // Se o limite de gás for excedido
                            displayStuff(1, 0, "G_lim excedido!"); // Exibir "G_lim excedido!" (Limite de gás excedido)
                        }
                        break;
                }
            }
            break;

        case 4: // Página do Menu 4: Status do Sistema (Atuadores LIGADO/DESLIGADO)
            displayStuff(0, 0, "Sistemas:");      // Exibir "Sistemas:" (Systems em português)
            if (currentTemp <= Tlim && currentHumid <= Hlim && currentGas <= Glim) { // Verificar se todas as leituras estão dentro dos limites
                displayStuff(1, 0, "OK");        // Exibir "OK" se todos os sistemas estiverem nominais
            } else { // Se algum limite for excedido, alternar entre mensagens de status do sistema (atuadores LIGADO)
                static int systemCycle = 0;       // Variável estática para o ciclo do sistema
                systemCycle++;                     // Incrementar o contador de ciclo do sistema

                if (systemCycle > 3) systemCycle = 0; // Reiniciar o ciclo do sistema após exibir todos os status do sistema

                switch (systemCycle) {             // Alternar entre diferentes status do sistema
                    case 0:
                        if (currentTemp > Tlim) { // Se o limite de temperatura for excedido
                            displayStuff(1, 0, "Resfriador ON"); // Exibir "Resfriador ON" (Cooler LIGADO)
                        }
                        break;
                    case 1:
                        if (currentHumid > Hlim) { // Se o limite de umidade for excedido
                            displayStuff(1, 0, "Humidificador ON"); // Exibir "Humidificador ON" (Umidificador LIGADO)
                        }
                        break;
                    case 2:
                        if (currentGas > Glim) {   // Se o limite de gás for excedido
                            displayStuff(1, 0, "Exaustor ON");     // Exibir "Exaustor ON" (Exaustor LIGADO)
                        }
                        break;
                }
            }
            break;

        default: // Caso padrão para índice de menu inválido
            displayStuff(0, 0, "Menu Inválido");  // Exibir "Menu Inválido" para caso de erro
            break;
    }
} // Fim da função displayMenu

// Função para ler um valor de dois dígitos do teclado, com um prompt no LCD
int readTwoDigitValue(char* prompt) {
    char input[3] = {0};        // Array de caracteres para armazenar dígitos de entrada, terminado em nulo
    int digitCount = 0;         // Contador para o número de dígitos inseridos
    char key;                   // Variável para armazenar a entrada do teclado

    WriteCmdXLCD(0x01);        // Limpar o display LCD
    __delay_ms(2);              // Atraso após o comando do LCD
    WriteCmdXLCD(0x02);        // Retornar o cursor para a posição inicial
    __delay_ms(2);              // Atraso após o comando do LCD

    displayStuff(0, 0, prompt); // Exibir a mensagem de prompt na primeira linha do LCD

    while (digitCount < 2) {    // Loop para ler dois dígitos
        key = readKey();         // Ler uma tecla do teclado

        if (key >= '0' && key <= '9') { // Se a tecla for um dígito numérico
            input[digitCount] = key;     // Armazenar o dígito no array de entrada
            displayStuff(1, digitCount, &input[digitCount]); // Exibir o dígito na segunda linha do LCD
            digitCount++;                // Incrementar o contador de dígitos
            __delay_ms(200);             // Pequeno atraso para feedback visual
        }

        if (key == 'F') {             // Se a tecla 'F' for pressionada (Cancelar)
            return -1;                 // Retornar -1 para indicar cancelamento
        }
    }

    // Converter a string de dois dígitos para um inteiro
    return (input[0] - '0') * 10 + (input[1] - '0'); // Converter dígitos ASCII para valor inteiro
} // Fim da função readTwoDigitValue

// Rotina de Serviço de Interrupção (ISR) - lida com interrupções do Timer 0
void __interrupt() ISR() {
    if (INTCONbits.TMR0IF) { // Verificar se a flag de interrupção do Timer 0 está definida (ocorreu overflow do Timer 0)
        TMR0L = 100;          // Recarregar o byte baixo do Timer 0 para criar overflow de 1ms (ajustar com base no prescaler e clock)
        milliseconds++;       // Incrementar o contador de milissegundos
        if (milliseconds % 1000 == 0) { // Verificar se passou um segundo
            if (tak > 1) {      // Alternar a variável 'tak' a cada segundo (propósito não claro, pode ser para piscar)
                tak = 0;
            } else {
                tak = 1;
            }
        }
        INTCONbits.TMR0IF = 0;  // Limpar a flag de interrupção do Timer 0 para permitir a próxima interrupção
    }
} // Fim da função ISR

// Função para obter a contagem atual de milissegundos
unsigned long millis(void) {
    unsigned long ms;         // Variável local para armazenar o valor de milissegundos

    INTCONbits.GIE = 0;      // Desabilitar interrupções globais para garantir leitura atômica de milissegundos
    ms = milliseconds;       // Ler o valor do contador de milissegundos
    INTCONbits.GIE = 1;      // Reabilitar interrupções globais

    return ms;                // Retornar o valor de milissegundos
} // Fim da função millis

// Função para configurar os pinos de entrada/saída
void configureIO(void) {
    TRISB = 0xF0;           // Configurar PORTB: RB4-RB7 como entradas (linhas do teclado), RB0-RB3 como saídas (colunas do teclado)
    TRISAbits.RA3 = 1;      // Configurar RA3/AN3 como entrada analógica (sensor de umidade)
    TRISAbits.RA0 = 1;      // Configurar RA0/AN0 como entrada analógica (sensor de temperatura)
    TRISAbits.RA2 = 1;      // Configurar RA2/AN2 como entrada analógica (sensor de gás)
    TRISDbits.TRISD0 = 0;   // Configurar RD0 como saída (LED de alarme de temperatura)
    TRISDbits.TRISD1 = 0;   // Configurar RD1 como saída (LED de alarme de umidade)
    TRISCbits.TRISC1 = 0;   // Configurar RC1 como saída (Controle do aquecedor)
    TRISCbits.TRISC2 = 0;   // Configurar RC2 como saída (Controle do cooler)
    PORTDbits.RD0 = 0;        // Inicializar o LED RD0 como desligado
    PORTDbits.RD1 = 0;        // Inicializar o LED RD1 como desligado
    PORTDbits.RD2 = 0;        // Inicializar o LED RD2 como desligado
    PORTCbits.RC1 = 0;          // Inicializar o aquecedor como desligado
    PORTCbits.RC2 = 0;        // Inicializar o pino de controle do cooler como desligado
} // Fim da função configureIO

// Função para configurar o módulo LCD
void configureLCD(void) {
    OpenXLCD(FOUR_BIT & LINES_5X7); // Inicializar o LCD no modo de 4 bits, fonte 5x7 e 2 linhas
    WriteCmdXLCD(0x01);             // Limpar o display LCD
    __delay_ms(10);                 // Atraso após o comando de limpeza
    WriteCmdXLCD(0x02);             // Retornar o cursor para a posição inicial
    __delay_ms(2);                  // Atraso após o comando de retorno
    WriteCmdXLCD(0x0C);             // Ligar o display, cursor desligado, sem piscar
    __delay_ms(2);                  // Atraso após o comando de display
} // Fim da função configureLCD

// Função para configurar interrupções
void configureInterrupt(void) {
    RCONbits.IPEN = 1;          // Habilitar níveis de prioridade de interrupção
    INTCONbits.GIE = 1;         // Habilitar interrupções globais (alta e baixa prioridade)
    INTCONbits.PEIE = 1;        // Habilitar interrupções periféricas (para o Timer 0)

    // Configuração da interrupção do Timer 0
    INTCON2bits.TMR0IP = 1;     // Definir a interrupção do Timer 0 como alta prioridade
    INTCONbits.TMR0IE = 1;      // Habilitar a interrupção do Timer 0
    INTCONbits.TMR0IF = 0;      // Limpar a flag de interrupção do Timer 0

    // Configuração da interrupção do PORTB (não usada efetivamente no código atual para o teclado, a sondagem é usada em vez disso)
    INTCON2bits.RBIP = 0;       // Definir a interrupção de mudança do PORTB como baixa prioridade (não usada ativamente na implementação atual do teclado)
    INTCONbits.RBIE = 0;       // Desabilitar a interrupção de mudança do PORTB (a sondagem é usada para o teclado)
    INTCONbits.RBIF = 0;       // Limpar a flag de interrupção de mudança do PORTB
} // Fim da função configureInterrupt

// Função para configurar o Timer 0 para contagem de milissegundos
void configureTimer(void) {
    T0CON = 0b11000100;         // Configuração do Timer 0:
                                // 11 - Clock interno do ciclo de instrução (FOSC/4)
                                // 00 - Timer/contador de 8 bits
                                // 0100 - Prescaler 1:32 (ajustar o prescaler para overflow de 1ms com clock de 20MHz)
    TMR0L = 100;              // Valor inicial do byte baixo do Timer 0 (para overflow de 1ms - ajustar este valor com base no prescaler)
    INTCONbits.TMR0IF = 0;      // Limpar a flag de interrupção do Timer 0
} // Fim da função configureTimer

// Estrutura de estado de debounce para manipulação do teclado
typedef struct {
    char lastKey;               // Última tecla estável registrada
    char currentKey;            // Tecla atualmente pressionada (durante o debounce)
    unsigned char keyPressCount; // Contagem de teclas pressionadas (não usada diretamente na lógica de debounce aqui)
    unsigned long lastKeyPressTime; // Tempo do último evento de tecla pressionada
    unsigned long lastRepeatTime;   // Tempo da última repetição de tecla (não implementado neste debounce simples)
} KeyState;

// Variável global de estado de debounce
KeyState keyState = {0};

// Função para ler uma tecla pressionada do teclado matricial com debounce e intervalo de pressionamento de tecla
char readKey(void) {
    const int InvCol[COLS] = {3, 2, 1, 0}; // Mapeamento inverso das colunas para o pinout do PORTB (RB3, RB2, RB1, RB0 para colunas)
    static unsigned long lastDebounceTime = 0;  // Variável estática para armazenar o último tempo de debounce
    static unsigned long lastKeyPressTime = 0;  // Variável estática para armazenar o último tempo de tecla válida pressionada
    static char lastStableKey = 0;           // Variável estática para armazenar a última tecla estável detectada
    unsigned long currentTime = millis();      // Obter o tempo atual em milissegundos
    int col, row;                            // Contadores de loop

    for (col = 0; col < COLS; col++) {       // Iterar através de cada coluna do teclado
        LATB = ~(1 << col);                 // Ativar a coluna atual definindo o pino RB correspondente como baixo (colunas são ativas em baixo)
        __delay_ms(5);                      // Pequeno atraso para estabilização do pino da coluna

        for (row = 0; row < ROWS; row++) {   // Iterar através de cada linha do teclado
            if (!(PORTB & (1 << (row + 4)))) { // Verificar se a tecla na linha e coluna atual está pressionada (os pinos das linhas estão em RB4-RB7)
                                                // Tecla pressionada detectada (pino da linha está baixo quando pressionado)
                char pressedKey = keymap[row][InvCol[col]]; // Obter o caractere associado à tecla pressionada a partir do keymap

                if (pressedKey != lastStableKey) { // Se a tecla atualmente pressionada for diferente da última tecla estável
                    lastDebounceTime = currentTime;   // Reiniciar o timer de debounce, pois um novo evento de tecla foi detectado
                    lastStableKey = pressedKey;      // Atualizar a última tecla estável para a tecla atualmente pressionada
                }

                if ((currentTime - lastDebounceTime) > DEBOUNCE_DELAY) { // Verificar se a tecla pressionada está estável durante o tempo de debounce
                    if ((currentTime - lastKeyPressTime) > KEY_WAIT_PERIOD) { // Verificar o intervalo de pressionamento de tecla para evitar entradas repetidas rápidas
                        lastKeyPressTime = currentTime;    // Atualizar o tempo do último evento de tecla pressionada
                        keyState.currentKey = pressedKey;  // Atualizar o estado atual da tecla
                        keyState.lastKey = pressedKey;     // Atualizar o estado da última tecla
                        return pressedKey;                 // Retornar o caractere da tecla detectada e debounced
                    }
                }
                return 0; // A tecla está sendo pressionada, mas está em debounce ou aguardando a liberação da tecla, retornar 0 (nenhum novo evento de tecla)
            } // Fim da detecção de tecla pressionada
        } // Fim do loop das linhas
    } // Fim do loop das colunas

    keyState.lastKey = 0;         // Reiniciar o estado da última tecla quando nenhuma tecla estiver pressionada
    lastStableKey = 0;            // Reiniciar a última tecla estável
    return 0;                     // Nenhuma tecla pressionada detectada ou processada nesta varredura, retornar 0
} // Fim da função readKey

// Função para configurar o módulo ADC
void configureADC(void) {
    ADCON1 = 0b00001011;        // Registrador de Configuração 1 do ADC:
                                // 0000 - VREF+ = VDD, VREF- = VSS (Referências de tensão são VDD e VSS)
                                // 1011 - AN0-AN3 são entradas analógicas, todas as outras são I/O digital
    ADCON2 = 0b10010101;        // Registrador de Configuração 2 do ADC:
                                // 1 - Resultado justificado à direita (ADRESH:ADRESL)
                                // 001 - Tempo de aquisição 2 TAD
                                // 010 - Clock do ADC FOSC/32 (ajustar o clock do ADC para taxa de conversão ideal)
                                // 1 -  TAD = 0.5 us (mín) em Fosc de 20MHz e Fosc/32 = 625kHz de clock do ADC, TAD = 1/625kHz = 1.6us > 0.7us (mín para PIC18F4550)
    ADCON0bits.ADON = 1;        // Habilitar o módulo ADC (ligar o módulo ADC)
} // Fim da função configureADC

// Função para ler o valor analógico de um pino ADC especificado
unsigned int readAnalog(unsigned char pin) {
    unsigned int result = 0;      // Variável para armazenar o resultado do ADC
    if (pin > 12) {              // Validar o número do pino ADC
        return 0;                 // Retornar 0 para número de pino inválido
    }

    ADCON0bits.CHS = pin;         // Selecionar o canal ADC (pino) para conversão (Bits de Seleção de Canal)
    __delay_ms(2);              // Permitir tempo de aquisição do canal (tempo de estabilização para entrada analógica)
    ADCON0bits.GO_DONE = 1;       // Iniciar a conversão do ADC (Bit Go/Done - inicia a conversão)

    while (ADCON0bits.GO_DONE);    // Aguardar a conclusão da conversão do ADC (o bit GO_DONE será limpo pelo hardware quando concluído)

    result = (ADRESH << 8) | ADRESL; // Combinar ADRESH e ADRESL para obter o resultado de 10 bits do ADC (Justificado à direita)
    return result;                 // Retornar o resultado de 10 bits do ADC
} // Fim da função readAnalog

// Função para ler a temperatura do sensor LM35 conectado ao AN0/RA0
float tempRead(void) {
    unsigned int rawAnalog = readAnalog(0); // Ler o valor analógico bruto do AN0 (sensor de temperatura)
    float TempC = ((rawAnalog * 5.0) / 1023.0) / 0.01; // Converter o valor bruto do ADC para temperatura em Celsius:
                                                        // (rawAnalog * Vref / 1023) fornece a tensão, o LM35 fornece 10mV/grau C (0.01V/grau C)
    return TempC;                              // Retornar a temperatura em Celsius
} // Fim da função tempRead

// Função para ler a umidade do sensor conectado ao AN3/RA3 (usando um potenciômetro para simular o sensor de umidade no código original)
float humidade(void) {
    unsigned int rawAnalog = readAnalog(3); // Ler o valor analógico bruto do AN3 (sensor de umidade)
    float Humid = ((rawAnalog * 5.0) / 1023.0) * 20; // Converter o valor bruto do ADC para porcentagem de umidade (0-100%):
                                                        // Supondo que a faixa de tensão de saída do sensor corresponda a 0-100% de umidade linearmente na faixa de 0-5V.
                                                        // Esta conversão pode precisar ser ajustada com base nas características reais do sensor de umidade.
    return Humid;                               // Retornar a porcentagem de umidade
} // Fim da função humidade

// Função para ler a concentração de gás do sensor conectado ao AN2/RA2
float gasRead(void) {
    unsigned int rawAnalog = readAnalog(2); // Ler o valor analógico bruto do AN2 (sensor de gás)
    float gas = ((rawAnalog * 5.0) / 1023.0) * 20;  // Converter o valor bruto do ADC para concentração de gás (em unidade 'ppm' - a unidade é assumida e pode precisar de calibração):
                                                        // Escalonamento linear semelhante ao da umidade, pode precisar de conversão e calibração específicas do sensor.
    return gas;                                 // Retornar o valor da concentração de gás
} // Fim da função gasRead

// Função para simplificar a escrita no LCD na linha e coluna especificadas
void displayStuff(int row, int column, char *stuff) {
    int LCDWritePosition = (row == 0 ? 0x80 : 0xC0) + column; // Calcular o endereço DDRAM do LCD com base na linha e coluna:
                                                                // Linha 0 começa no endereço 0x80, Linha 1 começa em 0xC0

    WriteCmdXLCD(LCDWritePosition);             // Definir a posição do cursor do LCD
    while (*stuff) {                             // Loop através de cada caractere na string de entrada
        WriteDataXLCD(*stuff);                   // Escrever o caractere no registrador de dados do LCD
        stuff++;                                 // Mover para o próximo caractere na string
    }
} // Fim da função displayStuff

// Função de placeholder para definir a velocidade do cooler usando PWM (implementação de PWM necessária)
void setCoolerSpeed(int PWMset) {
    // A implementação do controle PWM para o controle da velocidade do cooler iria aqui
    // O parâmetro PWMset controlaria o ciclo de trabalho ou o valor PWM para definir a velocidade do cooler
} // Fim da função setCoolerSpeed

// Função de placeholder para controlar o umidificador (Controle LIGADO/DESLIGADO - implementação necessária)
void setMoist(int moistSet) {
    if (moistSet == 1) {                        // Se moistSet for 1, ligar o umidificador
        // Código para ligar o umidificador (por exemplo, definir um pino de saída como alto) iria aqui
    } else {                                    // Se moistSet não for 1 (por exemplo, 0), desligar o umidificador
        // Código para desligar o umidificador (por exemplo, definir o pino de controle como baixo) iria aqui
    }
} // Fim da função setMoist

// Função de placeholder para controlar o buzzer (Controle LIGADO/DESLIGADO - implementação necessária)
void buzzer(int buzzerStatus) {
    if (buzzerStatus == 1) {                      // Se buzzerStatus for 1, ativar o buzzer
        // Código para ativar o buzzer (por exemplo, definir um pino de saída como alto para acionar um buzzer) iria aqui
    } else {                                      // Se buzzerStatus não for 1, desativar o buzzer
        // Código para desativar o buzzer (por exemplo, definir o pino de controle do buzzer como baixo) iria aqui
    }
} // Fim da função buzzer

// Função para converter inteiro para string
int itoa(int value, char *ptr) {
    int count = 0, temp;                         // Inicializar variáveis
    char *start = ptr;                           // Armazenar o ponteiro inicial para cálculo de retorno

    if (ptr == NULL)                             // Verificar se o ponteiro de entrada é nulo
        return 0;                                // Retornar 0 se o ponteiro for NULL

    if (value == 0) {                             // Lidar com o caso de valor ser 0
        *ptr = '0';                              // Definir a string como "0"
        ptr[1] = '\0';                           // Terminar a string com nulo
        return 1;                                // Retornar o comprimento da string 1
    }

    if (value < 0) {                             // Lidar com números negativos
        value = -value;                           // Tornar o valor positivo para conversão
        *ptr++ = '-';                             // Adicionar sinal '-' à string
        count++;                                 // Incrementar a contagem para o sinal '-'
        start = ptr;                             // Atualizar o ponteiro inicial para após o sinal '-'
    }

    temp = value;                               // Variável temporária para contar dígitos
    while (temp > 0) {                           // Contar o número de dígitos no valor
        temp /= 10;                             // Divisão inteira para remover o último dígito
        count++;                                 // Incrementar a contagem de dígitos
    }

    ptr[count] = '\0';                          // Terminar a string com nulo

    temp = value;                               // Reiniciar temp para o valor original para conversão de dígitos
    while (temp > 0) {                           // Converter dígitos inteiros para caracteres da direita para a esquerda
        ptr[--count] = temp % 10 + '0';         // Obter o último dígito, converter para char e colocar na string da direita
        temp /= 10;                             // Remover o último dígito
    }

    return count + (start != ptr);              // Retornar o comprimento da string convertida (incluindo o sinal '-' se houver)
} // Fim da função itoa

// Função para converter float para string com precisão especificada
void floatToString(float value, char *buffer, int precision) {
    int intPart;                                 // Parte inteira do float
    int i, index = 0;                            // Contadores de loop e variável de índice
    char intStr[12];                             // Buffer de string temporário para a parte inteira

    if (value < 0) {                             // Lidar com valores negativos
        *buffer++ = '-';                           // Adicionar sinal '-' ao buffer
        value = -value;                           // Tornar o valor positivo para conversão
    }

    intPart = (int)value;                       // Extrair a parte inteira do float
    value -= intPart;                           // Obter a parte fracionária

    if(intPart == 0) {                           // Lidar com o caso da parte inteira ser 0
        intStr[index++] = '0';                   // Armazenar '0' para a parte inteira se o valor for menor que 1
    } else {                                    // Converter a parte inteira para string (semelhante ao itoa para inteiros positivos)
        int temp = intPart;                       // Variável temporária para conversão
        index = 0;                               // Reiniciar o índice para conversão da string inteira
        do {                                    // Converter dígitos da parte inteira para string
            intStr[index++] = (temp % 10) + '0';   // Obter o último dígito e converter para caractere
            temp /= 10;                         // Remover o último dígito
        } while (temp > 0);
    }

    for (i = 0; i < index; i++) {               // Reverter a string inteira (os dígitos foram adicionados em ordem inversa)
        buffer[i] = intStr[index - i - 1];         // Copiar a string inteira invertida para o buffer de saída
    }
    buffer += index;                             // Atualizar o ponteiro do buffer para o final da string inteira

    *buffer++ = '.';                             // Adicionar ponto decimal ao buffer

    for (i = 0; i < precision; i++) {           // Converter a parte fracionária para string até a precisão especificada
        value *= 10;                             // Multiplicar a parte fracionária por 10 para obter o próximo dígito
        int digit = (int)value;                   // Extrair a parte inteira como próximo dígito
        *buffer++ = digit + '0';                 // Converter dígito para caractere e adicionar ao buffer
        value -= digit;                           // Atualizar a parte fracionária para o próximo dígito
    }

    *buffer = '\0';                              // Terminar a string de saída com nulo
} // Fim da função floatToString