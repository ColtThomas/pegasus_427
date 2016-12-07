#include <stdio.h>
#include <stdint.h>
#include "dma_magic.h"
#include "xparameters.h"
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
void print(char *str);

int main()
{
	uint32_t source_word = 0xDEADBEEF;
	uint32_t destination_word = 0x0;
	uint32_t next_word = 0xDEEDEEDE;
	uint32_t length = 16; // each 4 bits make one hex digit.
	uint32_t response;
    init_platform();
    print("Hello World\n\r");
    cleanup_platform(); // THIS ALREADY CLEARS THE CACHES

    // enable the interrupt
    XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
    			(XPAR_DMA_MAGIC_0_BASEADDR |XPAR_ARDUINO_0_BASEADDR | XPAR_PITIFUL_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK | XPAR_AXI_AC97_0_INTERRUPT_MASK));
    	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);

    // test the value register. Let reg0 be the value register.
    DMA_MAGIC_mWriteSlaveReg0(XPAR_DMA_MAGIC_0_BASEADDR, DMA_MAGIC_SLV_REG0_OFFSET,length );
    printf("Printing null value in the value register.\n\r");
            xil_printf("%x\r\n", response);
    response = DMA_MAGIC_mReadSlaveReg0(XPAR_DMA_MAGIC_0_BASEADDR,DMA_MAGIC_SLV_REG0_OFFSET);

    printf("Printing value in the value register.\n\r");
        xil_printf("%x\r\n", response);

    printf("Printing value before DMA transfer.\n\r");
    xil_printf("%x\r\n", destination_word);

    DMA_MAGIC_MasterRecvWord(XPAR_DMA_MAGIC_0_BASEADDR, (Xuint32) &source_word);
//    DMA_MAGIC_MasterRecvWord(XPAR_DMA_MAGIC_0_BASEADDR, (Xuint32) &next_word);
    DMA_MAGIC_MasterSendWord(XPAR_DMA_MAGIC_0_BASEADDR, (Xuint32) &destination_word);
//
    printf("Printing value after DMA transfer.\n\r");
    xil_printf("%x\r\n", destination_word);
//
//    DMA_MAGIC_MasterSendWord(XPAR_DMA_MAGIC_0_BASEADDR, (Xuint32) &destination_word);
//
//    printf("Printing value after DMA transfer.\n\r");
//    xil_printf("%x\r\n", destination_word);
//    cleanup_platform();

    return 0;
}
