   #include <lpc21xx.h>
     typedef unsigned int u32;
   typedef signed int s32;
   typedef unsigned char u8;
   typedef signed char s8;
   typedef unsigned short int u16;
   
   typedef struct CAN2_MSG{
   u32 id;
  u32 byteA;
  u32 byteB;
  u8 rtr;
  u8 dlc;
  }CAN2;
  
  extern void uart0_init(u32 baud);
  extern void uart0_tx(u8 data);
  extern void delay_sec(u32);
  extern void delay_ms(u32);
  extern void lcd_data(u8);
  extern void lcd_cmd(u8);
  extern void lcd_init(void);
  extern void lcd_string(s8*);
  extern u8 uart0_rx(void);
  extern void uart0_tx_string(s8*);
  extern void uart0_rx_string(s8*,u8);
  #define SW (((IOPIN0>>14)&1))
  extern void config_vic(void);
  #define CS0 (1<<7)
  extern void spi0_init(void);
  extern u8 spi0(u8 data);
  extern void uart0_tx_float(float num);
  extern void uart0_tx_integer(s32 num);
  extern u16 read_mcp3204(u8 ch_num);
  #define TCS (((C2GSR>>3)&1))
  
          extern void can2_init(void);
          extern void config_vic_for_can2(void);
          extern void en_can2_interrupt(void);
          extern void can2_rx(CAN2 *ptr);
                                                     extern void can2_tx(CAN2 v);

