#include "area.h"

/*GLOBAL VARIABLE*/
AREA_UNIT				AREA_LIST[MAX_AREA_CNT];
AREA_UNIT				TMP_AREA_LIST[MAX_TMP_AREA_CNT];
SEGMENT_UNIT			SEG_LIST[MAX_SEG_CNT];
static MASK_UNIT		MASK_LIST[MAX_MASK_CNT];
static unsigned char	MASK2AREA[MAX_MASK_CNT];
static unsigned char	TMP_AREA_TAILS[MAX_TMP_AREA_CNT];
static unsigned char	ROW, COL;
//static unsigned char	AREA_DISCARD_LIST[MAX_AREA_CNT];
//static unsigned char	areaDiscardIdx;
static unsigned short	AREA_MIN = AREA_MIN_DEFAULT, AREA_MAX = AREA_MAX_DEFAULT;
/*AREA INITIALIZE*/
unsigned char AREA_LIST_Init() {
	unsigned char i;

	//AREA_LIST = malloc(MAX_AREA_CNT * sizeof(AREA_UNIT));
	for (i = 0; i < MAX_AREA_CNT; i++) {
		AREA_LIST[i].AREA_ON_CNT = (unsigned char)0;
		AREA_LIST[i].FIRST_SEG = (unsigned char)0;
		AREA_LIST[i].X = (unsigned char)0;
		AREA_LIST[i].Y = (unsigned char)0;
	}
	return 1;
}

/*SEGMENT INITIALIZE*/
unsigned char SEGMENT_LIST_Init() {
	unsigned char i;

	//SEG_LIST = malloc(MAX_SEG_CNT * sizeof(SEGMENT_UNIT));
	for (i = 0; i < MAX_SEG_CNT; i++) {
		SEG_LIST[i].AREA_SEG_PTR = (unsigned char)0;
		SEG_LIST[i].ROW_NDX = (unsigned char)0;
		SEG_LIST[i].COL_NDX = (unsigned char)0;
		SEG_LIST[i].LEN = (unsigned char)0;
		SEG_LIST[i].MASK_PTR = (unsigned char)0;
	}
	return 1;
}

/*MASK INITIALIZE*/
unsigned char MASK_LIST_Init() {
	unsigned char i;

	//MASK_LIST = malloc(MAX_MASK_CNT * sizeof(MASK_UNIT));
	for (i = 0; i < MAX_MASK_CNT; i++) {
		MASK_LIST[i].AREA_CODE = (unsigned char)0;
	}
	return 1;
}

void setRowCol(unsigned char row, unsigned char col) {
	ROW = row;
	COL = col / 32 + 1;
}
void setAreaRange(unsigned short min, unsigned short max) {
	AREA_MIN = min;
	AREA_MAX = max;
}
/*INPUT MSB(WORD) AND RETURN THE INDEX OF MSB*/
unsigned char indexOfBit(unsigned long word) {
	switch (word) {
	case 0x80000000:
		return 31;
	case 0x40000000:
		return 30;
	case 0x20000000:
		return 29;
	case 0x10000000:
		return 28;
	case 0x08000000:
		return 27;
	case 0x04000000:
		return 26;
	case 0x02000000:
		return 25;
	case 0x01000000:
		return 24;
	case 0x00800000:
		return 23;
	case 0x00400000:
		return 22;
	case 0x00200000:
		return 21;
	case 0x00100000:
		return 20;
	case 0x00080000:
		return 19;
	case 0x00040000:
		return 18;
	case 0x00020000:
		return 17;
	case 0x00010000:
		return 16;
	case 0x00008000:
		return 15;
	case 0x00004000:
		return 14;
	case 0x00002000:
		return 13;
	case 0x00001000:
		return 12;
	case 0x00000800:
		return 11;
	case 0x00000400:
		return 10;
	case 0x00000200:
		return 9;
	case 0x00000100:
		return 8;
	case 0x00000080:
		return 7;
	case 0x00000040:
		return 6;
	case 0x00000020:
		return 5;
	case 0x00000010:
		return 4;
	case 0x00000008:
		return 3;
	case 0x00000004:
		return 2;
	case 0x00000002:
		return 1;
	case 0x00000001:
		return 0;
	default:
		return 255;
	}
}

