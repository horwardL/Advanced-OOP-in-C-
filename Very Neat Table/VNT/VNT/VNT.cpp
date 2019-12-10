//author: han wen loh

#include <iostream>
#include <cmath>
using namespace std;

class VNT {
private:
	int row, col;
	int lastRow, lastCol;
	int** matrix;

public:
	VNT() {
		row = 0; col = 0, lastRow = -1, lastCol = -1;
		matrix = NULL;
	}

	VNT(int s) {
		row = s; col = s;
		matrix = new int*[row];
		for (int i = 0; i < row; ++i) {
			matrix[i] = new int[col];
			for (int j = 0; j < col; ++j) {
				matrix[i][j] = INT_MAX;
			}
		}
		lastRow = lastCol = -1;
	}

	VNT(int r, int c) {
		row = r; col = c;
		matrix = new int* [row];
		for (int i = 0; i < row; ++i) {
			matrix[i] = new int[col];
			for (int j = 0; j < col; ++j) {
				matrix[i][j] = INT_MAX;
			}
		}
		lastRow = lastCol = -1;
	}

	void add(int n) {
		if (matrix[row - 1][col - 1] < INT_MAX) {
			cout << "VNT is full";
			return;
		}
		lastCol = (lastCol + 1) % col;
		if (lastCol == 0)
			++lastRow;
		matrix[lastRow][lastCol] = n;
		bubbleUp(lastRow, lastCol);
	}

	void bubbleUp(int lastRow, int lastCol) {
		int top = lastRow, left = lastCol, temp = 0;
		while (true) {
			if (top <= 0 && left <= 0) {
				return;
			}
			else if (top <= 0) {
				if (matrix[top][left - 1] > matrix[top][left]) {
					swap(top, left, top, left - 1);
					--left;
				}
				else {
					return;
				}
			}
			else if(left <= 0){
				if(matrix[top - 1][left] > matrix[top][left]) {
					swap(top, left, top - 1, left);
					--top;
				}
				else {
					return;
				}
			}
			else {
				if (matrix[top][left - 1] > matrix[top][left] && matrix[top - 1][left] > matrix[top][left]) {
					if (matrix[top][left - 1] > matrix[top - 1][left]) {
						swap(top, left - 1, top, left);
						--left;
					}
					else {
						swap(top - 1, left, top, left);
						--top;
					}
				}else if(matrix[top][left - 1] > matrix[top][left]){
					swap(top, left - 1, top, left);
					--left;
				}
				else if (matrix[top - 1][left] > matrix[top][left]) {
					swap(top, left, top - 1, left);
					--top;
				}
				else {
					return;
				}
			}

		}
	}

	void swap(int t1, int l1, int t2, int l2) {
		int temp = matrix[t1][l1];
		matrix[t1][l1] = matrix[t2][l2];
		matrix[t2][l2] = temp;
	}

	int getMin() {
		if (matrix[0][0] == INT_MAX) {
			return INT_MAX;
		}
		int min = matrix[0][0];
		matrix[0][0] = INT_MAX;
		bubbleDown();
		return min;
	}

	void bubbleDown() {
		int bot = 0, right = 0;
		while (true) {
			if (bot == row - 1 && right == col - 1) {
				return;
			}
			else if (bot == row - 1) {
				if (matrix[bot][right + 1] < matrix[bot][right]) {
					swap(bot, right, bot, right + 1);
					++right;
				}
				else {
					return;
				}
			}
			else if (right == col - 1) {
				if (matrix[bot + 1][right] < matrix[bot][right]) {
					swap(bot, right, bot + 1, right);
					++bot;
				}
				else {
					return;
				}
			}
			else{
				if (matrix[bot][right + 1] < matrix[bot][right] && matrix[bot + 1][right] < matrix[bot][right]) {
					if (matrix[bot][right + 1] < matrix[bot + 1][right]) {
						swap(bot, right, bot, right + 1);
						++right;
					}
					else {
						swap(bot, right, bot + 1, right);
						++bot;
					}
				}
				else if (matrix[bot][right + 1] < matrix[bot][right]) {
					swap(bot, right, bot, right + 1);
					++right;
				}
				else if (matrix[bot + 1][right] < matrix[bot][right]) {
					swap(bot, right, bot + 1, right);
					++bot;
				}
				else {
					return;
				}
			}
		}
	}

	void sort(int *arr, int s) {
		VNT sorted(s);
		for (int i = 0; i < s; ++i) {
			sorted.add(arr[i]);
		}
		for (int i = 0; i < s; ++i) {
			arr[i] = sorted.getMin();
		}
	}

	bool find(int i) {
		int r = row - 1, c = 0;
		while (r >= 0 && c < col) {
			if (matrix[r][c] == i) {
				return true;
			}
			else if (i < matrix[r][c]) {
				--r;
			}
			else {
				++c;
			}
		}
		return false;
	}

	~VNT() {
		for (int i = 0; i < row; ++i)
			delete[] matrix[i];
		delete[] matrix;
	}
};

int main() {
	VNT a(5, 4);
	for (int i = 0; i < 20; i++) {
		a.add(20 - i);
	}

	int arr[9] = { 2, 6, 9, 0, 3, 1, 8, 4, 7 };

	cout << "Unsorted array is:\n";
	for (int i = 0; i < 9; ++i) {
		cout << arr[i] << " ";
	}
	cout << endl << endl;

	a.sort(arr, 9);
	cout << "Sorted Array is:\n";
	for (int i = 0; i < 9; ++i) {
		cout << arr[i] << " ";
	}
	cout << endl << endl;

	cout << "a.find(i):" << endl;
	for (int i = 1; i <= 20; i += 2) {
		cout << i << ": " << (a.find(i) ? "True" : "False") << endl;
	}
	cout << endl;

	cout << "a.getMin(i)" << endl;
	for (int i = 0; i < 10; ++i) {
		cout << "Min is " << a.getMin() << endl;
	}
	cout << endl;

	cout << "a.find(i):" << endl;
	for (int i = 1; i <= 20; ++i) {
		cout << i << ": " << (a.find(i) ? "True" : "False") << endl;
	}
	return 0;
}