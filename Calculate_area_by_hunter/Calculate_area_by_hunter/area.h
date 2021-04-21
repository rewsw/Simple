#ifndef AREA_H
#define AREA_H

#include <stdio.h>

#define NOMASK				0x3F
#define NONEXT				0xFF
#define DISCARD_AREA		0xFF
#define AREA_FULL			0xFFFFFF80
#define MAX_AREA_CNT 		10
#define MAX_TMP_AREA_CNT 	35
#define MAX_SEG_CNT  		254
#define MAX_MASK_CNT 		64
#define AREA_MIN_DEFAULT 			0
#define AREA_MAX_DEFAULT 			30

#define PRINT_SEGMENT		0
#define PRINT_SEGMENT_FINAL 0

/*STRUCTURE*/
//	record every segments(continuos 1's in a column) in bitmap
typedef struct _SEGMENT_UNIT {
	unsigned long	AREA_SEG_PTR : 8;
	unsigned long	ROW_NDX : 6;
	unsigned long	COL_NDX : 6;
	unsigned long	LEN : 6;
	unsigned long	MASK_PTR : 6;
}SEGMENT_UNIT;

//	record area which contains connected segments
typedef struct _AREA_UNIT {
	unsigned char	AREA_ON_CNT;
	unsigned char	FIRST_SEG;
	unsigned char	X;
	unsigned char	Y;
}AREA_UNIT;

// record the range of segments in the scanning row and the last row, which is purposed to check whether the segment can connect each other
typedef struct _MASK_UNIT {
	unsigned char	AREA_CODE;
}MASK_UNIT;
AREA_UNIT		AREA_LIST[MAX_AREA_CNT];
extern SEGMENT_UNIT		SEG_LIST[MAX_SEG_CNT];

void			setRowCol(unsigned char ROW, unsigned char COL);
unsigned char	AREA_LIST_Init(void);
unsigned char	SEGMENT_LIST_Init(void);
unsigned char	MASK_LIST_Init(void);
unsigned char	getArea(unsigned long *BITMAP);
void setAreaRange(unsigned short min, unsigned short max);

#endif