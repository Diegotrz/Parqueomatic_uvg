//Librerías
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/adc.h"
#include "utils/ustdlib.h"

#define XTAL 16000000

//Variables
int sens1;
int sens2;
int sens3;
int sens4;

char Ocupados;

//Prototipos
void setup(void);

//Loop
int main(void){
    setup();
    while (1){
        if (GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0) == 0){                                 //Sensor1 = 0,
            sens1 = 1;                                                                      //Ocupado
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2);                          //Encender led rojo
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);                                   //Apagar led verde
        }
        else {
            sens1 = 0;                                                                      //Desocupado
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0);                                   //Apagar led rojo
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);                          //Encender led verde
        }

        if (GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1) == 0){                                 //Sensor2 = 0,
            sens2 = 1;                                                                      //Ocupado
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4);                          //Encender led rojo
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0);                                   //Apagar led verde
        }
        else {
            sens2 = 0;                                                                      //Desocupado
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0);                                   //Apagar led rojo
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_PIN_6);                          //Encender led verde
        }

        if (GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2) == 0){                                 //Sensor3 = 0,
            sens3 = 1;                                                                      //Ocupado
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_PIN_7);                          //Encender led rojo
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);                                   //Apagar led verde
        }
        else {
            sens3 = 0;                                                                      //Desocupado
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0);                                   //Apagar led rojo
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);                          //Encender led verde
        }

        if (GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3) == 0){                                 //Sensor4 = 0,
            sens4 = 1;                                                                      //Ocupado
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0);                          //Encender led rojo
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0);                                   //Apagar led verde
        }
        else {
            sens4 = 0;                                                                      //Desocupado
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);                                   //Apagar led rojo
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);                          //Encender led verde
        }

        sens2 = sens2 << 1;
        sens3 = sens3 << 2;
        sens4 = sens4 << 3;
        Ocupados = sens1 | sens2 | sens3 | sens4;                                           //Juntar bits en el char, con el sens4 como msb
        UARTCharPutNonBlocking(UART1_BASE, Ocupados);                                       //Enviar al ESP32
    }
}

void setup(void){
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);         //Reloj a 40MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);                                                    //Habilitar el puerto A
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));                                             //Esperar a que termine
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);                                                    //Habilitar el puerto B
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));                                             //Esperar a que termine
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);                                                    //Habilitar el puerto D
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));                                             //Esperar a que termine
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);                                                    //Habilitar el puerto E
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));                                             //Esperar a que termine
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);                                                    //Habilitar el puerto F
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));                                             //Esperar a que termine
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);       //D0, D1, D2, D3 (sensores) entradas
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_3);                   //A2, A4 (rojo), A3 (verde) salidas
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3);      //B7 (rojo), B6, B2, B3 (verde) salidas
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);                                             //E0 (rojo) salidas
    IntMasterEnable();                                                                              //Habilitar interrupciones

    //UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);                                                    //Habilitar UART1
    GPIOPinConfigure(GPIO_PB0_U1RX);                                                                //B0 como RX
    GPIOPinConfigure(GPIO_PB1_U1TX);                                                                //B1 como TX
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);                                      //Habilitar los pines para el UART
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE)); //Setear el UART1 con 115200 de baudrate, de 8 bits, de un dato y sin paridad
    UARTEnable(UART1_BASE);                                                                         //Iniciar UART1
}
