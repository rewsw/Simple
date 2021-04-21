// Calculate_area_by_hunter.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include "area.h"
typedef unsigned long uLONG;
void Bitmap2binary(uint32_t data) {
	for (int i = 0; i < 32; i++) {
		printf("%d", data & 0x01);
		data >>= 1;
	}
	printf("\r\n");
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

	for (int ii = 0; ii < sizeof(bitmap) / sizeof(uLONG); ii++) {
		Bitmap2binary(bitmap[ii]);
	}
	AREA_LIST_Init();
	SEGMENT_LIST_Init();
	MASK_LIST_Init();
	setRowCol(8, 32);
	setAreaRange(0, 20);
	return 0;
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
