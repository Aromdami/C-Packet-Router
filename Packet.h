#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <fstream>

#define MAX_HOP_COUNT 16

typedef unsigned char UINT_8;

using namespace std;

class Packet
{
public:
	Packet() { }
	Packet(UINT_8 sA, UINT_8 dA, UINT_8 sN) : srcAddr(sA), dstAddr(dA), seqNo(sN) { }
	~Packet() { }
	/*Accessors*/
	UINT_8 getSrcAddr() { return srcAddr; }
	UINT_8 getDstAddr() { return dstAddr; }
	UINT_8 getProtocol() { return protocol; }
	UINT_8 getSeqNo() { return seqNo; }
	UINT_8 getHopCount() { return hop_count; }
	UINT_8 getRouteNodes(int h) { return route_nodes[h]; }

	/*Initializers*/
	void setSrcAddr(UINT_8 sA) { srcAddr = sA; }
	void setDstAddr(UINT_8 dA) { dstAddr = dA; }
	void setProtocol(UINT_8 proto) { protocol = proto; }
	void setSeqNo(UINT_8 sN) { seqNo = sN; }
	void setHopCount(UINT_8 hc) { hop_count = hc; }
	void setRouteNodes(int h, int rn) { route_nodes[h] = rn; }

	void pushRouteNode(int rn) { route_nodes[hop_count++] = rn; }
	ostream& printSaddrSeqNo(ostream *pFout)
	{
		*pFout << "[Src : " << srcAddr << "] ¢º Dst : " << dstAddr << ", Seq No : " << seqNo;
	}
	ostream& printPacket(ostream *pFout)
	{
		for (int i = 0; i < payloadLength; i++)
			*pFout << route_nodes[i];
	}

private:
	UINT_8 srcAddr;	// source address
	UINT_8 dstAddr;	// destination address
	UINT_8 protocol;
	UINT_8 seqNo;		// sequence number
	UINT_8 payloadLength;
	int hop_count;		// prevent sending more than "hop_count" datas. Ignore when exceeded.
	int route_nodes[MAX_HOP_COUNT]; // payload
};

#endif