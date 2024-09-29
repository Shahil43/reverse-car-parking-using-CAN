 
 /* CAN2_RX_DRIVER*/
 
 #include"header.h"
 extern CAN2 v7;
 extern u32 flag;
  #define RBS (C2GSR&1)
 void can2_init(void)
 {
         u32 *ptr = (u32 *)0xE0038000;
         VPBDIV = 1;
         PINSEL1 |= 0x14000;  //po.23-->RD2, po.24-->TD2
         C2MOD = 1; // SELECT RESET MODE
         C2BTR = 0X001C001D;//SET 125KPBS BAUD RATE
 //      AFMR = 2; //ACCEPT ALL RECIEVED MESSAGE
         ptr[0] = 0x000020AA;
          ptr[1] = 0x20AB20AC;
          SFF_sa = 0;
          SFF_GRP_sa = 0x8;
          EFF_sa =  0x8;
          EFF_GRP_sa =  0x8;
          ENDofTable =  0x8;
          AFMR = 0; //Enable AF setting
          C2MOD = 0; //RELEASE RESET MODE
  }
  
  
  __irq void can2_rx_handler(void)
  {
         v7.id = C2RID;
          v7.dlc = (C2RFS>>16)&0XF;
          v7.rtr = (C2RFS>>30)&1;
         if(v7.rtr==0)
          {
                  v7.byteA = C2RDA;
                  v7.byteB =  C2RDB;
         }
          C2CMR = (1<<2);
          flag = 1;
          VICVectAddr = 0;
  }
  
  
  void config_vic_for_can2(void)
  {
          VICIntSelect = 0;
          VICVectAddr5 = (u32)can2_rx_handler;
          VICVectCntl5 = 27 | (1<<5);
          VICIntEnable |= (1<<27);
  }
  
  
  void en_can2_interrupt(void)
  {
          C2IER = 1;
  }
  
  
  void can2_rx(CAN2 *ptr)
  {
          while(RBS==0); //waiting for msg to recieve
          // read rx buff
          ptr->id = C2RID;   //READING IDENTIFIER
          ptr->dlc = (C2RFS>>16)&0XF;
          ptr->rtr = (C2RFS>>30)&1; //EXTRACT RTR
          if(ptr->rtr==0)
          {
                  ptr->byteA = C2RDA;
                  ptr->byteB = C2RDB;
          }
          C2CMR = (1<<2);//RELEASE RX BUFFER
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
 
  

