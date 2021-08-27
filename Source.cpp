#include "Includes.h"
#include "LRUCache.h"

#include <time.h>
#include <chrono>
using namespace std;

int MAX_CHARS = 27;

//class trie {
//public:
//	trie() {
//		for (int i = 0; i < 27; i++) {
//			child[i] = nullptr;
//		}
//	}
//
//	~trie() {
//	}
//
//	int getIndex(char& c) {
//
//		if (c == '.') return 26;
//		else {
//			int x = c - 'a';
//			if (x >= 0 && x < 26) return x;
//			return 0;
//		}
//	}
//
//	bool add(string& s, int idx, string& ip) {
//		if (idx == s.size()) {
//			ip_address = ip;
//			if (end) return false;
//			end = true;
//			return true;
//		}
//
//		int i = getIndex(s[idx]);
//		if (child[i] == nullptr) {
//			child[i] = new trie;
//		}
//		if (child[i]->add(s, idx + 1, ip)) {
//			entries++;
//			return true;
//		}
//		return false;
//	}
//
//	string getIP(string& name, int idx) {
//		if (idx == name.size()) {
//			string copy;
//			if (end) copy = ip_address;
//			else {
//				copy = "NOT_FOUND";
//			}
//			return copy;
//		}
//		int i = getIndex(name[idx]);
//		if (child[i] == nullptr) {
//			string copy = "NOT_FOUND";
//			return copy;
//		}
//		return child[i]->getIP(name, idx + 1);
//	}
//
//	bool deleteIP(string& name, int idx) {
//		if (idx == name.size()) {
//			if (end) {
//				end = false;
//				ip_address = "";
//				if (entries == 0) this->~trie();
//				else entries--;
//				return true;
//			}
//			return false;
//		}
//		int i = getIndex(name[idx]);
//		if (child[i] == nullptr) {
//			return false;
//		}
//		if (child[i]->deleteIP(name, idx + 1)) {
//			entries--;
//			if (entries == 0) this->~trie();
//			return true;
//		}
//		return false;
//	}
//
//	void printAll(string& rec) {
//		if (end) cout << rec << endl;
//		for (int i = 0; i < 27; i++) {
//			if (child[i] != nullptr) {
//				rec.push_back(char((i == 26 ? '.' : i + 'a')));
//				child[i]->printAll(rec);
//				rec.pop_back();
//			}
//		}
//	}
//	void print() {
//		string s;
//		printAll(s);
//	}
//
//private:
//	trie* child[27];
//	bool end = false;
//	int entries = 0;
//	string ip_address;
//};

string randomString(int MAX_SIZE) {
	int size = 1 + rand() % MAX_SIZE;
	string s;
	for (int i = 0; i < size; i++) {
		s += char('a' + rand() % 26);
	}
	return s;
}

pair<long long, long long> func(int size) {
	// generate array of random string {ie domain names}
	vector<string> temp(size, randomString(20));
	// cache capacity set as 10 percent of total number of domains 
	dns::LRUCache c(size/10);

	// randomly set the cache state
	for (int i = 0; i < size / 10; i++) {
		c.addEntry(temp[rand() % size], temp[0]);
	}

	// avg time to execute getIP function
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (int i = 0; i < size; i++) {
		c.getIP(temp[rand() % size]);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	auto a = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / size;

	// avg time to execute addEntry function
	std::chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();
	for (int i = 0; i < size; i++) {
		c.addEntry(temp[rand() % size], temp[0]);
	}
	std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();
	auto b = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1).count() / size;

	return make_pair(a, b);
}

int main() {
	for (int ii = 0; ii < 10; ii++) {
		vector<pair<long long, long long>> temp;
		for (int i = 100; i < 1e6; i *= 10) {
			temp.push_back(func(i));
		}
		for (auto i : temp) {
			cout << i.first << " ";
		}
		cout << endl;
		for (auto i : temp) {
			cout << i.second << " ";
		}
		cout << endl << endl;
	}
}