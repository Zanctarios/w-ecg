/*
 * Functions.h
 *
 *  Created on: Mar 15, 2017
 *      Author: Lappy
 */

#ifndef SPI_FUNCTIONS_H_
#define SPI_FUNCTIONS_H_

/*First bit 0 is write operation.
 * Next 7 bits are the register address.
 * Last 8 bits are the data to be written on the register.
*/


void writeRegister(uint8_t address, uint8_t value, SPI_Handle spiHandle)
{

	SPI_Transaction spiTransaction;
	uint8_t transmit[] = {address & 0x7F, value};    //Value is 2A if AA is input. 42 in Decimal.
	//uint8_t transmit[] = address;
	spiTransaction.count = 2;
	spiTransaction.txBuf = transmit;
	spiTransaction.rxBuf = NULL;
	PIN_setOutputValue(csPinHandle, CSPin, 0);
	SPI_transfer(spiHandle, &spiTransaction);
	PIN_setOutputValue(csPinHandle, CSPin, 1);
}

/*First bit 1 is read operation.
 * Next 7 bits are the register address.
 * Last 8 bits are the data to be written on the register.
*/

void readRegister(uint8_t address, SPI_Handle spiHandle, UART_Handle uartHandle)
{
	countSize = 2;
	SPI_Transaction spiTransaction;
	uint8_t receiveBuf[10] = {NULL};
	uint8_t completeAddress[10] = {NULL};
	completeAddress[0] = address;
	spiTransaction.count = countSize;
	spiTransaction.txBuf = completeAddress;
	spiTransaction.rxBuf = receiveBuf;
	PIN_setOutputValue(csPinHandle, CSPin, 0);
	SPI_transfer(spiHandle, &spiTransaction);
	PIN_setOutputValue(csPinHandle, CSPin, 1);
	UART_write(uartHandle,receiveBuf,sizeof(receiveBuf));
}

void readRegCirc(uint8_t address, SPI_Handle spiHandle, UART_Handle uartHandle)
{
	countSize = 7;
	SPI_Transaction spiTransaction;
	uint8_t receiveBuf[] = {};
	uint8_t completeAddress[7] = {NULL};
	completeAddress[0] = address;
	spiTransaction.count = countSize;
	spiTransaction.txBuf = completeAddress;
	spiTransaction.rxBuf = receiveBuf;
	PIN_setOutputValue(csPinHandle, CSPin, 0);
	SPI_transfer(spiHandle, &spiTransaction);
	PIN_setOutputValue(csPinHandle, CSPin, 1);
	UART_write(uartHandle,receiveBuf,sizeof(receiveBuf));
}

void initSequence(SPI_Handle spiHandle)
{
	writeRegister(0x00,0x00,spiHandle);
	writeRegister(0x01,0x11,spiHandle);
	writeRegister(0x02,0x19,spiHandle);
	writeRegister(0x0A,0x07,spiHandle);
	writeRegister(0x0C,0x04,spiHandle);
	writeRegister(0x12,0x04,spiHandle);		//Lol
	writeRegister(0x14,0x24,spiHandle);
	writeRegister(0x21,0x02,spiHandle);
	writeRegister(0x22,0x02,spiHandle);
	writeRegister(0x23,0x02,spiHandle);
	//writeRegister(0x26,0x04,spiHandle);
	writeRegister(0x27,0x08,spiHandle);
	writeRegister(0x2F,0x31,spiHandle);
	writeRegister(0x00,0x01,spiHandle);

	System_printf("Initialization registers written.\n");
}

void initSequenceCircuit(SPI_Handle spiHandle)
{
	writeRegister(0x00,0x00,spiHandle);
	writeRegister(0x01,0x11,spiHandle);
	writeRegister(0x0A,0x03,spiHandle);
	writeRegister(0x0C,0x03,spiHandle);
	writeRegister(0x12,0x04,spiHandle);
	writeRegister(0x14,0x39,spiHandle);
	writeRegister(0x21,0x02,spiHandle);
	writeRegister(0x22,0x02,spiHandle);
	writeRegister(0x27,0x08,spiHandle);
	writeRegister(0x2F,0x11,spiHandle);
	writeRegister(0x00,0x01,spiHandle);

	System_printf("Initialization registers written.\n");
}

void factoryReset(SPI_Handle spiHandle)
{
	writeRegister(0x00,0x02,spiHandle);
	writeRegister(0x01,0x00,spiHandle);
	writeRegister(0x02,0x00,spiHandle);
	writeRegister(0x03,0x00,spiHandle);
	writeRegister(0x04,0x00,spiHandle);
	writeRegister(0x05,0x00,spiHandle);
	writeRegister(0x06,0x08,spiHandle);
	writeRegister(0x07,0x00,spiHandle);
	writeRegister(0x08,0x00,spiHandle);
	writeRegister(0x09,0x00,spiHandle);
	writeRegister(0x0A,0x00,spiHandle);
	writeRegister(0x0B,0x00,spiHandle);
	writeRegister(0x0C,0x00,spiHandle);
	writeRegister(0x0D,0x00,spiHandle);
	writeRegister(0x0E,0x00,spiHandle);
	writeRegister(0x0F,0x00,spiHandle);
	writeRegister(0x10,0x00,spiHandle);
	writeRegister(0x11,0x00,spiHandle);
	writeRegister(0x12,0x00,spiHandle);
	writeRegister(0x13,0x00,spiHandle);
	writeRegister(0x14,0x00,spiHandle);
	writeRegister(0x15,0x00,spiHandle);
	writeRegister(0x16,0x00,spiHandle);
	writeRegister(0x17,0x01,spiHandle);
	writeRegister(0x1E,0x00,spiHandle);
	writeRegister(0x1F,0x03,spiHandle);
	writeRegister(0x21,0x08,spiHandle);
	writeRegister(0x22,0x80,spiHandle);
	writeRegister(0x23,0x80,spiHandle);
	writeRegister(0x24,0x80,spiHandle);
	writeRegister(0x25,0x00,spiHandle);
	writeRegister(0x26,0x00,spiHandle);
	writeRegister(0x27,0x00,spiHandle);
	writeRegister(0x28,0x40,spiHandle);
	writeRegister(0x29,0x00,spiHandle);
	writeRegister(0x2A,0x00,spiHandle);
	writeRegister(0x2B,0x00,spiHandle);
	writeRegister(0x2C,0x00,spiHandle);
	writeRegister(0x2D,0x09,spiHandle);
	writeRegister(0x2E,0x33,spiHandle);
	writeRegister(0x2F,0x00,spiHandle);
	writeRegister(0x40,0x01,spiHandle);

	System_printf("Initialization registers written.\n");
}

#endif /* SPI_FUNCTIONS_H_ */
