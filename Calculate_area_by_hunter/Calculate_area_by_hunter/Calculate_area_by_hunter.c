// Calculate_area_by_hunter.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//
#include <stdio.h>
#include <stdint.h>

#include "area.h"
typedef unsigned long uLONG;
typedef enum 
{
	False = 0,True = 1
}Bool ;
void Bitmap2binary(uint32_t *data, Bool is_two) {
	if (is_two) {
		for (int i = 0; i < 32; i++) {
			printf("%d", ((*data & 0x80000000) >> 31));
			*data <<= 1;
		}
		printf("\r\n");
	}
	else {
		for (int i = 0; i < 32; i++) {
			printf("%d", ((*data & 0x80000000) >> 31));
			*data <<= 1;
		}
		printf("------------");
		*data++;
		for (int i = 0; i < 32; i++) {
			printf("%d", ((*data & 0x80000000) >> 31));
			*data <<= 1;
		}
		printf("\r\n");
	}
}
int main()
{
	uLONG bitmap[] = { 0x42,
		   0xff,
		   0x00,
		   0xf0,

		   0xf0,
		   0x07,
		   0x07,
		   0x01 };

	for (int ii = 0; ii < sizeof(bitmap) / sizeof(uLONG); ii+=2) {
		Bitmap2binary(&bitmap[ii],False);
	}
	AREA_LIST_Init();
	SEGMENT_LIST_Init();
	MASK_LIST_Init();
	setRowCol(8, 32);
	setAreaRange(0, 20);
	return 0;
}

