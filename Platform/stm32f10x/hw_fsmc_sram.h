#ifndef  __HW_FSMC_SRAM_H
#define  __HW_FSMC_SRAM_H

/* ---地址区定义--- */
#define Bank1_SRAM1_ADDR    (0x60000000)
#define Bank1_SRAM2_ADDR    (0x64000000)
#define Bank1_SRAM3_ADDR    (0x68000000)
#define Bank1_SRAM4_ADDR    (0x6C000000)

#define FSMC_SRAM_NE1  (0)
#define FSMC_SRAM_NE2  (2)
#define FSMC_SRAM_NE3  (4)
#define FSMC_SRAM_NE4  (6)

#define FSMC_SRAM_NES   FSMC_SRAM_NE3

#if(FSMC_SRAM_NES==FSMC_SRAM_NE1)
#define FSMC_SRAM_ADDR Bank1_SRAM1_ADDR

#elif(FSMC_SRAM_NES==FSMC_SRAM_NE2)
#define FSMC_SRAM_ADDR Bank1_SRAM2_ADDR

#elif(FSMC_SRAM_NES==FSMC_SRAM_NE3)
#define FSMC_SRAM_ADDR Bank1_SRAM3_ADDR

#elif(FSMC_SRAM_NES==FSMC_SRAM_NE4)
#define FSMC_SRAM_ADDR Bank1_SRAM4_ADDR

#else

#error "FSMC_SRAM_NES Parameter error!"

#endif

#ifdef __cplusplus
extern "C" {
#endif

  void FSMC_SRAM_Init (void);  //基于FSMC的SRAM扩展使能
  
#ifdef __cplusplus
}
#endif

#endif  /* __FSMC_SRAM_H */
