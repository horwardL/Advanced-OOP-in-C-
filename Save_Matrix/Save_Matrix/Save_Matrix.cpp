#include<iostream>
#include<cstdlib>
using namespace std;

template <typename T>
class SafeMatrix {
	int r_l, r_h, c_l, c_h;
	int rowSize, colSize;
	T** p;

public:

	//default constructor
	SafeMatrix() {
		r_l = 0;
		r_h = -1;
		c_l = 0;
		c_h = -1;
		rowSize = 0;
		colSize = 0;
		p = NULL;
	}

	//2 parameter constructor that take #row and #col
	SafeMatrix(int rl, int rh, int cl, int ch) {
		if (rh < rl || ch < cl) {
			cout << "constructor error in bounds definition" << endl;
			exit(1);
		}
		r_l = rl;
		r_h = rh;
		c_l = cl;
		c_h = ch;
		rowSize = rh - rl + 1;
		colSize = ch - cl + 1;
		p = new T * [rowSize];
		for (int i = 0; i < rowSize; ++i) {
			p[i] = new T[colSize];
		}
	}

	SafeMatrix(int rh, int ch) {
		if (rh <= 0 || ch <= 0) {
			cout << "constructor error in bounds definition" << endl;
			exit(1);
		}
		r_l = 0;
		r_h = rh-1;
		c_l = 0;
		c_h = ch-1;
		rowSize = rh;
		colSize = ch;
		p = new T * [rowSize];
		for (int i = 0; i < rowSize; ++i) {
			p[i] = new T[colSize];
		}
	}

	//copy constructor
	SafeMatrix(const SafeMatrix<T>& m) {
		r_l = m.r_l;
		r_h = m.r_h;
		c_l = m.c_l;
		c_h = m.c_h;
		rowSize = m.rowSize;
		colSize = m.colSize;
		p = new T * [rowSize];
		for (int i = 0; i < rowSize; ++i) {
			p[i] = new T[colSize];
			for (int j = 0; j < colSize; ++j) {
				p[i][j] = m.p[i][j];
			}
		}
	}

	//destructor
	~SafeMatrix() {
		for (int i = 0; i < rowSize; ++i) {
			delete[] p[i];
		}
		delete[] p;
	}

	friend class Proxy;

	class Proxy{
		T* ip;
		SafeMatrix<T> s;
	public:
		Proxy(T* ip1, SafeMatrix<T> sm) : ip(ip1), s(sm) { }

		T& operator[](int j) {
			if (j < s.c_l || j > s.c_h) {
				cout << "index " << j << " out of range" << endl;
				exit(1);
			}
			return ip[j-s.c_l];
		}
	};

	Proxy operator[](int i) {
		if (i < r_l || i > r_h) {
			cout << "index " << i << " out of range" << endl;
			exit(1);
		}
		return Proxy(p[i-r_l], *this);
	}


	SafeMatrix<T>& operator=(const SafeMatrix<T> & s) {
		if (this == &s) { return *this; }

		for (int i = 0; i < rowSize; ++i) {
			delete[] p[i];
		}
		delete[] p;

		r_l = s.r_l;
		r_h = s.r_h;
		c_l = s.c_l;
		c_h = s.c_h;
		rowSize = s.rowSize;
		colSize = s.colSize;

		p = new T * [rowSize];
		for (int i = 0; i < rowSize; ++i) {
			p[i] = new T[colSize];
			for (int j = 0; j < colSize; ++j) {
				p[i][j] = s.p[i][j];
			}
		}
		return *this;
	}

	SafeMatrix<T> operator+(const SafeMatrix<T>& s) {
		if (rowSize != s.rowSize || colSize != s.colSize) {
			cout << "addition error matrix has different size." << endl;
			exit(1);
		}
		SafeMatrix<T> t(rowSize, colSize);
		for (int i = 0; i < rowSize; ++i) {
			for (int j = 0; j < colSize; ++j) {
				t.p[i][j] = p[i][j] + s.p[i][j];
			}
		}
		return t;
	}

	SafeMatrix<T> operator-(const SafeMatrix<T>& s) {
		if (rowSize != s.rowSize || colSize != s.colSize) {
			cout << "subtraction error matrix has different size." << endl;
			exit(1);
		}
		SafeMatrix<T> t(rowSize, colSize);
		for (int i = 0; i < rowSize; ++i) {
			for (int j = 0; j < colSize; ++j) {
				t.p[i][j] = p[i][j] - s.p[i][j];
			}
		}
		return t;
	}

	SafeMatrix<T> operator*(const SafeMatrix<T>& s) {
		if (colSize != s.rowSize) {
			cout << "multiplication error matrix size do not match." << endl;
			exit(1);
		}
		SafeMatrix<T> t(rowSize, s.colSize);
		for (int i = 0; i < rowSize; ++i) {
			for (int j = 0; j < s.colSize; ++j) {
				t.p[i][j] = 0;
				for (int k = 0; k < colSize; ++k) {
					t.p[i][j] += p[i][k] * s.p[k][j];
				}
			}
		}
		return t;
	}

	template <class U>
	friend ostream& operator<<(ostream& os, SafeMatrix<U> s);
};

template<typename U>
ostream& operator<<(ostream& os, SafeMatrix<U> s) {
	for (int i = 0; i < s.rowSize; ++i) {
		for (int j = 0; j < s.colSize; ++j) {
			cout << s.p[i][j] << " ";
		}
		cout << endl;
	}
	return os;
};

int main() {
	SafeMatrix<double> a(0,0,-3,-1);
	/*for (int i = -2; i <= 2; ++i) {
		for (int j = -1; j <= 3; ++j) {
			a[i][j] = i+j;
		}
	}*/
	a[0][-3] = 3.1;
	a[0][-2] = 4.1;
	a[0][-1] = 2.1;

	cout << a << endl;
	SafeMatrix<double> b(3,4);
	/*for (int i = -2; i <= 2; ++i) {
		for (int j = -1; j <= 3; ++j) {
			b[i][j] = i - j;
		}
	}*/
	b[0][0] = 13.1;
	b[0][1] = 9.1;
	b[0][2] = 7.1;
	b[0][3] = 15.1;
	b[1][0] = 8.1;
	b[1][1] = 7.1;
	b[1][2] = 4.1;
	b[1][3] = 6.1;
	b[2][0] = 6.1;
	b[2][1] = 4.1;
	b[2][2] = 0.1;
	b[2][3] = 3.1;

	cout << b << endl;
	SafeMatrix<double> c = a * b;
	cout << c << endl;
}