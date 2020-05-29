#ifndef BITNODE_H
#define BITNODE_H

#include <iostream>

using namespace std;
enum BTN_Type { INTERNAL, EXTERNAL };

template <typename K, typename V>
class BitTrieNode
{
public:
	BitTrieNode() { }
	BitTrieNode(K k, V v, BTN_Type ty) : keyStr(k), value(v), type(ty)
	{
		prev = next = parent = child = NULL;
	}
	~BitTrieNode() { }

	/*Initializers*/
	void setKeyStr(char k) { keyStr = k; }
	void setValue(V v) { value = v; }
	void setNext(BitTrieNode<K, V> *nxt) { next = nxt; }
	void setPrev(BitTrieNode<K, V> *pv) { prev = pv; }
	void setParent(BitTrieNode<K, V> *pr) { parent = pr; }
	void setChild(BitTrieNode<K, V> *chld) { child = chld; }
	void setType(BTN_Type btype) { type = btype; }

	/*Accessors*/
	K getKeyStr() { return keyStr; }
	V getValue() { return value; }
	BTN_Type getType() { return type; }
	BitTrieNode<K, V> *getPrev() { return prev; }
	BitTrieNode<K, V> *getNext() { return next; }
	BitTrieNode<K, V> *getParent() { return parent; }
	BitTrieNode<K, V> *getChild() { return child; }

	void _fprintBTN(ostream& fout, BitTrieNode<K, V> *pTN, int indent)
	{
		if (pTN == NULL)
		{
			fout << endl;
			return;
		}
		else
		{
			if (pTN->keyStr >= 0 && pTN->getType() != EXTERNAL)
				fout << pTN->keyStr - '1';
			_fprintBTN(fout, pTN->child, indent + 1);
			if (pTN->next == NULL)
			{
				return;
			}
			for (int i = 0; i < indent; i++)
			{
				fout << " ";
			}
			_fprintBTN(fout, pTN->next, indent);
		}
	}
private:
	K keyStr;
	V value;
	BTN_Type type;
	BitTrieNode<K, V> *prev;
	BitTrieNode<K, V> *next;
	BitTrieNode<K, V> *parent;
	BitTrieNode<K, V> *child;
};

#endif