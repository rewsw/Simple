// Hungaria_ Algorithm.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <vector>
using namespace std;
struct Point
{
	int x, y;
};
void show(int** data, int row, int col) {
	for (int i = 0; i < row; i++) {
		int *ptr = *data++;
		for (int j = 0; j < col; j++) {
			cout << *ptr++ << " ";
		}
		cout << endl;
	}
}
int main()
{
	vector<Point>zero_position;
	int data[3][3] = { {30,25,10},{15,10,20},{25,20,15} };
	int *data_ptr[3];
	for (int i = 0; i < 3; i++) {
		data_ptr[i] = &data[i][0];
	}
	for (int i = 0; i < 3; i++) {
		int min = 1000;
		int *l_ptr = data_ptr[i];
		for (int j = 0; j < 3; j++) {
			if (*l_ptr < min) {
				min = *l_ptr;
			}
			*l_ptr++;

		}
		l_ptr = data_ptr[i];
		for (int j = 0; j < 3; j++) {
			*l_ptr++ -= min;


		}
	}

	for (int i = 0; i < 3; i++) {
		int min = 1000;
		for (int j = 0; j < 3; j++) {
			if (data[j][i] < min) {
				min = data[j][i];
			}
		}
		for (int j = 0; j < 3; j++) {
			data[j][i] -= min;
			if (data[j][i] == 0) {
				zero_position.push_back(Point{ i,j });
			}
		}
	}
	while (zero_position.size())
	{
		//for()
	}

	show(data_ptr, 3, 3);


	std::cout << "Hello World!\n";
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