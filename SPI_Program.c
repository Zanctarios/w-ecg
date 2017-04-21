/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/cfg/global.h>

//#include "SPI_Functions.h"
#define TASKSTACKSIZE   512

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>

// #include <ti/drivers/Watchdog.h>
#include <ti/drivers/spi/SPICC26XXDMA.h>
#include <ti/drivers/dma/UDMACC26XX.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Board Header files */
#include "Board.h"

#define CSPin Board_DIO21

static PIN_Handle csPinHandle;
static PIN_State csPinState;

Task_Struct spi0TaskStruct;
Char spi0TaskStack[TASKSTACKSIZE];

PIN_Config csPinTable[] = {
		CSPin | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH  | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	    PIN_TERMINATE
};
const char initBuff[] = "Normal mode initialized.\n\r";
const char cirBuff[] = "Circuit mode initialized.\n\r";
const char factBuff[] = "Factory reset done.\n\r";

const String init = "init";
const String inic = "inic";
const String fact = "fact";


int countSize = 2;

#include "SPI_Functions.h"

int hexToDec(char *c)
{
	int n= 0, i=0;
	for(i=0; c[i]!='\0' && c[i]!=0 ; i++){
	            if(c[i]>='0' && c[i]<='9')
	                n=n*16+(c[i]-'0');
	            else if(c[i]=='A' || c[i]=='a')
	                n=n*16+(10);
	            else if(c[i]=='B' || c[i]=='b')
	                n=n*16+(11);
	            else if(c[i]=='C' || c[i]=='c')
	                n=n*16+(12);
	            else if(c[i]=='D' || c[i]=='d')
	                n=n*16+(13);
	            else if(c[i]=='E' || c[i]=='e')
	                n=n*16+(14);
	            else if(c[i]=='F' || c[i]=='f')
	                n=n*16+(15);
	            else{
	                return -1;
	            }
	    }
	return n;
}

void spiTaskFxn(UArg arg0, UArg arg1)
{

		/*Declaration of SPI variables*/
	    SPI_Handle spiHandle;
	    SPI_Params SPI_defParams;
	    UInt spiPeripheralNum = 0;  	/*SPI0*/
	    UInt uartPeripheralNum = 0;  	/*UART0*/

	    /*Declaration of UART variables*/
		UART_Handle uartHandle;
		UART_Params UART_defParams;

		//char regBuff[] = "Register sent      \n\r";
		bool normalMode = true;
		bool proceed = true;
	    /* SPI Parameters initialization */
	    SPI_Params_init(&SPI_defParams);
	    SPI_defParams.dataSize = 8;
	    SPI_defParams.frameFormat = SPI_POL0_PHA0;
	    SPI_defParams.bitRate = 4000000;

	    /*UART Parameters initialization*/
	    UART_Params_init(&UART_defParams); //baudRate = 115200; dataLength = 8; stopBits = ONE; No Parity

		spiHandle = SPI_open(spiPeripheralNum, &SPI_defParams);
		uartHandle = UART_open(uartPeripheralNum,&UART_defParams);

		if(!uartHandle || !spiHandle)
		{
			System_printf("Error opening UART or SPI");
		}

		while(1)
		{
			char rxBuffer[5] = {NULL};
			UART_read(uartHandle,rxBuffer,4);
			//UART_write(uartHandle,rxBuffer,sizeof(rxBuffer));

			if(strcmp(rxBuffer,"init") == 0)
			{
				initSequence(spiHandle);
				UART_write(uartHandle,initBuff,sizeof(initBuff));
				normalMode = true;
			}
			else if(strcmp(rxBuffer,"inic") == 0)
			{
				initSequenceCircuit(spiHandle);
				UART_write(uartHandle,cirBuff,sizeof(cirBuff));
				normalMode = false;

			}
			else if(strcmp(rxBuffer,"fact") == 0)
			{
				factoryReset(spiHandle);
				UART_write(uartHandle,factBuff,sizeof(factBuff));
			}
			else if(strcmp(rxBuffer,"cont") == 0)
			{
				while(proceed)
				{
					readRegisterContinuous(0xD0,spiHandle, uartHandle);
				}
			}
			else
			{
				if(normalMode)
					readRegister(hexToDec(rxBuffer),spiHandle, uartHandle);
				else
					readRegCirc(hexToDec(rxBuffer),spiHandle, uartHandle);
			}
		}


	   /* SPI_close(spiHandle);
	    System_flush();*/
}

void spiTaskInit()
{
		Board_initSPI();
		Board_initUART();

	    //SPI Task vars
	    Task_Params spiTaskParams;
	    Error_Block errBlock;

	    //SPI Task init
	    Task_Params_init(&spiTaskParams);
	    Error_init(&errBlock);
	    spiTaskParams.stackSize = TASKSTACKSIZE;
	    spiTaskParams.stack = &spi0TaskStack;
	    spiTaskParams.instance->name = "SPITask";

	    Task_construct(&spi0TaskStruct, (Task_FuncPtr)spiTaskFxn,&spiTaskParams,&errBlock);
}

/*  ======== main ========
 */

int main(void)
{
    /* Call board init functions */
    Board_initGeneral();

    csPinHandle = PIN_open(&csPinState, csPinTable);
	if(!csPinHandle) {
			System_abort("Error initializing board LED pins\n");
	 }

    /*Start SPI Task Initialization (Board, Parameters, etc) */
    spiTaskInit();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
