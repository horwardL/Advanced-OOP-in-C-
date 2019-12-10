//Program for Assignment 2 Polynomial Arithmetic
//Author : Han Wen Loh
//Date : 27 Sep 2019

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

class Node {
	int cof;
	int exp;
	Node* next;

	void parseTerm(string term, int &co, int& ex) {
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
			else if(term[i] == '^'){
				if (i-1 < 0 || term[i - 1] != 'x'){
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
	Node() {
		cof = 0;
		exp = 0;
		next = NULL;
	}

	Node(int c, int e, Node* n) {
		cof = c;
		exp = e;
		next = n;
	}

	void addNode(string term) {
		int co = 0;
		int ex = 0;
		parseTerm(term, co, ex);
		Node* cur = this;
		if (cur) {
			while (cur->next && cur->next->exp >= ex) {
				if (cur->next->exp == ex) {
					cur->next->cof += co;
					return;
				}
				cur = cur->next;
			}
			if (cur) {
				cur->next = new Node(co, ex, cur->next);
				return;
			}
		}
		next = new Node(co, ex, NULL);
	}

	void addNode(Node* newNode) {
		int co = newNode->cof;
		int ex = newNode->exp;
		Node* cur = this;
		if (cur) {
			while (cur->next && cur->next->exp >= ex) {
				if (cur->next->exp == ex) {
					cur->next->cof += co;
					return;
				}
				cur = cur->next;
			}
			if (cur) {
				cur->next = new Node(co, ex, cur->next);
				return;
			}
		}
		next = new Node(co, ex, NULL);
	}

	string toString() {
		string poly = "";
		Node* cur = next;
		while (cur) {
			if (poly.length() == 0) {
				if (cur->exp == 1) {
					if (cur->cof == 1) {
						poly += "x";
					}
					else if (cur->cof == -1) {
						poly += "-x";
					}
					else {
						poly += to_string(cur->cof) + "x";
					}
				}
				else if (cur->exp != 0) {
					if (cur->cof == 1) {
						poly += "x^" + to_string(cur->exp);
					}
					else if (cur->cof == -1) {
						poly += "-x^" + to_string(cur->exp);
					}
					else {
						poly += to_string(cur->cof) + "x^" + to_string(cur->exp);
					}
				}
				else {
					poly += to_string(cur->cof);
				}
			}
			else if (cur->cof == 1) {
				poly += "+";
				if (cur->exp == 1) {
					poly += "x";
				}
				else if (cur->exp != 0) {
					poly += "x^" + to_string(cur->exp);
				}
				else{
					poly += to_string(cur->cof);
				}
			}
			else if (cur->cof < 0) {
				if (cur->exp == 0) {
					poly+= to_string(cur->cof);
				}
				else {
					if (cur->cof == -1) {
						poly += "-";
					}
					else {
						poly += to_string(cur->cof);
					}
					if (cur->exp == 1) {
						poly += "x";
					}
					else{
						poly += "x^" + to_string(cur->exp);
					}
				}
			}
			else {
				poly += "+" + to_string(cur->cof);
				if (cur->exp == 1) {
					poly += "x";
				}
				else if (cur->exp != 0) {
					poly += "x^" + to_string(cur->exp);
				}
			}
			cur = cur->next;
		}
		return poly;
	}

	Node* addition(Node* head) {
		Node* result = new Node();
		Node* cur = result;
		Node* left = next;
		Node* right = head->next;
		Node* newNode = NULL;
		while (left || right) {
			if (left == NULL) {
				newNode = new Node(right->cof, right->exp, NULL);
				right = right->next;
			}
			else if (right == NULL) {
				newNode = new Node(left->cof, left->exp, NULL);
				left = left->next;
			}
			else if (right->exp > left->exp) {
				newNode = new Node(right->cof, right->exp, NULL);
				right = right->next;
			}
			else if (left->exp > right->exp) {
				newNode = new Node(left->cof, left->exp, NULL);
				left = left->next;
			}
			else if(left->exp == right->exp){
				if (left->cof + right->cof != 0) {
					newNode = new Node(left->cof + right->cof, left->exp, NULL);
				}
				else {
					newNode = NULL;
				}
				right = right->next;
				left = left->next;
			}
			if (newNode) {
				cur->next = newNode;
				cur = cur->next;
			}
		}
		return result;
	}

	Node* subtraction(Node* head) {
		Node* result = new Node();
		Node* cur = result;
		Node* left = next;
		Node* right = head->next;
		Node* newNode = NULL;
		while (left || right) {
			if (left == NULL) {
				newNode = new Node(right->cof * -1, right->exp, NULL);
				right = right->next;
			}
			else if (right == NULL) {
				newNode = new Node(left->cof, left->exp, NULL);
				left = left->next;
			}
			else if (right->exp > left->exp) {
				newNode = new Node(right->cof * -1, right->exp, NULL);
				right = right->next;
			}
			else if (left->exp > right->exp) {
				newNode = new Node(left->cof, left->exp, NULL);
				left = left->next;
			}
			else if (left->exp == right->exp) {
				if (left->cof - right->cof != 0) {
					newNode = new Node(left->cof - right->cof, left->exp, NULL);
				}
				else {
					newNode = NULL;
				}
				right = right->next;
				left = left->next;
			}
			if (newNode) {
				cur->next = newNode;
				cur = cur->next;
			}
		}
		return result;
	}

	Node* multiplication(Node* head) {
		Node* result = new Node();
		Node* left = next;
		Node* right = head->next;
		Node* newNode = NULL;
		while (left) {
			Node* right = head->next;
			while (right) {
				newNode = new Node(left->cof * right->cof, left->exp + right->exp, NULL);
				result->addNode(newNode);
				right = right->next;
			}
			left = left->next;
		}
		return result;
	}

	~Node() {
		delete next;
	}
};

int main() {
	ifstream infile("input.txt");
	ofstream outfile("output.txt");
	string line;
	Node* head1;
	Node* head2;
	Node* result;

	regex r("((-?)[\\d]*x((\\^(-?))[\\d]*)?)|((-?)[\\d]*)");
	smatch m;

	while (getline(infile, line)) {
		sregex_iterator currentMatch(line.begin(), line.end(), r);
		sregex_iterator lastMatch;
		head1 = new Node();
		while (currentMatch != lastMatch) {
			smatch m = *currentMatch;
			string term = m.str();
			// no exponent for this term
			if (term != "") {
				head1->addNode(term);
			}
			currentMatch++;
		}
		outfile << line << " = ";
		outfile << head1->toString() << endl;
		if (getline(infile, line)) {
			sregex_iterator currentMatch(line.begin(), line.end(), r);
			sregex_iterator lastMatch;
			head2 = new Node();
			while (currentMatch != lastMatch) {
				smatch m = *currentMatch;
				string term = m.str();
				// no exponent for this term
				if (term != "") {
					head2->addNode(term);
				}
				currentMatch++;
			}

			outfile << line << " = ";
			outfile << head2->toString() << endl;

			result = head1->addition(head2);
			outfile << "addition: " << result->toString() << endl;
			result = head1->subtraction(head2);
			outfile << "subtraction: " << result->toString() << endl;
			result = head1->multiplication(head2);
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