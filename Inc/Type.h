
#ifndef TYPE_H
#define TYPE_H

#define TEST_DEBUG

//-------------------- include files ----------------------------------------
//#include "stm32f2xx.h"
#include <stdio.h>

//#ifdef TEST_DEBUG
//   #include "stm32f429i_discovery_lcd.h"
//#endif

//-------------------- public definitions -----------------------------------
#ifndef NULL
#define NULL    ((void*) 0 )
#endif


// Basic unsigned types
typedef unsigned char  UNSIGNED8;
typedef unsigned short UNSIGNED16;
typedef unsigned long  UNSIGNED32;

// Basic signed types
typedef signed char  SIGNED8;
typedef signed short SIGNED16;
typedef signed long  SIGNED32;

//typedef float        FLOAT32;    //AVE: TBR ?

// "typeless" types - NOT TO BE USED IN NUMERICAL CALCULATION
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned short BIT_SEQUENCE;


typedef unsigned char    BOOLEAN;      // eg bVarName
#define FALSE 0
#ifndef TRUE
   #define TRUE  !FALSE
#endif


// Define general const's

#define TWO_IN_32_C 4294967295
#define TWO_IN_31_C 2147483648
#define TWO_IN_30_C 1073741824
#define TWO_IN_29_C 536870912
#define TWO_IN_28_C 268435456
#define TWO_IN_27_C 134217728
#define TWO_IN_24_C 16777216
#define TWO_IN_22_C 4194304
#define TWO_IN_21_C 2097152
#define TWO_IN_20_C 1048576
#define TWO_IN_18_C 262144
#define TWO_IN_16_C 65536
#define TWO_IN_15_C 32768
#define TWO_IN_14_C 16384
#define TWO_IN_12_C 4096
#define TWO_IN_10_C 1024
#define TWO_IN_9_C  512
#define TWO_IN_8_C  256
#define TWO_IN_7_C  128
#define TWO_IN_6_C  64
#define TWO_IN_5_C  32
#define TWO_IN_4_C  16
#define TWO_IN_2_C  4

#define TEN_IN_6_C  1000000


#endif  // TYPE_H
//---------------------------------------------------------------------------
//  End of file
//---------------------------------------------------------------------------
