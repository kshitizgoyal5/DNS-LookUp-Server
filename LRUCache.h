#pragma once
#include "Includes.h"

namespace dns {
	const int MAX_CHAR = 27;

	class node {
	public:
		node() {
			_next = nullptr;
		}

		node(std::string& key, std::string& val) {
			_val = val;
			_key = key;
			_next = nullptr;
		}
		std::string getVal() { return _val; }
		std::string getKey() { return _key; }

		node* _next;
		std::string _val, _key;		
		
	};

	class trie {
	public:
		trie() {
			for (int i = 0; i < MAX_CHAR; i++) {
				child.push_back(nullptr);
			}
			end = false;
			ptr = nullptr;
			entries = 0;
		}

		~trie() {

		}

		int getIndex(char& c) {
			if (c == '.') return 26;
			else {
				int x = c - 'a';
				if (x >= 0 && x < 26) return x;
				return 0;
			}
		}

		bool add(std::string& s, node *p, int idx=0) {
			//end of string 
			if (idx == s.size()) {
				ptr = p;
				if (end) {
					// already exist
					return false;
				}
				entries++;
				end = true;
				return true;
			}
			int i = getIndex(s[idx]);
			if (child[i] == nullptr) {
				child[i] = new trie;
			}
			if (child[i]->add(s, p, idx + 1)) {
				entries++;
				return true;
			}
			return false;
		}

		node* getIP(std::string& name, int idx=0) {
			if (idx == name.size()) {
				node* copy = nullptr;
				if (end) copy = ptr;
				return copy;
			}
			int i = getIndex(name[idx]);
			if (child[i] == nullptr) {
				return nullptr;
			}
			return child[i]->getIP(name, idx + 1);
		}

		bool deleteIP(std::string& name, int idx=0) {
			if (idx == name.size()) {
				if (end) {
					end = false;
					ptr = nullptr;
					entries--;
					return true;
				}
				return false;
			}
			int i = getIndex(name[idx]);
			if (child[i] == nullptr) {
				return false;
			}
			if (child[i]->deleteIP(name, idx + 1)) {
				entries--;
				if (child[i]->entries == 0) {
					delete child[i];
					child[i] = nullptr;
				}
				return true;
			}
			return false;
		}

	private:
		std::vector<trie*> child;
		bool end;
		int entries;
		node* ptr;
	};

	class LRUCache
	{
	public:
		LRUCache(int capacity) {
			_capacity = capacity;
			_size = 0;
			start = nullptr;
			end = nullptr;
		}

		std::string getIP(std::string& name) {
			node* p = _map.getIP(name);
			if (p == nullptr) return "";
			if (p == end) return p->getVal();
			node* q = p->_next;
			if (q == end) {
				end = p;
			}
			std::string nextKey = q->getKey(), a = p->getKey(), b = p->getVal();
			*p = *q;
			delete q;
			addAtEnd(a, b);
			_map.add(nextKey, p);
			_map.add(name, end);
			return end->getVal();
		}

		void addEntry(std::string& name, std::string& ip) {
			node* p = _map.getIP(name);
			if (p == nullptr) {
				if (_size == 0) {
					start = new node(name, ip);
					end = start;
				}
				else addAtEnd(name, ip);
				_size++;
				_map.add(name, end);
			}
			else if (p == end) {
				p->_val = ip;
			}
			else {
				_size++;
				node* q = p->_next;
				if (q == end) end = p;
				std::string nextKey = q->getKey();
				*p = *q;
				delete q;
				addAtEnd(name, ip);
				_map.add(nextKey, p);
				_map.add(name, end);
			}
			if (_size > _capacity) {
				std::string key = start->getKey();
				std::cout << "deleted " << key << std::endl;
				_map.deleteIP(key);
				if (start == end) {	
					delete start;
					start = nullptr;
					end = nullptr;
				}
				else {
					node* p = start->_next;
					if (p == end) end = start;
					*start = *p;
					delete p;
				}
				_size--;
			}
		}

	private:

		void addAtEnd(std::string& name, std::string& ip) {
			end->_next = new node(name, ip);
			end = end->_next;
		}

	private:
		trie _map;
		int _capacity;
		int _size;
		node* start, * end;
	};


}


