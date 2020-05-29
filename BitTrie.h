#ifndef Trie_H
#define Trie_H

#include "BitTrieNode.h"
#include <iostream>
#include <string>
#include <list>

typedef list<string*> List_pStr;
typedef list<string*>::iterator List_pStr_Iter;
enum SearchMode { FULL_MATCH, PREFIX_MATCH };

#define VALUE_INTERNAL_NODE NULL

template <typename K, typename V>
class BitTrie
{
public:
	BitTrie() { }
	BitTrie(string name);
	~BitTrie() { }
	
	int size() { return num_keyWords; }
	string getName() { return trie_name; }

	void insert(K keyStr, V value);
	void insertExternalTN(BitTrieNode<K, V> *pTN, K keyStr, V value);
	
	BitTrieNode<K, V> *find(K keyStr);
	V longestPrefixMatch(K prefix, int bitPos);

	void deleteKeyStr(K keyStr);
	void eraseBitTrie();

	void fprintBitTrie(ostream& fout);

protected:
	BitTrieNode<K, V> *_find(K keyStr, SearchMode sm = FULL_MATCH, int bitPos = 7);
	int getBit(K keyStr, int bitPos) { return ((keyStr >> bitPos) & 0x1); }

private:
	BitTrieNode<K, V> *_root;
	int num_keyWords;
	string trie_name;
};

template <typename K, typename V>
BitTrie<K, V>::BitTrie(string name)
{
	trie_name = name;
	_root = new BitTrieNode<K, V>(0, 0, EXTERNAL);
	_root->setPrev(NULL);
	_root->setNext(NULL);
	_root->setParent(NULL);
	_root->setChild(NULL);
	//cout << "Testing of Root" << endl << "Type : " << _root->getType() << endl << "KeyStr : " << _root->getKeyStr() << endl << "Value :" << _root->getValue() << endl;
	num_keyWords = 0;
}

template <typename K, typename V>
void BitTrie<K, V>::insertExternalTN(BitTrieNode<K, V> *pTN, K keyStr, V value)
{
	BitTrieNode<K, V> *pTN_New = NULL;
	pTN_New = new BitTrieNode<K, V>('0', value, EXTERNAL);
	pTN->setChild(pTN_New);
	(pTN->getChild())->setParent(pTN);
	pTN_New->setValue(value);
}

template<typename K, typename V>
void BitTrie<K, V>::insert(K keyStr, V value)
{
	BitTrieNode<K, V> *pTN = NULL, *pTN_New = NULL;
	int bitPos = sizeof(K) * 8 - 1;

	if (keyStr < 0)
		return;

	if (_find(keyStr, FULL_MATCH) != NULL)		//추가하려는 키워드가 이미 존재함
	{
		cout << "The given key already exists!! : " << keyStr << endl;
		return;
	}
	pTN = this->_root;

	while ((pTN != NULL) && (bitPos != -1))		//트라이의 끝, 단어글자의 끝까지
	{
		if ((pTN->getKeyStr() < getBit(keyStr, bitPos) + '1') && (pTN->getNext() == NULL) && (bitPos != -1))
			break;

		while ((pTN->getKeyStr() < getBit(keyStr, bitPos) + '1') && (pTN->getNext() != NULL))
			pTN = pTN->getNext();

		while ((pTN != NULL) && (pTN->getKeyStr() == getBit(keyStr, bitPos) + '1') && (bitPos != -1))
		{
			pTN = pTN->getChild();
			bitPos--;
		}

		if ((pTN->getKeyStr() > getBit(keyStr, bitPos) + '1') && (bitPos != -1))
			break;
	}

	if ((pTN->getType() == INTERNAL) && (getBit(keyStr, bitPos) + '1' == VALUE_INTERNAL_NODE))	//추가하려는 단어가 sub-string인 경우
	{
		/*기존에 존재하는, 겹치는 글자들을 제외시켜야함*/
		pTN_New = new BitTrieNode<K, V>(0, value, EXTERNAL);

		pTN_New->setParent(pTN->getParent());
		pTN->getParent()->setChild(pTN_New);

		pTN_New->setNext(pTN);
		pTN->setPrev(pTN_New);

		this->num_keyWords++;
		return;
	}

	else if ((pTN->getKeyStr() < getBit(keyStr, bitPos) + '1') && (pTN->getNext() == NULL) && (bitPos != -1)) //추가만 하면 되는 상황
	{
		pTN_New = new BitTrieNode<K, V>(getBit(keyStr, bitPos) + '1', VALUE_INTERNAL_NODE, INTERNAL);
		pTN_New->setParent(pTN->getParent());

		pTN_New->setPrev(pTN);
		pTN->setNext(pTN_New);

		pTN = pTN_New;
		bitPos--;

		while (bitPos != -1)
		{
			pTN_New = new BitTrieNode<K, V>(getBit(keyStr, bitPos) + '1', VALUE_INTERNAL_NODE, INTERNAL);
			pTN->setChild(pTN_New);

			pTN->getChild()->setParent(pTN);
			pTN = pTN->getChild();
			bitPos--;
		}

		if (bitPos == -1)
		{
			insertExternalTN(pTN, keyStr, value);
			this->num_keyWords++;
			return;
		}
	}
	else if ((pTN->getKeyStr() > getBit(keyStr, bitPos) + '1') && (bitPos != -1))			//기존에 있는 두 노드들 사이에 추가
	{
		pTN_New = new BitTrieNode<K, V>(getBit(keyStr, bitPos) + '1', VALUE_INTERNAL_NODE, INTERNAL);
		pTN_New->setNext(pTN);
		pTN_New->setParent(pTN->getParent());
		
		if (pTN->getPrev() == NULL)
		{
			if (pTN->getParent() != NULL)
				(pTN->getParent())->setChild(pTN_New);
			else
			{
				pTN_New->setType(EXTERNAL);
			}
		}
		else
			pTN->getPrev()->setNext(pTN_New);

		pTN_New->setPrev(pTN->getPrev());
		pTN->setPrev(pTN_New);

		pTN = pTN_New;
		bitPos--;

		while (bitPos != -1)
		{
			pTN_New = new BitTrieNode<K, V>(getBit(keyStr, bitPos) + '1', VALUE_INTERNAL_NODE, INTERNAL);
			cout << "Inserted : " << pTN_New->getKeyStr() << endl;
			pTN->setChild(pTN_New);

			pTN->getChild()->setParent(pTN);
			pTN = pTN->getChild();
			bitPos--;
		}

		if (bitPos == -1)
		{
			insertExternalTN(pTN, keyStr, value);
			this->num_keyWords++;
			return;
		}
	}
}

