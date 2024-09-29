/* CAN2 tx  DRIVER.C*/

#include"header.h"
extern CAN2 v3;
extern u32 rx_flag;

void en_can2_interrupt(void)
{
	C2IER = 1;
}


void can2_init(void)
{
	VPBDIV = 1;
	PINSEL1 |= 0x14000;  //po.23-->RD2, po.24-->TD2
	C2MOD = 1; // SELECT RESET MODE
	C2BTR = 0X001C001D;//SET 125KPBS BAUD RATE
	AFMR = 2; //ACCEPT ALL RECIEVED MESSAGE
	C2MOD = 0; //RELEASE RESET MODE
}


void can2_tx(CAN2 v)
{
	C2TID1 = v.id;
	C2TFI1 = v.dlc<<16; // set dlc
	if(v.rtr==0) //if data frame
	{
		C2TDA1 = v.byteA;
		C2TDB1 = v.byteB;
	}
	else
		C2TFI1 |= (1<<30); //SET RTR =1
	C2CMR = 1 | (1<<5);      //START TRANSMISSION AND SELECT TXBUFF

	while(TCS==0); //TCS 
}

