//Program for Assignment 2 Polynomial Arithmetic
//Author : Han Wen Loh
//Date : 27 Sep 2019

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <algorithm>

using namespace std;

class Poly {
	map<int, int> poly; //key = exp, value = cof

	void parseTerm(string term, int& co, int& ex) {
		bool e = false;
		int cof_positive = 1;
		int exp_positive = 1;
		int num_x = 0;

		for (int i = 0; i < term.length(); i++) {
			if (num_x > 1) {
				cout << "invalid polynomial." << endl;
				exit(1);
			}
			if (e) {
				if (term[i] == '-') {
					exp_positive = -1;
				}
				else if (isdigit(term[i])) {
					ex = ex * 10 + (term[i] - '0');
				}
				else {
					cout << "invalid polynomial." << endl;
					exit(1);
				}
			}
			else if (term[i] == '-') {
				cof_positive = -1;
			}
			else if (isdigit(term[i])) {
				co = co * 10 + (term[i] - '0');
			}
			else if (term[i] == 'x') {
				num_x++;
				if (co == 0) {
					co = 1;
				}
				if (i + 1 >= term.length()) {
					ex = 1;
				}
			}
			else if (term[i] == '^') {
				if (i - 1 < 0 || term[i - 1] != 'x') {
					cout << "invalid polynomial." << endl;
					exit(1);
				}
				if (i + 1 >= term.length() || !isdigit(term[i + 1])) {
					cout << "invalid polynomial." << endl;
					exit(1);
				}
				e = true;
			}
			else {
				cout << "invalid polynomial." << endl;
				exit(1);
			}
		}
		co *= cof_positive;
		ex *= exp_positive;
	}

public:
	Poly() {}

	void addNode(string term) {
		int co = 0;
		int ex = 0;
		parseTerm(term, co, ex);
		poly[ex] += co;
	}

	string toString() {
		string p = "";
		for (auto it = poly.begin(); it != poly.end(); ++it) {
			int exp = it->first;
			int cof = it->second;
			if (cof < -1 or cof > 1) {
				if (exp == 0) {
					p = to_string(cof) + p;
				}
				else if (exp == 1) {
					p = to_string(cof) + "x" + p;
				}
				else {
					p = to_string(cof) + "x^" + to_string(exp) + p;
				}
			}
			else if (cof == -1 or cof == 1) {
				if (exp == 0) {
					p = to_string(cof) + p;
				}
				else if (exp == 1) {
					if (cof == -1)
						p = "-x" + p;
					else
						p = "x" + p;
				}
				else {
					if (cof == -1)
						p = "-x^" + to_string(exp) + p;
					else
						p = "x^" + to_string(exp) + p;
				}
			}
			else if (cof < 0) {
				if (exp == 0) {
					p = to_string(cof) + p;
				}
				else if (exp == 1) {
					p = to_string(cof) + "x" + p;
				}
				else {
					p = to_string(cof) + "x^" + to_string(exp) + p;
				}
			}
			if (cof > 0) {
				p = "+" + p;
			}
		}
		p.erase(0, 1);
		return p;
	}

	Poly* addition(const Poly* poly2) {
		Poly *result = new Poly();
		auto left = poly.begin();
		auto right = poly2->poly.begin();

		while (left != poly.end() && right != poly2->poly.end()) {
			if (left->first < right->first) {
				result->poly[left->first] = left->second;
				++left;
			}else if (right->first < left->first) {
				result->poly[right->first] = right->second;
				++right;
			}
			else {
				result->poly[left->first] = left->second + right->second;
				++left;
				++right;
			}
		}

		while (left != poly.end()) {
			result->poly[left->first] = left->second;
			++left;
		}

		while (right != poly2->poly.end()) {
			result->poly[right->first] = right->second;
			++right;
		}

		return result;
	}

	Poly* subtraction(const Poly* poly2) {
		Poly* result = new Poly;
		auto left = poly.begin();
		auto right = poly2->poly.begin();

		while (left != poly.end() && right != poly2->poly.end()) {
			if (left->first < right->first) {
				result->poly[left->first] = left->second;
				++left;
			}
			else if (right->first < left->first) {
				result->poly[right->first] = right->second * -1;
				++right;
			}
			else {
				if(left->second - right->second != 0)
					result->poly[left->first] = left->second - right->second;
				++left;
				++right;
			}
		}

		while (left != poly.end()) {
			result->poly[left->first] = left->second;
			++left;
		}

		while (right != poly2->poly.end()) {
			result->poly[right->first] = right->second * -1;
			++right;
		}

		return result;
	}

	Poly* multiplication(const Poly* poly2) {
		Poly* result = new Poly();

		for (auto left = poly.begin(); left != poly.end(); ++left) {
			for (auto right = poly2->poly.begin(); right != poly2->poly.end(); ++right) {
				result->poly[left->first + right->first] += left->second * right->second;
			}
		}

		return result;
	}
};

int main() {
	ifstream infile("input.txt");
	ofstream outfile("output.txt");
	string line;
	Poly *poly1;
	Poly *poly2;
	Poly *result;

	regex r("((-?)[\\d]*x((\\^(-?))[\\d]*)?)|((-?)[\\d]*)");
	smatch m;

	while (getline(infile, line)) {
		sregex_iterator currentMatch(line.begin(), line.end(), r);
		sregex_iterator lastMatch;
		poly1 = new Poly();
		while (currentMatch != lastMatch) {
			smatch m = *currentMatch;
			string term = m.str();
			// no exponent for this term
			if (term != "") {
				poly1->addNode(term);
			}
			currentMatch++;
		}
		outfile << line << " = ";
		outfile << poly1->toString() << endl;
		if (getline(infile, line)) {
			sregex_iterator currentMatch(line.begin(), line.end(), r);
			sregex_iterator lastMatch;
			poly2 = new Poly();
			while (currentMatch != lastMatch) {
				smatch m = *currentMatch;
				string term = m.str();
				// no exponent for this term
				if (term != "") {
					poly2->addNode(term);
				}
				currentMatch++;
			}

			outfile << line << " = ";
			outfile << poly2->toString() << endl;

			result = poly1->addition(poly2);
			outfile << "addition: " << result->toString() << endl;
			result = poly1->subtraction(poly2);
			outfile << "subtraction: " << result->toString() << endl;
			result = poly1->multiplication(poly2);
			outfile << "multiplication: " << result->toString() << endl << endl;
		}
		else {
			cout << "missing polynimial.\n";
		}
	}
	infile.close();
	outfile.close();
	return 0;
}