/*LEAVE THE LEAST SIGNIFICANT BIT AND DELETE OTHER BITS*/
unsigned long findLSB(unsigned long word) {
	//FIND MSB
	//FOR EXAMPLE :       word = 00010000100000100000000100001000 
	/*word |= word>>1;	//word = 00011000110000110000000110001100
	word |= word>>2;	//word = 00011110111100111100000111101111
	word |= word>>4;	//word = 00011111111111111111110111111111
	word |= word>>8;	//word = 00011111111111111111111111111111
	word |= word>>16;	//word = 00011111111111111111111111111111

	return (word>>1)+1;	//word = 00010000000000000000000000000000
	*/
	return word & (-word);
}

/*NEW SEGMENT IS RECORDED*/
void mark(SEGMENT_UNIT** build_stop, unsigned char col, unsigned char row) {
	(*build_stop)->ROW_NDX = row;			//RECORD THE ROW INDEX OF THIS SEGMENT
	(*build_stop)->COL_NDX = col;			//RECORD THE BEGINNING COLUMN INDEX OF THIS SEGMENT
	(*build_stop)->AREA_SEG_PTR = NONEXT;	//NO CONNECTION WITH OTHER SEGMENTS
	(*build_stop)->MASK_PTR = NOMASK;		//NOT ASSIGN TO AREA YET
}

/*COMPLETE THE SEGMENT RECORD*/
char endMark(SEGMENT_UNIT** build_stop, SEGMENT_UNIT** build_max, unsigned char start, unsigned char stop) {
	(*build_stop)->LEN = stop - start;	//RECORD THE LENGTH OF THIS SEGMENT
	if (*build_stop == *build_max)		//REPRESENT THE SEGMENT LIST IS FULL
		return 0;
	(*build_stop)++;					//THE LAST SEGMENT IN SEGMENT LIST
	return 1;
}

char buildSeg(unsigned long WORD, unsigned char endWord, unsigned char concat, SEGMENT_UNIT** build_stop, SEGMENT_UNIT** build_max, unsigned char row) {
	unsigned long WORD_XOR = WORD ^ (WORD << 1);	//FIND THE LOCATION OF 1->0 OR 0->1
	unsigned long A = WORD & WORD_XOR;			//THE START INDEX OF SEGMENT
	unsigned long B = (WORD << 1)&WORD_XOR;		//THE END INDEX OF SEGMENT+1
	unsigned long LSB;							//MOST SIGNIFICANT BIT IN A WORD
	unsigned char indexA, indexB;				//INDEXA IS THE INDEX OF SEGMENT HEAD, INDEXB IS THE TAIL

	if (endWord) {	//THE LAST WORD IN A ROW
		if (concat) {	//PREVIOUS WORD IN SAME ROW HAS UNCOMPLETE SEGMENT
			if (A & 0x80000000) {	//CHECK IF THE UNCOMPLETE SEGMENT IS GROWING
				A ^= 0x80000000;
				LSB = findLSB(B);
				indexB = indexOfBit(LSB) + 32;
				if (!endMark(build_stop, build_max, (*build_stop)->COL_NDX, indexB))
					return 0;	//SEGMENT LIST IS FULL
				B ^= LSB;
			}
			else {
				if (!endMark(build_stop, build_max, (*build_stop)->COL_NDX, 32))	//COMPLETE THE SEGMENT
					return 0;	//SEGMENT LIST IS FULL
			}
		}
	}

	while (B) {	//DOING UNTIL THE SEGMENT TAIL INDEX IS EMPTY
		LSB = findLSB(A);
		indexA = endWord == 1 ? indexOfBit(LSB) + 32 : indexOfBit(LSB);
		mark(build_stop, indexA, row);
		A ^= LSB;	//ERASE THE INDEX OF SEGMENT HEAD

		LSB = findLSB(B);
		indexB = endWord == 1 ? indexOfBit(LSB) + 32 : indexOfBit(LSB);
		if (!endMark(build_stop, build_max, indexA, indexB))
			return 0;
		B ^= LSB;	//ERASE THE INDEX OF SEGMENT TAIL
	}

	if (A) {	//NEW SEGMENT APPEAR, BUT NEED TO BE CHECKED IN NEXT WORD
		LSB = findLSB(A);
		indexA = indexOfBit(LSB);
		mark(build_stop, indexA, row);
		return 2;	//HAVE TO CHECK NECT WORD TO FINISH SEGMENT
	}

	return 1;
}

