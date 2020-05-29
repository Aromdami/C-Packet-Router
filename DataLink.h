#ifndef DATALINK_H
#define DATALINK_H
#include "Packet.h"

#include <queue>
#include <Windows.h>

using namespace std;

class DataLink
{
private:
	CRITICAL_SECTION queue_mutex;		// for input & output ops
	queue<Packet> pktQ;

public:
	DataLink(void) { InitializeCriticalSection(&queue_mutex); }
	~DataLink() { }

	void enqueue(Packet pkt)
	{
		EnterCriticalSection(&queue_mutex);
		pktQ.push(pkt);
		LeaveCriticalSection(&queue_mutex);
	}
	
	void dequeue()
	{
		EnterCriticalSection(&queue_mutex);
		if (pktQ.empty())
		{
			LeaveCriticalSection(&queue_mutex);
			return;
		}
		else
			pktQ.pop();
		LeaveCriticalSection(&queue_mutex);
	}

	void front(Packet *pPkt)
	{
		EnterCriticalSection(&queue_mutex);
		if (pktQ.empty())
		{
			pPkt = NULL;
			LeaveCriticalSection(&queue_mutex);
			return;
		}
		*pPkt = pktQ.front();
		LeaveCriticalSection(&queue_mutex);
	}

	bool empty()
	{
		int size = pktQ.size();
		if (size == 0)
			return true;
		else
			return false;
	}
};

#endif