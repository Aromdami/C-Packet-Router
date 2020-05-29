#ifndef BFS_H
#define BFS_H

#include "Graph.h"

using namespace std;

class BreadthFirstSearch
{
protected:
	typedef Graph::Vertex Vertex;
	typedef Graph::Edge Edge;
	typedef std::list<Graph::Vertex> VrtxList;
	typedef std::list<Graph::Vertex>::iterator VertexItor;
	typedef std::list<Graph::Edge> EdgeList;
	typedef std::list<Graph::Edge>::iterator EdgeItor;


private:
	Graph& graph;
	Vertex start;
	bool done;				// flag of search :  done
	int **ppDistMtrx;	// distance matrix
	int *pLeastCost;		//least cost to the destination from this (start) node
	int *pParent;			// previous vertex in shortest path tree

protected:
	void bfsTraversal(Vertex& start, Vertex& target, VrtxList& path);
	void dijkstraBFS(Vertex& s, int* pParent);
	virtual bool isDone() const { return done; }

	/*Basic Utilities*/
	bool isValidVrtxID(int vid) { return graph.isValidVrtxID(vid); }
	void visit(Vertex& v);
	void visit(Edge& e);

	void unvisit(Vertex& v);
	void unvisit(Edge& e);

	bool isVisited(Vertex& v);
	bool isVisited(Edge& e);

	void setEdgeStatus(Edge& e, EdgeStatus es);
	EdgeStatus getEdgeStatus(Edge& e);

public:
	BreadthFirstSearch(Graph& g) :graph(g) 
	{
		int num_nodes = g.getNumVertices();

		/*Initialize Distance Matrix*/
		for (int i = 0; i < num_nodes; i++)
			ppDistMtrx = new int*[num_nodes];
		
		for (int i = 0; i < num_nodes; i++)
			ppDistMtrx[i] = new int[num_nodes];

		for (int i = 0; i < num_nodes; i++) 
		{
			for (int j = 0; j < num_nodes; j++) 
				ppDistMtrx[i][j] = PLUS_INF;
		}

		/*Initialize Least Cost Array*/
		pLeastCost = new int[num_nodes];
		for (int i = 0; i < num_nodes; i++)
			pLeastCost[i] = PLUS_INF;

		pParent = new int[num_nodes];
		for (int i = 0; i < num_nodes; i++) 
			pParent[i] = -1;
	}

	void initialize();
	void initDistMtrx();
	void printDistMtrx(ostream &fout);

	void findShortestPath(Vertex& s, Vertex& t, VrtxList& path);
	void ShortestPathsTree(Vertex& start);
	Graph& getGraph() { return graph; }

	int** getppDistMtrx() { return ppDistMtrx; }
	int* getpParent() { return pParent; }
	int* getpLeastCost() { return pLeastCost; }
};

#endif