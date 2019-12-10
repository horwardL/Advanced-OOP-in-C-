#include <iostream>
#include <sstream>
#include <iterator>
#include <map>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class IncertedIndex {
private:
	map<string, vector<vector<int>>> index;
public:
	IncertedIndex() {};

	void getIndex(ifstream& infile) {
		string line;
		string word;
		int line_index = 0;
		int position_index = 0;
		while (getline(infile, line)) {
			istringstream iss(line);
			position_index = 0;
			while (iss >> word) {
				word.erase(remove_if(word.begin(), word.end(), ispunct), word.end());
				if (word.length() > 0) {
					index[word].push_back({ line_index, position_index });
					++position_index;
				}
			}
			++line_index;
		}
	}

	void toString() {
		for (auto itr = index.begin(); itr != index.end(); ++itr) {
			string word = itr->first;
			vector<vector<int>> positions = itr->second;
			cout << word << " " << positions.size() << " |";
			string p = "";
			for (auto const& v : positions) {
				p += " (" + to_string(v[0]) + "," + to_string(v[1]) + "),";
			}
			p.pop_back();
			cout << p << endl;
		}
	}
};

int main() {
	ifstream infile;
	infile.open("Gettysburg Address.txt");
	IncertedIndex *obj = new IncertedIndex();
	obj->getIndex(infile);
	obj->toString();
	infile.close();
	return 0;
}