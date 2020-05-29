#ifndef DEPTHFIRSTSEARCH_H
#define DEPTHFIRSTSEARCH_H

#include "Graph.h"
#include <iostream>
#include <fstream>

using namespace std;

typedef Graph::Vertex Vertex;
typedef Graph::Edge Edge;
typedef std::list<Graph::Vertex> VrtxList;
typedef std::list<Graph::Vertex>::iterator VertexItor;
typedef std::list<Graph::Edge> EdgeList;
typedef std::list<Graph::Edge>::iterator EdgeItor;


class DepthFirstSearch
{
protected:
	Graph& graph;
	Vertex start;
	bool done;

protected:
	void initialize();
	void dfsTraversal(Vertex& v, Vertex& target, VrtxList& path);
	virtual void traverseDiscovery(const Edge& e, const Vertex& from) { }
	virtual void traverseBack(const Edge& e, const Vertex& from) { }
	virtual void finishVisit(const Vertex& v) {}
	virtual bool isDone() const { return done; }

	void visit(Vertex& v);
	void visit(Edge& e);
	void unvisit(Vertex& v);
	void unvisit(Edge& e);
	bool isVisited(Vertex& v);
	bool isVisited(Edge& e);

	void setEdgeStatus(Edge& e, EdgeStatus es);
	EdgeStatus getEdgeStatus(Edge& e);
	bool isValidVrtxID(int vid) { return graph.isValidVrtxID(vid); }
	int getNumVertices() { return graph.getNumVertices(); }

public:
	DepthFirstSearch(Graph& g);
	~DepthFirstSearch() {}
	void findPath(Vertex& s, Vertex& t, VrtxList& path);

	Graph& getGraph() { return graph; }
	void showConnectivity(ofstream& fout);

private:
	VrtxStatus* pVrtxStatus;
	EdgeStatus** ppEdgeStatus;
	int** ppConnectivity;
};
#endif