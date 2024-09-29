/* reverse parking assistance in the car  main function*/

#include"header.h"

CAN2 v1 = {0xAA,0,0,1,4};
CAN2 v2 = {0xAA,0,0,0,4};
CAN2 v3;
u32 tx_flag = 0,rx_flag=0;


__irq void EINT0_Handler(void)
{
	tx_flag =1;
	EXTINT = 1;
	VICVectAddr = 0;

}

__irq void CAN2_Rx_Handler(void)
{
	v3.id = C2RID;
	v3.dlc = (C2RFS)&0xF;
	v3.rtr = (C2RFS>>30)&1;
	if(v3.rtr==0)
	{
		v3.byteA = C2RDA;
		v3.byteB = C2RDB;
	}
	C2CMR = (1<<2);
	rx_flag = 1;
	VICVectAddr = 0;
}


void config_vic(void)
{
	VICIntSelect = 0;
	VICVectAddr0 = (u32)EINT0_Handler;
	VICVectCntl0 = 14 | (1<<5);
	VICVectAddr1 = (u32)CAN2_Rx_Handler;
	VICVectCntl1 = 27 | (1<<5);
	VICIntEnable = ((1<<14)|(1<<27));
}

void en_eint0(void)
{
	PINSEL1 |=1;
	EXTMODE = 1;
	EXTPOLAR = 0;
}

void buzzer_beep(u32 t)
{
	int a[] = {15,60,30,15,15};
	u32 pclk = a[VPBDIV]*1000;
	T0PC = 0;
	T0PR = pclk - 1;
	T0TC = 0;
	T0TCR = 1;
	while(t>T0TC)
	{
		IOSET0 = buzzer;
		IOCLR0 = LED1;
	}
	T0TCR = 0;
	T0PC = T0TC = 0;
	T0TCR = 1;
	while(t>T0TC)
	{
		IOCLR0 = buzzer;
		IOSET0 = LED1;
	}
	T0TCR = 0;
}


int main()
{
	u32 distance;
	can2_init();
	uart0_init(9600);
	config_vic();
	en_can2_interrupt();
	en_eint0();
	IODIR0 = (LED1|buzzer|LED2);
	IOSET0 = LED1|LED2;
	IOCLR0 = buzzer;
	while(1)
	{
		if(tx_flag==1)
		{
			delay_ms(200);
			can2_tx(v1);
			tx_flag = 0;
			IOCLR0 = LED2;
			while(1)
			{
				if(rx_flag==1)
				{
					rx_flag = 0;
					distance = v3.byteA;
					uart0_tx_string("\r\n");
					distance = (0.0343 * distance)/2;
					uart0_tx_integer(distance);
					if((distance>=300) && (distance<=400))
					{
						buzzer_beep(1000);
					}
					else if((distance>=200) && (distance <=299))
					{
						buzzer_beep(500);
					}
					else if((distance>=100) && (distance <= 199))
					{
						buzzer_beep(100);
					}
					else if((distance>=50)&& (distance<=99))
					{
						buzzer_beep(50);
					}
					else if(distance<50)
					{
						IOSET0 = buzzer;
						IOCLR0 = LED1;
					}
					else
					{
						IOCLR0 = buzzer;
						IOSET0 = LED1;
					}
				}
				if(tx_flag==1)
				{
					delay_ms(200);
					can2_tx(v2);
					IOCLR0 = buzzer;
					IOSET0 = LED1|LED2;
					tx_flag = 0;
					break;
				}
			}
		}
	}
}
