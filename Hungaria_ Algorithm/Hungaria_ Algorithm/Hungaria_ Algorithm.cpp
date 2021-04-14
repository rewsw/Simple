// Hungaria_ Algorithm.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <vector>
#include <stdio.h>
using namespace std;
struct Point
{
	int x, y;
	bool is_check = false;
	
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
/*
int main() {
	int data[5][5] = { {5,0,2,0,2},{2,3,0,0,0,},{0,10,5,7,5},{9,8,0,0,4},{0,6,3,6,2} };
	vector<Point>  zero_position;
	for (int i = 0; i < 5; i++) {
		int min = 1000;
		for (int j = 0; j < 5; j++) {
			if (data[j][i] < min) {
				min = data[j][i];
			}
		}
		for (int j = 0; j < 5; j++) {
			data[j][i] -= min;
			if (data[j][i] == 0) {
				zero_position.push_back(Point{ i,j });
			}
			
		}
	}
	int line_num = 0;
	while (zero_position.size())
	{
		Point init = zero_position[zero_position.size() - 1];
		vector<int> can_pick_x, can_pick_y;
		for (int i = zero_position.size() - 2; i >= 0; i--) {
			if (init.x == zero_position[i].x) {
				can_pick_x.push_back(i);
			}
			else if (init.y == zero_position[i].y) {
				can_pick_y.push_back(i);
			}
		}
		for (int i = 0; i < can_pick_x.size(); i++) {
			cout << can_pick_x[i] << endl;
			zero_position.erase(zero_position.begin() + can_pick_x[i]);
		}
		for (int i = 0; i < can_pick_y.size(); i++) {
			cout << can_pick_y[i] << endl;
			zero_position.erase(zero_position.begin() + can_pick_y[i]);
		}
		cout << "-----" << endl;
		zero_position.pop_back();
		if (can_pick_x.size() > 0 && can_pick_y.size() > 0) {
			line_num += 2;
		}
		else
		{
			line_num++;

		}

	}
	printf("num %d \r\n", line_num);
}
*/
int main() {
	//int data[4][4] = { {2,15,13,4},{10,4,14,15},{9,14,16,13},{7,8,11,9}};
	int data[5][5] = { {5,0,2,0,2},{2,3,0,0,0,},{0,10,5,7,5},{9,8,0,0,4},{0,6,3,6,2} };
	int *data_ptr[5];
	for (int i = 0; i < 5; i++) {
		data_ptr[i] = &data[i][0];
	}
	int zero_Point_R[5] = { 0,0,0,0,0 };
	int zero_Point_C[5] = { 0,0,0,0,0 };
	bool R[5] = { false };
	bool C[5] = { true };
	int zero[5][5] = {};
	
	vector<Point>  zero_pos;
	for (int i = 0; i < 5; i++) {
		int *l_ptr = data_ptr[i];
		int *m_ptr = zero[i];
		memset(m_ptr, 0, 5);
		for (int j = 0; j < 5; j++) {
			if (*l_ptr++ == 0) {
				m_ptr[j] = 1;
				zero_pos.push_back(Point{ i,j });
				zero_Point_C[j]++;
				zero_Point_R[i]++;
			}
		}
	}
	int min = 1000;
	for (int i = 0; i < 5; i++) {
		if (min > zero_Point_C[i]) {
			min = zero_Point_C[i];
		}
	}
	cout << min << endl;
	for (auto p : zero_pos) {
		cout << p.x << " " << p.y << endl;
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << zero[i][j] << " ";
		}
		cout << endl;
	}
	show(data_ptr, 5, 5);

	return 0;
}
/*
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
	int all_min = 1000;
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
			if (data[j][i] < all_min && data[j][i]!= 0) {
				all_min = data[j][i];
			}
		}
	}
	int line_num = 0;


	//zero_position.push_back(Point{ 1,3 });
	//zero_position.push_back(Point{ 3,0 });
	//zero_position.push_back(Point{ 3,2 });
	//zero_position.push_back(Point{ 3,3 });
	for (auto p : zero_position) {
		printf("p %d %d\r\n", p.x, p.y);
	}
	while (zero_position.size())
	{
		Point init = zero_position[zero_position.size()-1];
		vector<int> can_pick_x, can_pick_y;
		for (int i = zero_position.size()-2; i>=0; i--) {
			if (init.x == zero_position[i].x) {
				can_pick_x.push_back(i);
			}
			else if (init.y == zero_position[i].y) {
				can_pick_y.push_back(i);
			}
		}
		for (int i = 0; i < can_pick_x.size(); i++) {
			cout << can_pick_x[i] << endl;
			zero_position.erase(zero_position.begin()+ can_pick_x[i]);
		}
		for (int i = 0; i < can_pick_y.size(); i++) {
			cout << can_pick_y[i] << endl;
			zero_position.erase(zero_position.begin() + can_pick_y[i]);
		}
		cout << "-----" << endl;
		zero_position.pop_back();
		if (can_pick_x.size() > 0 && can_pick_y.size() > 0) {
			line_num+=2;
		}
		else
		{
			line_num++;

		}
		
	}
	printf("num %d min %d\r\n", line_num,all_min);

	if (line_num < 3) {
		
	}
	show(data_ptr, 3, 3);


	std::cout << "Hello World!\n";
}
*/
// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案