/*void AREA_JOIN(SEGMENT_UNIT** ref_start, SEGMENT_UNIT** ref_stop, SEGMENT_UNIT** build_start, SEGMENT_UNIT** build_stop, unsigned char* mask_idx){
	unsigned char state1 = (*ref_start)==(*ref_stop); 		//STATE1 STANDS FOR REFERENCE SEGMENTS IS OVER
	unsigned char state2 = (*build_start)==(*build_stop); 	//STATE2 STANDS FOR BUILDING SEGMENTS IS OVER
	unsigned char ref_high = (unsigned char)(*ref_start)->COL_NDX+(unsigned char)(*ref_start)->LEN-1;		//RECORD THE RIGHTMOST BIT IN THE CURRENT REFERENCE SEGMENT
	unsigned char build_high = (unsigned char)(*build_start)->COL_NDX+(unsigned char)(*build_start)->LEN-1;	//RECORD THE RIGHTMOST BIT IN THE CURRENT BUILDING SEGMENT

	while( !state2 ){	//KEEP DOING UNTIL BUILDING SEGMENTS IS OVER
		//printfpro(("ref = %d build = %d\r\n", (*ref_start)->COL_NDX, (*build_start)->COL_NDX);
		if(!state1 && (ref_high < (*build_start)->COL_NDX)){	//REFERENCE SEGMENTS NOT OVER AND REF.HIGH < BUILD.LOW OCCUR
			state1 = ++(*ref_start)==(*ref_stop);
			ref_high = (unsigned char)(*ref_start)->COL_NDX+(unsigned char)(*ref_start)->LEN-1;
			continue;
		}else if(state1 || (!state2 && ((*ref_start)->COL_NDX > build_high))){//CASE 1:REFERENCE SEGMENTS IS OVER
																			  //CASE 2:BUILDING SEGMENTS NOT OVER AND REF.LOW > BUILD.HIGH OCCUR
			if((*build_start)->MASK_PTR == NOMASK){			//ASSIGN NEW AREA CODE TO NOMASKING SEGMENT
				(*build_start)->MASK_PTR = *mask_idx;
				MASK_LIST[*mask_idx].AREA_CODE = *mask_idx;
				(*mask_idx)++;
			}
			state2 = ++(*build_start)==(*build_stop);	//GO NEXT BUILDING SEGMENT
			build_high = (unsigned char)(*build_start)->COL_NDX+(unsigned char)(*build_start)->LEN-1;
			continue;
		}
		//printfpro(("overlap\r\n");
		//printfpro(("col = %d, build mask %d\r\n", (*build_start)->COL_NDX, (*build_start)->MASK_PTR);
		//REFERENCE SEGMENT AND BUILDING SEGMENT IS OVERLAP BELOW
		if((*build_start)->MASK_PTR != NOMASK)	//CONNECT TWO AREA
			MASK_LIST[(*ref_start)->MASK_PTR].AREA_CODE = MASK_LIST[(*build_start)->MASK_PTR].AREA_CODE;
		else									//ASSIGN AREA CODE OF REFERENCE SEGMENT TO BUILDING SEGMENT
			(*build_start)->MASK_PTR = (*ref_start)->MASK_PTR;
		//printfpro(("after build mask %d\r\n", (*build_start)->MASK_PTR);

		if(ref_high < build_high){	//OVERLAP AND REF.HIGH < BUILD.HIGH, GO NEXT REFERENCE SEGMENT
			state1 = ++(*ref_start)==(*ref_stop);
			ref_high = (*ref_start)->COL_NDX+(*ref_start)->LEN-1;
		}else if(ref_high > build_high){	//OVERLAP AND REF.HIGH > BUILD.HIGH, GO NEXT BUILDING SEGMENT
			state2 = ++(*build_start)==(*build_stop);
			build_high = (unsigned char)(*build_start)->COL_NDX+(unsigned char)(*build_start)->LEN-1;
		}else if(ref_high == build_high){	//OVERLAP AND REF.HIGH = BUILD.HIGH, GO NEXT REFERENCE AND BUILDING SEGMENT
			state1 = ++(*ref_start)==(*ref_stop);
			ref_high = (unsigned char)(*ref_start)->COL_NDX+(unsigned char)(*ref_start)->LEN-1;
			state2 = ++(*build_start)==(*build_stop);
			build_high = (unsigned char)(*build_start)->COL_NDX+(unsigned char)(*build_start)->LEN-1;
		}
		//printfpro(("end (%d, %d) (%d, %d)\r\n", SEG_REF_START, SEG_REF_STOP, SEG_BUILD_START, SEG_BUILD_STOP);
	}
	(*ref_start) = (*ref_stop);	//ENSURE THE REFERENCE SEGMENT IS OVER
	(*ref_stop) = (*build_stop);	//CHANGE BUILDING SEGMENTS TO REFERENCE SEGMENTS
	//printfpro(("ref_start = (%d, %d), build_start = (%d, %d)\r\n", (*ref_start)->ROW_NDX, (*ref_start)->COL_NDX, (*ref_stop)->ROW_NDX, (*ref_stop)->COL_NDX);
}*/

