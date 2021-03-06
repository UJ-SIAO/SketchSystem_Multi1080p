/*
 * vdmaTest.c
 *
 *  Created on: Apr 9, 2020
 *      Author: VIPIN
 */
#include "xparameters.h"
#include "xaxivdma.h"
#include "xaxidma.h"
#include "xscugic.h"
#include <stdlib.h>
#include <stdio.h>
#include "imageProcess.h"
#include "A.h"
#include "C.h"
#include "D.h"
#include "E.h"
#include "Y.h"
#include "R.h"
#include "U.h"
#include "W.h"
#include "sleep.h"

#define HSize 1920
#define VSize 1080
#define FrameSize HSize*VSize*3

#define imgHSize 1920
#define imgVSize 1080
#define imageSize 1920*1080


//char imageData[] = {
//static void imageProcISR2(void *CallBackRef);
int initIntrController(XScuGic *Intc);
static int SetupVideoIntrSystem(XAxiVdma *AxiVdmaPtr, u16 ReadIntrId, XScuGic *Intc);

char Buffer[FrameSize];

int main(){
	XScuGic Intc;
	initIntrController(&Intc);
	int status;
	imgProcess myImgProcess;
	char *filteredImage;
	char *filteredImage1;
	char *filteredImage2;
	filteredImage  = malloc(sizeof(char)*(imageSize) * 4);
	//filteredImage1 = malloc(sizeof(char)*(imageSize));
	filteredImage2 = malloc(sizeof(char)*(imageSize) * 4);
	//Y?BW?B
	myImgProcess.imageDataPointer1 = imageDataA;
	myImgProcess.imageDataPointer2 = imageDataC;
	myImgProcess.imageDataPointer3 = imageDataD;
	myImgProcess.imageDataPointer4 = imageDataE;
	myImgProcess.imageDataPointer5 = imageDataY;
	myImgProcess.imageDataPointer6 = imageDataR;
	myImgProcess.imageDataPointer7 = imageDataU;
	myImgProcess.imageDataPointer8 = imageDataW;
	myImgProcess.imageHSize = imgHSize;
	myImgProcess.imageVSize = imgVSize;
	myImgProcess.filteredImageDataPointer  = filteredImage;
	myImgProcess.filteredImageDataPointer2 = filteredImage2;

	initImgProcessSystem(&myImgProcess, (u32)XPAR_AXI_DMA_0_BASEADDR, &Intc);
	//print("aaa\n");
	startImageProcessing(&myImgProcess);
	//print("bbb\n");

    while(myImgProcess.done == 0){
    	//print("fff\n");
    }

    sleep(1);
    myImgProcess.row =4;
    status=XAxiDma_SimpleTransfer(myImgProcess.DmaCtrlPointer,(u32)myImgProcess.filteredImageDataPointer2,(1920*1080*4),XAXIDMA_DEVICE_TO_DMA);
    if(status != XST_SUCCESS){
		xil_printf("DMA Receive Failed with Status %d\n",status);
		return -1;
	}
    status=XAxiDma_SimpleTransfer(myImgProcess.DmaCtrlPointer,(u32)myImgProcess.imageDataPointer5, 4*1920,XAXIDMA_DMA_TO_DEVICE);
    if(status != XST_SUCCESS){
		xil_printf("DMA Transfer Failed with Status0 %d\n",status);
		return -1;
	}
    while(myImgProcess.done == 1){
    	//xil_printf("row = %d \r\n",myImgProcess.row);
    	//xil_printf("done = %d \r\n",myImgProcess.done);
    }
    //XScuGic_Disable(myImgProcess.IntrCtrlPointer,XPAR_FABRIC_SKETCHIP_1080P_0_O_INTR_INTR);
    /*sleep(1);
    myImgProcess.row =4;
    status=XAxiDma_SimpleTransfer(myImgProcess.DmaCtrlPointer,(u32)myImgProcess.filteredImageDataPointer3,(1920*1080),XAXIDMA_DEVICE_TO_DMA);
    if(status != XST_SUCCESS){
		xil_printf("DMA Receive Failed with Status %d\n",status);
		return -1;
	}
	status=XAxiDma_SimpleTransfer(myImgProcess.DmaCtrlPointer,(u32)myImgProcess.imageDataPointer4, 4*1920,XAXIDMA_DMA_TO_DEVICE);
	if(status != XST_SUCCESS){
		xil_printf("DMA Receive Failed with Status %d\n",status);
		return -1;
	}
	//xil_printf("hh\r\n");
	while(myImgProcess.done == 2){
		//xil_printf("row2 = %d \r\n",myImgProcess.row);
		//xil_printf("done = %d \r\n",myImgProcess.done);
	}*/
	//xil_printf("hh\r\n");
	int Index;
	int choice;
	u32 Addr1;

	XAxiVdma myVDMA;
	XAxiVdma_Config *config = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
	XAxiVdma_DmaSetup ReadCfg;
	status = XAxiVdma_CfgInitialize(&myVDMA, config, config->BaseAddress);
    if(status != XST_SUCCESS){
    	xil_printf("DMA Initialization failed");
    }
    ReadCfg.VertSizeInput = VSize;
    ReadCfg.HoriSizeInput = HSize*3;
    ReadCfg.Stride = HSize*3;
    ReadCfg.FrameDelay = 0;
    ReadCfg.EnableCircularBuf = 1;
    ReadCfg.EnableSync = 1;
    ReadCfg.PointNum = 0;
    ReadCfg.EnableFrameCounter = 0;
    ReadCfg.FixedFrameStoreAddr = 0;

    status = XAxiVdma_DmaConfig(&myVDMA, XAXIVDMA_READ, &ReadCfg);
    if (status != XST_SUCCESS) {
    	xil_printf("Write channel config failed %d\r\n", status);
    	return status;
    }

    xil_printf("MaxNumFrames = %d \r\n",myVDMA.MaxNumFrames);

    Addr1 = (u32)&(Buffer[0]);

    //print("h1\r\n");
	for(Index = 0; Index < myVDMA.MaxNumFrames; Index++) {
		ReadCfg.FrameStoreStartAddr[Index] = Addr1;
		// print("h1\r\n");
		Addr1 +=  FrameSize;
	}
	//print("h2\r\n");
	status = XAxiVdma_DmaSetBufferAddr(&myVDMA, XAXIVDMA_READ,ReadCfg.FrameStoreStartAddr);
	if (status != XST_SUCCESS) {
		xil_printf("Read channel set buffer address failed %d\r\n", status);
		return XST_FAILURE;
	}
	//print("h3\r\n");
	XAxiVdma_IntrEnable(&myVDMA, XAXIVDMA_IXR_COMPLETION_MASK, XAXIVDMA_READ);
	SetupVideoIntrSystem(&myVDMA, XPAR_FABRIC_AXI_VDMA_0_MM2S_INTROUT_INTR,&Intc);
	//print("h4\r\n");
	status = XAxiVdma_DmaStart(&myVDMA,XAXIVDMA_READ);
	if (status != XST_SUCCESS) {
		if(status == XST_VDMA_MISMATCH_ERROR)
			xil_printf("DMA Mismatch Error\r\n");
		return XST_FAILURE;
	}
	//print("h5\r\n");
	choice=0;
    while(1){
    	/*xil_printf("Enter your choice\n\r1.Original Image\n\r2.Sketched image\n\r");
    	xil_printf("3.Original Image\n\r4.Sketched image\n\r");
    	scanf("%d",&choice);*/
    	//print("h\r\n");
    	sleep(1);
    	if(choice<16)
    		choice++;
    	else
    		choice=1;
    	switch(choice){
    	case 1:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,imageDataA,Buffer);
    		break;
    	case 2:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,filteredImage,Buffer);
			break;
    	case 3:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,imageDataC,Buffer);
			break;
    	case 4:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,filteredImage + (imageSize),Buffer);
			break;
    	case 5:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,imageDataD,Buffer);
			break;
    	case 6:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,filteredImage + (imageSize*2),Buffer);
			break;
    	case 7:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,imageDataE,Buffer);
			break;
    	case 8:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,filteredImage + (imageSize*3),Buffer);
			break;
    	case 9:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,imageDataY,Buffer);
			break;
    	case 10:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,filteredImage2,Buffer);
			break;
    	case 11:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,imageDataR,Buffer);
			break;
    	case 12:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,filteredImage2+ (imageSize)-5,Buffer);
			break;
    	case 13:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,imageDataU,Buffer);
			break;
    	case 14:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,filteredImage2+ (imageSize*2)-5,Buffer);
			break;
    	case 15:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,imageDataW,Buffer);
			break;
    	case 16:
    		drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,1,filteredImage2+ (imageSize*3)-5,Buffer);
			break;


    	default:
    		xil_printf("Wrong choice\r\n");
    		break;
    	}
    	sleep(1);
    }
}


