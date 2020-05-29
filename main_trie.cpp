#include "BitTrie.h"
#include "BitTrieNode.h"

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <bitset>

using namespace std;
typedef unsigned char UINT8;
#define NUM_SAMPLE_TEXTS 5

typedef struct
{
	UINT8 dest_addr;
	int output_port;
} RoutingEntry;

void main()
{
	BitTrie<UINT8, RoutingEntry*> bitTrie_pktFwrd("BitTrie_PacketForwarding");
	BitTrieNode<UINT8, RoutingEntry*> *pTN;
	int word_count;
	UINT8 prefix;
	RoutingEntry *pResult;
	List_pStr predictTexts;
	List_pStr_Iter itr;

	/* Testing Basic Operation in BitTrie for routing table */
	RoutingEntry routing_table[] =
	{
	{ 0x12, 1 },
	{ 0x45, 3 },
	{ 0x23, 2 },
	{ 0xAB, 5 },
	{ 0x89, 4 }
	};

	cout << "Inserting keywords (texts) into " << bitTrie_pktFwrd.getName() << " ..... " << endl;
	for (int i = 0; i < NUM_SAMPLE_TEXTS; i++)
	{
		prefix = routing_table[i].dest_addr;
		bitTrie_pktFwrd.insert(prefix, &routing_table[i]);
		cout << "Destination addr (" << bitset<8>(prefix) << ") with output port ("<< routing_table[i].output_port << ") is inserted " << endl;
	}

	cout << endl << "Trie Structure of" << bitTrie_pktFwrd.getName() << " (size : " << bitTrie_pktFwrd.size() << ") : ";
	bitTrie_pktFwrd.fprintBitTrie(cout);
	cout << endl;

	/* testing keyWord search in trie */
	for (int i = 0; i < NUM_SAMPLE_TEXTS; i++)
	{
		prefix =  i;
		pResult = bitTrie_pktFwrd.longestPrefixMatch(prefix, 3);
		if (pResult != NULL)
			cout << "Result of longest prefix matching for (" << bitset<3>(prefix) << ") : output port(" << pResult->output_port << ")" << endl;
		else
			cout << "Result of longest prefix matching for (" << bitset<3>(prefix) << ") was not	found !!" << endl;
	}

	cout << endl << "Erasing BitTrie : " << bitTrie_pktFwrd.getName()<< endl;
	bitTrie_pktFwrd.eraseBitTrie();
	bitTrie_pktFwrd.fprintBitTrie(cout);
}