void AREA_JOIN(SEGMENT_UNIT** ref_start, SEGMENT_UNIT** ref_stop, SEGMENT_UNIT** build_start, SEGMENT_UNIT** build_stop, unsigned char* mask_idx) {
	unsigned char state1 = (*ref_start) == (*ref_stop); 		//STATE1 STANDS FOR REFERENCE SEGMENTS IS OVER
	unsigned char state2 = (*build_start) == (*build_stop); 	//STATE2 STANDS FOR BUILDING SEGMENTS IS OVER
	unsigned char ref_high = (unsigned char)(*ref_start)->COL_NDX + (unsigned char)(*ref_start)->LEN - 1;		//RECORD THE RIGHTMOST BIT IN THE CURRENT REFERENCE SEGMENT
	unsigned char build_high = (unsigned char)(*build_start)->COL_NDX + (unsigned char)(*build_start)->LEN;	//RECORD THE RIGHTMOST BIT IN THE CURRENT BUILDING SEGMENT

	while (!state2) {	//KEEP DOING UNTIL BUILDING SEGMENTS IS OVER	
		//printfpro(("ref = %d build = %d\r\n", (*ref_start)->COL_NDX, (*build_start)->COL_NDX);
		if (!state1 && (ref_high < (*build_start)->COL_NDX - 1)) {	//REFERENCE SEGMENTS NOT OVER AND REF.HIGH < BUILD.LOW OCCUR 
			state1 = ++(*ref_start) == (*ref_stop);
			ref_high = (unsigned char)(*ref_start)->COL_NDX + (unsigned char)(*ref_start)->LEN - 1;
			continue;
		}
		else if (state1 || (!state2 && ((*ref_start)->COL_NDX > build_high))) {//CASE 1:REFERENCE SEGMENTS IS OVER
																			 //CASE 2:BUILDING SEGMENTS NOT OVER AND REF.LOW > BUILD.HIGH OCCUR
			if ((*build_start)->MASK_PTR == NOMASK) {			//ASSIGN NEW AREA CODE TO NOMASKING SEGMENT
				(*build_start)->MASK_PTR = *mask_idx;
				MASK_LIST[*mask_idx].AREA_CODE = *mask_idx;
				(*mask_idx)++;
			}
			state2 = ++(*build_start) == (*build_stop);	//GO NEXT BUILDING SEGMENT
			build_high = (unsigned char)(*build_start)->COL_NDX + (unsigned char)(*build_start)->LEN;
			continue;
		}
		//printfpro(("overlap\r\n");
		//printfpro(("col = %d, build mask %d\r\n", (*build_start)->COL_NDX, (*build_start)->MASK_PTR);
		//REFERENCE SEGMENT AND BUILDING SEGMENT IS OVERLAP BELOW
		if ((*build_start)->MASK_PTR != NOMASK)	//CONNECT TWO AREA
			MASK_LIST[(*ref_start)->MASK_PTR].AREA_CODE = MASK_LIST[(*build_start)->MASK_PTR].AREA_CODE;
		else									//ASSIGN AREA CODE OF REFERENCE SEGMENT TO BUILDING SEGMENT
			(*build_start)->MASK_PTR = (*ref_start)->MASK_PTR;
		//printfpro(("after build mask %d\r\n", (*build_start)->MASK_PTR);

		if (ref_high < build_high) {	//OVERLAP AND REF.HIGH < BUILD.HIGH, GO NEXT REFERENCE SEGMENT
			state1 = ++(*ref_start) == (*ref_stop);
			ref_high = (*ref_start)->COL_NDX + (*ref_start)->LEN - 1;
		}
		else if (ref_high > build_high) {	//OVERLAP AND REF.HIGH > BUILD.HIGH, GO NEXT BUILDING SEGMENT
			state2 = ++(*build_start) == (*build_stop);
			build_high = (unsigned char)(*build_start)->COL_NDX + (unsigned char)(*build_start)->LEN;
		}
		else if (ref_high == build_high) {	//OVERLAP AND REF.HIGH = BUILD.HIGH, GO NEXT REFERENCE AND BUILDING SEGMENT
			state1 = ++(*ref_start) == (*ref_stop);
			ref_high = (unsigned char)(*ref_start)->COL_NDX + (unsigned char)(*ref_start)->LEN - 1;
			state2 = ++(*build_start) == (*build_stop);
			build_high = (unsigned char)(*build_start)->COL_NDX + (unsigned char)(*build_start)->LEN;
		}
		//printfpro(("end (%d, %d) (%d, %d)\r\n", SEG_REF_START, SEG_REF_STOP, SEG_BUILD_START, SEG_BUILD_STOP);
	}
	(*ref_start) = (*ref_stop);	//ENSURE THE REFERENCE SEGMENT IS OVER
	(*ref_stop) = (*build_stop);	//CHANGE BUILDING SEGMENTS TO REFERENCE SEGMENTS
	//printfpro(("ref_start = (%d, %d), build_start = (%d, %d)\r\n", (*ref_start)->ROW_NDX, (*ref_start)->COL_NDX, (*ref_stop)->ROW_NDX, (*ref_stop)->COL_NDX);
}

