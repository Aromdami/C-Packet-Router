#ifndef PACKETROUTER_H
#define PACKETROUTER_H

#include "SimParam.h"
#include "DataLink.h"
#include "Packet.h"
#include "BreadthFirstSearch.h"
#include "Graph.h"
#include "BitTrie.h"

#include <Windows.h>
#include <process.h>
#include <thread>

using namespace std;
#define THREAD_EXIT_CODE 0
enum ROLE { PACKET_ROUTER, PACKET_GEN, PACKET_RECV, LINK_TX, LINK_RX };

typedef struct ThreadParam
{
	CRITICAL_SECTION *pCS;
	DataLink ***pppDL;
	BreadthFirstSearch* pNetTopology;
	int num_queue;
	int id, role;
	UINT_8 addr;
	int max_queue;
	int *pNum_total_received;
	int *pTotal_packet_TX;
	ostream *pFout;
	ThreadParam() { }
}ThreadParam;

unsigned __stdcall Thread_PacketRouter(LPVOID pParam);

#endif