template <typename K, typename V>
void BitTrie<K, V>::deleteKeyStr(K keyStr)
{
	BitTrieNode<K. V> *pTN = NULL, *_root;
	BitTrieNode<K. V> *tmp = NULL;
	int trie_val;

	_root = this->_root;
	if (_root == NULL || keyStr < 0)
		return;

	pTN = _find(keyStr, FULL_MATCH);

	if (pTN == NULL)
	{
		cout << "Key [" << keyStr << "] not found in trie" << endl;
		return;
	}

	while (1)
	{
		if (pTN == NULL)
			break;

		if (pTN->getPrev() && pTN->getNext())				//전 후로 단어들이 있음
		{
			tmp = pTN;
			pTN->getNext()->setPrev(pTN->getPrev());
			pTN->getPrev()->setNext(pTN->getNext());
			free(tmp);
			break;
		}
		else if (pTN->getPrev() && !(pTN->getNext()))	//끝에 있는 단어임
		{
			tmp = pTN;
			pTN->getPrev()->setNext(NULL);
			free(tmp);
			break;
		}
		else if (!(pTN->getPrev()) && pTN->getNext())
		{
			tmp = pTN;
			pTN->getParent()->setChild(pTN->getNext());
			pTN->getNext()->setPrev(NULL);

			pTN = pTN->getNext();
			free(tmp);
			break;
		}
		else
		{
			tmp = pTN;
			pTN = pTN->getParent();

			if (pTN != NULL)
				pTN->setChild(NULL);

			free(tmp);
			if ((pTN == _root) && (pTN->getNext() == NULL) && (pTN->getPrev() == NULL))
			{
				cout << "Now, The Trie is [EMPTY]." << endl;
				break;
			}
		}
	}
	this->num_keys--;
}

