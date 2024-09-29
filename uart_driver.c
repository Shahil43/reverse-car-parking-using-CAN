 #include<LPC21XX.H>
 #include "header.h"
 
 void delay_ms(unsigned int ms)
 {
         int a[]={15,60,35,15,15};
         unsigned int pclk;
        pclk=a[VPBDIV]*1000;
       T0PC=0;
         T0PR=pclk-1;
         T0TC=0;
        T0TCR=1;
         while(T0TC<ms);
         T0TCR=0;
 }
 
 
 void uart0_init(u32 baud)
 {
         unsigned int result=0,pclk;
         int a[]={15,60,30,15,15};
         PINSEL0|=0x5;
         pclk=a[VPBDIV]*1000000;
         result= pclk/(16*baud);
         U0LCR=0x83;
         U0DLL= result&0xFF;
         U0DLM=(result>>8)&0xFF;
         U0LCR=0x03;
 }
 #define THRE ((U0LSR>>5)&1)
 void uart0_tx(u8 data)
 {
         U0THR=data;
         while(THRE==0);
 }
 #define RDR (U0LSR&1)
 u8 uart0_rx(void)
 {
        while(RDR==0);
         return U0RBR;
 }
 void uart0_tx_string(s8 *ptr)
 {
         while(*ptr!='\0')
         {
                 U0THR=*ptr;
                 while(THRE==0);
                ptr++;
         }
 }
 void uart0_rx_string(s8 *ptr,u8 len)
 {
         u32 i;
         for(i=0;i<len;i++)
         {
                 while(RDR==0);
                 ptr[i]=U0RBR;
                 if(ptr[i]=='\r')
                         break;
         }
         ptr[i]='\0';
 }
 
 void uart0_tx_integer(s32 n)
 {

         int i=0;
         char a[16];
         if(n==0)
         {
                 uart0_tx('0');
                 return;
         }
         if(n<0)
         {
                uart0_tx('-');
                 n=-n;
         }
         while(n)
         {
                 a[i++]=n%10+48;
                 n=n/10;
         }
 
         for(i--;i>=0;i--)
                 uart0_tx(a[i]);
 }
 

