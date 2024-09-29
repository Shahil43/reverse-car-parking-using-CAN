   /* reverse parking assistance in the rx node main function*/
   
  
   #include"header.h"
   #define TRIG (1<<8)
   #define echo ((IOPIN0>>9)&1)
   
  CAN2 v1;
   CAN2 v7;
  u32 flag;
  
  
  void delay_us(u32 t)
  {
          int a[] = {15,60,30,15,15};
          u32 pclk = a[VPBDIV];
          T0PC = 0;
          T0PR = pclk - 1;
          T0TC = 0;
          T0TCR = 1;
          while(t>T0TC);
          T0TCR = 0;
  }
  
  
  void send_pulse()
  {
          IOSET0 = TRIG;
          delay_us(10);
          IOCLR0 = TRIG;
  }
  
  
  u32 get_range()
  {
          send_pulse();
          while(echo==0);
          T0TC = T0PC = 0;
          T0TCR = 1;
          while(echo==1);
          T0TCR = 0;
          return T0TC;
  }
  
  int main()
  {
          can2_init();
          IODIR0 |= TRIG;
          IOCLR0 = TRIG;
          config_vic_for_can2();
          en_can2_interrupt();
          uart0_init(9600);
          uart0_tx_string("test code\r\n");
          while(1)
          {
                  if(flag)
                  {
                          if(v7.rtr==1)
                          {
                                  v1.rtr = 0;
                                  v1.id = v7.id;
                                  v1.dlc = v7.dlc;
                                  v1.byteA = get_range();
                                  can2_tx(v1);
                                  delay_ms(100);
                                  uart0_tx_string("data sended\r\n");
                          }
                          if(v7.rtr==0)
                          {
                                  flag = 0;
                                  uart0_tx_string("remote frame\r\n");
  
                          }
                  }
          }
  }

