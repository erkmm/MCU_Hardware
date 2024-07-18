#include <stdio.h>
#include "NUC029xAN.h"
//#define RXBUFSIZE 128

//uint8_t datasize[RXBUFSIZE] = {0};
//
//volatile uint32_t karsayi = 0;
//volatile uint32_t baslangic = 0;
//volatile uint32_t kuyruk = 0;
uint32_t TxBuf = 0;
uint32_t RxBuf = 0;

void SerialPortInit() {
	SYS->IPRSTC2 |= SYS_IPRSTC2_UART0_RST_Msk;
	SYS->IPRSTC2 &= ~SYS_IPRSTC2_UART0_RST_Msk;

	UART0->BAUD = 0 | UART_BAUD_MODE2 | UART_BAUD_MODE2_DIVIDER(__HIRC, 115200);

	UART0->LCR = 0 | UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1; //5609

	UART0->IER = 0 | UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk;

}

void SerialPortTask() {
//	uint32_t u32delayno = 0;
//
//	while (RxBuf == 1) {
//		u32delayno++;
//		if (u32delayno >= 10000000)
//		{
//			UART0->RBR = 1<<1;
//			RxBuf=0;
//	}}
	//		SerialPortReceiverGetByte();
	//		SerialPortReceiverClear();
	//		SerialPortTransmitterWriteByte();
	//		SerialPortTransmitterSend();
}

uint32_t SerialPortIsReceived(UART_T *uart) {
	return ((uart)->FSR & UART_FSR_RX_EMPTY_Msk); /* Check RX empty */

}

uint32_t SerialPortReceiverGetByte() {
	uint32_t u32delayno = 0;

	while (SerialPortIsReceived(UART0)) /* Check RX empty */
	{
		u32delayno++;
		if (u32delayno >= 0x40000000)
			return FALSE;
	}
	RxBuf = UART0->RBR; /* Get Data from UART RX  */
	return 1;
}

void SerialPortReceiverClear() {
	UART0->FCR |= UART_FCR_RFR_Msk;
}

uint32_t SerialPortTransmitterReadyToSend(UART_T *uart) {
	return ((uart->FSR & UART_FSR_TE_FLAG_Msk) == 0); /* Wait Tx empty and Time-out manner */
}

uint32_t SerialPortTransmitterWriteByte() {
	uint32_t u32delayno = 0;

	while (SerialPortTransmitterReadyToSend(UART0)) /* Wait Tx empty and Time-out manner */
	{
		u32delayno++;
		if (u32delayno >= 0x40000000)
			return FALSE;
	}
	TxBuf = RxBuf;

	return 1;
}

uint32_t SerialPortTransmitterSend() {
	uint32_t u32delayno = 0;

	while (!SerialPortTransmitterWriteByte()) {
		u32delayno++;
		if (u32delayno >= 0x40000000)
			return FALSE;
	}
	UART0->THR = TxBuf; /* Send UART Data from buffer */
	//RxBuf = 1;
	return 1;
}

//
//void StartUARTTransmission(uint16_t adcValue)
//{
//    // Convert ADC value to string
//    char buffer[10];
//    sprintf(buffer, "%d\r\n", adcValue);
//
//    // Enable UART TX interrupt
//    UART0->IER |= UART_IER_THRE_IEN_Msk;
//
//    // Start sending the first byte
//    if(!(UART0->FSR & UART_FSR_TX_FULL_Msk))
//    {
//        UART0->THR = buffer[0];
//    }
//}

//
//volatile uint8_t txIndex = 0;
//volatile char txBuffer[10];
//
//void UART0_IRQHandler(void)
//{
//    if(UART0->ISR & UART_ISR_THRE_INT_Msk)
//    {
//        if(txBuffer[txIndex] != '\0')
//        {
//            UART0->THR = txBuffer[txIndex++];
//        }
//        else
//        {
//            // Transmission complete, disable UART TX interrupt
//            UART0->IER &= ~UART_IER_THRE_IEN_Msk;
//            txIndex = 0;
//        }
//    }
//}
//void UART0_IRQHandler(void){
//    uint32_t newchar = 0xFF;
//    uint32_t flag = UART0->ISR;
//
//    if (flag & UART_ISR_RDA_INT_Msk) {
//        while (UART0->ISR & UART_ISR_RDA_IF_Msk) {
//            newchar = UART0->RBR;
//            printf("%c", newchar);
//
//            if (karsayi < RXBUFSIZE) {
//                datasize[kuyruk] = newchar;
//                kuyruk = (kuyruk == (RXBUFSIZE - 1)) ? 0 : (kuyruk + 1);
//                karsayi++;
//                if (newchar == '\n' || newchar == '\r') {
//                    UART0->IER |= UART_IER_THRE_IEN_Msk;
//                }
//            }
//        }
//        UART0->ISR = UART_ISR_RDA_IF_Msk;
//    }
//
//    if (flag & UART_ISR_THRE_INT_Msk) {
//        while (baslangic != kuyruk && !(UART0->FSR & UART_FSR_TX_FULL_Msk)) {
//            newchar = datasize[baslangic];
//            UART0->THR = newchar;
//            baslangic = (baslangic == (RXBUFSIZE - 1)) ? 0 : (baslangic + 1);
//            karsayi--;
//            if (baslangic == kuyruk) {
//                UART0->IER &= ~UART_IER_THRE_IEN_Msk;
//                break;
//            }
//        }
//        UART0->ISR = UART_ISR_THRE_IF_Msk;
//    }
//}