template <typename K, typename V>
void BitTrie<K, V>::eraseBitTrie()
{
	BitTrieNode<K, V> *pTN;
	BitTrieNode<K, V> *pTN_to_be_deleted = NULL;

	if (this->_root == NULL)
		return;

	pTN = this->_root;

	while (pTN != NULL)
	{
		while ((pTN != NULL) && (pTN->getNext()))
			pTN = pTN->getNext();

		while (pTN->getChild())
		{
			if (pTN->getNext())
				break;
			pTN = pTN->getChild();
		}
		if (pTN->getNext())
			continue;

		if (pTN->getPrev() && pTN->getNext())
		{
			pTN_to_be_deleted = pTN;
			pTN->getNext()->setPrev(pTN->getPrev());
			pTN->getPrev()->setNext(pTN->getNext());

			pTN = pTN->getNext();
			free(pTN_to_be_deleted);
		}
		else if (pTN->getPrev() && !(pTN->getNext()))
		{
			pTN_to_be_deleted = pTN;
			pTN->getPrev()->setNext(NULL);
			pTN = pTN->getPrev();
			free(pTN_to_be_deleted);
		}
		else if (!(pTN->getPrev()) && pTN->getNext())
		{
			pTN_to_be_deleted = pTN;
			pTN->getParent()->setChild(pTN->getNext());
			pTN->getNext()->setPrev(NULL);

			pTN = pTN->getNext();
			free(pTN_to_be_deleted);
		}
		else
		{
			pTN_to_be_deleted = pTN;
			if (pTN == this->_root)
			{
				this->num_keyWords = 0;
				return;
			}
			if (pTN->getParent() != NULL)
			{
				pTN = pTN->getParent();
				pTN->setChild(NULL);
			}
			else
				pTN = pTN->getPrev();

			free(pTN_to_be_deleted);
		}
	}
}

template <typename K, typename V>
BitTrieNode<K, V>* BitTrie<K, V>::find(K keyString)
{
	_find(keyString);
}
template<typename K, typename V>
BitTrieNode<K,V> * BitTrie<K, V>::_find(K keyStr, SearchMode sm, int bitPos)
{
	BitTrieNode<K,V> *pTN = NULL;

	if (keyStr < 0)
		return NULL;

	pTN = this->_root;
	while ((pTN != NULL) && (bitPos != -1))
	{
		cout << "BIT NO." << bitPos << "\t";
		cout << "Current Search Value is : " << getBit(keyStr, bitPos);
		cout << "\tCurrent Target Value is : " << pTN->getKeyStr() -'1' << endl;
		while ((pTN != NULL) && (pTN->getKeyStr() < getBit(keyStr, bitPos)+'1'))
		{
			if (pTN->getNext() == NULL)
				return NULL;

			pTN = pTN->getNext();
		}
		if ((pTN != NULL) && (pTN->getKeyStr() > getBit(keyStr, bitPos) + '1'))		//못찾고 지나침
			return NULL;

		else if ((pTN == NULL) && (bitPos != -1))			//못찾고 끝까지 감
			return NULL;

		else if ((pTN->getType() != EXTERNAL) && (getBit(keyStr, bitPos) + '1' == pTN->getKeyStr()) && (bitPos != -1))
		{
			pTN = pTN->getChild();
			bitPos--;

			if ((bitPos == -1) && (pTN != NULL))
			{
				if (sm == FULL_MATCH)
				{
					if (pTN->getType() == EXTERNAL)	//완전히 일치하는걸 찾음
						return pTN;
					else											//앞글자만 같고 일치하는건 없음 (등록된 단어가 더 긴 경우임)
						return NULL;
				}
				else if (sm == PREFIX_MATCH)	//조금이라도 같은거 나오면 바로 반환
					return pTN;
			}

			else if (pTN == NULL)
				return NULL;

			else if ((pTN->getType() == EXTERNAL) && (bitPos != -1))
			{
				if ((pTN->getNext() != NULL))
				{
					pTN = pTN->getNext();
					continue;
				}
				else
					return NULL;
			}
			else
				continue;
		}
		else if ((pTN->getType() == EXTERNAL) && (pTN->getNext() != NULL))
		{
			pTN = pTN->getNext();
		}
		else if ((pTN->getType() == EXTERNAL) && (bitPos != -1) && (pTN->getNext() == NULL))
			return NULL;
		
	}
}

template <typename K, typename V>
V BitTrie<K, V>::longestPrefixMatch(K prefix, int bitPos)
{
	BitTrieNode<K, V> *pPtr = NULL;
	BitTrieNode<K, V> *pTN = NULL;

	if (prefix < 0)
		return NULL;

	pTN = this->_root;
	pTN = _find(prefix, PREFIX_MATCH, bitPos-1);

	if (pTN == NULL)
		return NULL;

	while (1)
	{
		if (pTN->getChild() == NULL)
			return pTN->getValue();

		else
			pTN = pTN->getChild();
	}
}

template <typename K, typename V>
void BitTrie<K, V>::fprintBitTrie(ostream &fout)
{
	BitTrieNode<K, V> *pTN;
	int line = 1, indent = 0;

	if (this->num_keyWords == 0)
	{
		fout << "Trie is Empty!" << endl;
		return;
	}
	fout << endl;
	pTN = this->_root;
	pTN->_fprintBTN(fout, pTN, indent);
}


#endif