unsigned char getArea(unsigned long *BITMAP) {
	unsigned char	AREA_READY_LIST = 0, TMP_AREA_IDX = 0;	//THE INDEX OF UNUSED AREA LIST
	unsigned char	MASK_READY_LIST = 0;	//THE INDEX OF UNUSED MASK LIST
	unsigned char	returnType = 0;	//RETURNTYPE = 0 STANDS FOR SEGMENT LIST IS FULL, 1 COMPLETE, 2 COMPLETE BUT SEGMENT NOT FINISH
	unsigned char	seg_idx = 0, mask_idx = 0, area_idx = 0;
	SEGMENT_UNIT*	seg;
	unsigned long	WORD;
	SEGMENT_UNIT	*SEG_REF_START = SEG_LIST, *SEG_REF_STOP = SEG_LIST;	//THE START AND STOP POINTER OF REFERENCE SEGMENTS
	SEGMENT_UNIT	*SEG_BUILD_START = SEG_LIST, *SEG_BUILD_STOP = SEG_LIST;//THE START AND STOP POINTER OF BUILDING SEGMENTS
	SEGMENT_UNIT	*SEG_BUILD_MAX = SEG_LIST + MAX_SEG_CNT - 1;	//THE TAIL POINTER OF SEGMENT LIST
	unsigned char ROW_NDX = 0;	//ROW INDEX
	unsigned char	big_panel = COL == 2 ? 1 : 0;
	unsigned char areaIdx = 0, areaCnt = 0;

	for (ROW_NDX = 0; ROW_NDX < ROW; ROW_NDX++) {
		/*********************FIRST WORD***********************/
		WORD = *(BITMAP + ROW_NDX * COL);
		if (WORD) {
			returnType = buildSeg(WORD, 0, 0, &SEG_BUILD_STOP, &SEG_BUILD_MAX, ROW_NDX);
			if (!returnType)	//SEGMENT LIST IS FULL
				break;
			else if ((returnType == 2) && !big_panel) {
				endMark(&SEG_BUILD_STOP, &SEG_BUILD_MAX, SEG_BUILD_STOP->COL_NDX, 32);	//COMPLETE THE SEGMENT
				returnType = 0;
			}
		}
		/*********************SECOND WORD***********************/
		if (big_panel) {
			WORD = *(BITMAP + ROW_NDX * COL + 1);
			if (WORD) {
				if (!buildSeg(WORD, 1, returnType == 2 ? 1 : 0, &SEG_BUILD_STOP, &SEG_BUILD_MAX, ROW_NDX))
					break;	//SEGMENT LIST IS FULL
			}
			else if (returnType == 2) {	//SEGMENT NOT FINISH
				endMark(&SEG_BUILD_STOP, &SEG_BUILD_MAX, SEG_BUILD_STOP->COL_NDX, 32);	//COMPLETE THE SEGMENT
				returnType = 0;
			}
		}
		/*********************CONNECT SEGMENTS******************/
		//printfpro(("row = %d\r\n", ROW_NDX);
		//printfpro(("ref_start = (%d, %d), build_start = (%d, %d)\r\n", SEG_REF_START->ROW_NDX, SEG_REF_START->COL_NDX, SEG_BUILD_START->ROW_NDX, SEG_BUILD_START->COL_NDX);
		AREA_JOIN(&SEG_REF_START, &SEG_REF_STOP, &SEG_BUILD_START, &SEG_BUILD_STOP, &mask_idx); //CHECK SEGMENT AND CONNECT AREA IF NECESSARY
	}

#if PRINT_SEGMENT
	printf("********SEGMENTS******\r\n");
	for (seg = SEG_LIST; seg < SEG_BUILD_STOP; seg++) {
		mask_idx = MASK_LIST[seg->MASK_PTR].AREA_CODE;
		printf("(%d, %d)\t%d", seg->ROW_NDX, seg->COL_NDX, seg->LEN);
		printf("\tarea = (%d, %d, %d)\r\n", seg->MASK_PTR, mask_idx, MASK2AREA[mask_idx]);
	}
	printf("**********************\r\n");
#endif

	//areaDiscardIdx = 0;
	/*ASSIGN SEGMENT TO AREA_UNIT AND CONNECT THE SEGMENT IF IN SAME AREA*/
	for (seg = SEG_LIST, seg_idx = 0; seg < SEG_BUILD_STOP; seg++, seg_idx++) {
		mask_idx = MASK_LIST[seg->MASK_PTR].AREA_CODE;
		if (AREA_READY_LIST == MAX_AREA_CNT && mask_idx >= MAX_AREA_CNT)
			continue;	//AREA LIST IS FULL

		//printf("seg%d: %d %d %d %d\r\n", seg_idx, mask_idx, seg->ROW_NDX, seg->COL_NDX, seg->LEN);

		if (mask_idx >= MASK_READY_LIST) {	//LARGER THAN MASK_READY_LIST MEANS THE AREA CORRESPOND TO MASK_IDX IS NOT BE ADDED TO AREA LIST
			//printfpro(("before : mask %d area %d\r\n", mask_idx, MASK2AREA[mask_idx]);
			/*if(areaDiscardIdx > 0){
				TMP_AREA_IDX = AREA_DISCARD_LIST[areaDiscardIdx--];
			}else if(seg->LEN > AREA_MAX){
				MASK2AREA[mask_idx] = DISCARD_AREA;	//<----------------??
				continue;							//<----------------??
			}else{
				TMP_AREA_IDX = AREA_READY_LIST;
				AREA_READY_LIST++;
			}*/

			MASK2AREA[mask_idx] = TMP_AREA_IDX;	//ASSIGN AREA CODE TO APPROPRIATE AREA(UNUSED SMALLEST AREA IN LIST)
			TMP_AREA_LIST[TMP_AREA_IDX].AREA_ON_CNT = seg->LEN;
			TMP_AREA_LIST[TMP_AREA_IDX].FIRST_SEG = seg_idx;
			TMP_AREA_LIST[TMP_AREA_IDX].X = seg->COL_NDX;
			TMP_AREA_LIST[TMP_AREA_IDX].Y = seg->ROW_NDX;
			TMP_AREA_TAILS[TMP_AREA_IDX] = seg_idx;
			++TMP_AREA_IDX;
			MASK_READY_LIST = mask_idx + 1;	//MASK_READY_LIST MUST LARGER THAN ALL CHECKED MASK_IDX
			//printfpro(("after : mask %d area %d\r\n", mask_idx, MASK2AREA[mask_idx]);
		}
		else {
			area_idx = MASK2AREA[mask_idx];
			/*if(area_idx == DISCARD_AREA)
				continue;*/
			TMP_AREA_LIST[area_idx].AREA_ON_CNT += seg->LEN;	//ACCUMLATE THE SEGMENT LENGTH TO THE AREA

			/*if(AREA_LIST[area_idx].AREA_ON_CNT > AREA_MAX){	//discard too big area
				AREA_DISCARD_LIST[areaDiscardIdx++] = MASK2AREA[mask_idx];	//release resource
				MASK2AREA[mask_idx] = DISCARD_AREA;	//mark this area as discarded area
				continue;
			}*/

			if (seg->COL_NDX < TMP_AREA_LIST[area_idx].X)		//RECORD THE LEFTMOST LOCATION
				TMP_AREA_LIST[area_idx].X = seg->COL_NDX;
			SEG_LIST[TMP_AREA_TAILS[area_idx]].AREA_SEG_PTR = seg_idx; //CONNECT TWO SEGMENT IN THE SAME AREA
			TMP_AREA_TAILS[area_idx] = seg_idx;	//THE LAST SEGMENT OF THE AREA
		}
	}

	for (areaIdx = 0; areaIdx < TMP_AREA_IDX; areaIdx++) {
		if (TMP_AREA_LIST[areaIdx].AREA_ON_CNT < AREA_MIN || TMP_AREA_LIST[areaIdx].AREA_ON_CNT > AREA_MAX)
			continue;
		else {
			if (AREA_READY_LIST < MAX_AREA_CNT) {
				AREA_LIST[AREA_READY_LIST].AREA_ON_CNT = TMP_AREA_LIST[areaIdx].AREA_ON_CNT;
				AREA_LIST[AREA_READY_LIST].FIRST_SEG = TMP_AREA_LIST[areaIdx].FIRST_SEG;
				AREA_LIST[AREA_READY_LIST].X = TMP_AREA_LIST[areaIdx].X;
				AREA_LIST[AREA_READY_LIST].Y = TMP_AREA_LIST[areaIdx].Y;
				AREA_READY_LIST++;
			}
			else {
				break;
			}
		}
	}

#if PRINT_SEGMENT_FINAL
	printf("********SEGMENTS******\r\n");
	for (seg = SEG_LIST; seg < SEG_BUILD_STOP; seg++) {
		mask_idx = MASK_LIST[seg->MASK_PTR].AREA_CODE;
		printf("(%d, %d)\t%d", seg->ROW_NDX, seg->COL_NDX, seg->LEN);
		printf(" area = (%d, %d, %d)\r\n", seg->MASK_PTR, mask_idx, MASK2AREA[mask_idx]);
	}
	printf("**********************\r\n");
#endif	
	return AREA_READY_LIST;//areaCnt;
}