/*****************************************************************************/
 /* Call back function for read channel
******************************************************************************/

static void ReadCallBack(void *CallbackRef, u32 Mask)
{
	/* User can add his code in this call back function */
	//xil_printf("Read Call back function is called\r\n");
}

/*****************************************************************************/
/*
 * The user can put his code that should get executed when this
 * call back happens.
 *
*
******************************************************************************/
static void ReadErrorCallBack(void *CallbackRef, u32 Mask)
{
	/* User can add his code in this call back function */
	xil_printf("Read Call back Error function is called\r\n");

}

int initIntrController(XScuGic *IntcInstancePtr){
	int Status;
	XScuGic_Config *IntcConfig;
	IntcConfig = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);
	Status =  XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig, IntcConfig->CpuBaseAddress);
	if(Status != XST_SUCCESS){
		xil_printf("Interrupt controller initialization failed..");
		return -1;
	}

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler,(void *)IntcInstancePtr);
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}


static int SetupVideoIntrSystem(XAxiVdma *AxiVdmaPtr, u16 ReadIntrId, XScuGic *Intc)
{
	int Status;
	XScuGic *IntcInstancePtr = Intc;

	Status = XScuGic_Connect(IntcInstancePtr,ReadIntrId,(Xil_InterruptHandler)XAxiVdma_ReadIntrHandler,(void *)AxiVdmaPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed read channel connect intc %d\r\n", Status);
		return XST_FAILURE;
	}

	XScuGic_Enable(IntcInstancePtr,ReadIntrId);

	XAxiVdma_SetCallBack(AxiVdmaPtr, XAXIVDMA_HANDLER_GENERAL, ReadCallBack, (void *)AxiVdmaPtr, XAXIVDMA_READ);

	XAxiVdma_SetCallBack(AxiVdmaPtr, XAXIVDMA_HANDLER_ERROR, ReadErrorCallBack, (void *)AxiVdmaPtr, XAXIVDMA_READ);

	return XST_SUCCESS;
}


