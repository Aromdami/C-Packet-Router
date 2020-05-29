#include "Thread_Packet_Router.h"

#define FORWARDING_TRIE
unsigned __stdcall Thread_PacketRouter(LPVOID pParam)
{
	ThreadParam *pThrParam;
	BreadthFirstSearch* pNetTopology;
	DataLink ***pppDL;	//pppDL[u][v] = address of DataLink[u][v]
	Graph::Vertex *pVrtx, *pMyVrtx;
	int myNetAddr;
	Packet *pPkt, newPkt;
	DataLink* pDL;
	UINT_8 srcAddr;
	ostream *pFout;

	int next_node, dst;
	int *pParent;
	int *pForwardTable = NULL;
	BitTrie<UINT8, UINT8> bitTrie("BitTrie_PacketForwarding");
	int num_nodes;
	int num_packets_generated = 0;
	int *pTotal_received;

	/*Default Initilization*/
	pThrParam = (ThreadParam *)pParam;
	myNetAddr = pThrParam->addr;
	pNetTopology = pThrParam->pNetTopology;
	pFout = pThrParam->pFout;
	pppDL = pThrParam->pppDL;
	pTotal_received = pThrParam->pNum_total_received;

	EnterCriticalSection(pThrParam->pCS);
	cout << " ******* Thread_PacketRouter [" << myNetAddr << "] *******" << endl;
	LeaveCriticalSection(pThrParam->pCS);

	srand(time(0) + pThrParam->addr);
	pNetTopology->initialize();
	pNetTopology->initDistMtrx();

	pVrtx = pNetTopology->getGraph().getpVrtxArray();
	pMyVrtx = &(pVrtx[myNetAddr]);
	pNetTopology->ShortestPathsTree(*pMyVrtx);
	pParent = pNetTopology->getpParent();
	num_nodes = pNetTopology->getGraph().getNumVertices();

	// initialize packet forwarding table or trie
	EnterCriticalSection(pThrParam->pCS);
	pForwardTable = new int[num_nodes];

	for (dst = 0; dst < num_nodes; dst++)
	{
#ifdef FORWARDING_TABLE
		int nxt;
		nxt = dst;
		while (pParent[nxt] != myNetAddr)
			nxt = pParent[nxt];
		if (pParent[nxt] == myNetAddr)
		{
			pForwardTable[dst] = nxt;
		}
		else 
		{
			cout << "Error in packet forwarding : destination (" << dst;
			cout << ") is not reachable from this node (" << myNetAddr << "!!" << endl;
		}
#endif

#ifdef FORWARDING_TRIE
		int nxt;
		nxt = dst;
		while (pParent[nxt] != myNetAddr)
			nxt = pParent[nxt];

		if (pParent[nxt] == myNetAddr)
			bitTrie.insert(dst, nxt);
		else
		{
			cout << "Error in packet forwarding : destination (" << dst;
			cout << ") is not reachable from this node (" << myNetAddr << "!!" << endl;
		}
#endif

	}
	LeaveCriticalSection(pThrParam->pCS);

	// packet generations as source node
	for (int i = 0; i < num_nodes; i++)
	{
		dst = i % num_nodes;
		pPkt = new Packet(myNetAddr, dst, i);
		pPkt->pushRouteNode(myNetAddr);		// record route
#ifdef FORWARDING_TABLE 
		next_node = pForwardTable[dst];
#endif
#ifdef FORWARDING_TRIE
		next_node = bitTrie.longestPrefixMatch(dst);
#endif	

		EnterCriticalSection(pThrParam->pCS);
		DataLink* pDL = pThrParam->pppDL[myNetAddr][next_node];

		if (pDL != NULL)
			pDL->enqueue(*pPkt);
		else
			cout << " Error : pppDL[][] is NULL" << endl;

		*pFout << " Router[" << myNetAddr << "]: (" << num_packets_generated << ")-th packet(";
		*pFout << pPkt->getSrcAddr() << " → " << pPkt->getDstAddr() << ", seqNo: " << pPkt->getSeqNo() << endl;
		LeaveCriticalSection(pThrParam->pCS);

		num_packets_generated++;

		Sleep(1);
	}

	EnterCriticalSection(pThrParam->pCS);
	*pFout << " Router[" << myNetAddr << "] generated total " << num_packets_generated	<< " packets " << endl;
	LeaveCriticalSection(pThrParam->pCS);

	// packet forwarding as transit node and packet processing as destination node
	int source;
	int total_processed = 0;
	int received_by_this_node = 0;
	int hop_count;



	return 0;
}