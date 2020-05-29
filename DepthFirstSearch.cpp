#include "Graph.h"
#include "DepthFirstSearch.h"

#include <iostream>
#include <list>
#include <algorithm>

DepthFirstSearch::DepthFirstSearch(Graph& g) : graph(g)
{
	int num_nodes = graph.getNumVertices();

	pVrtxStatus = new VrtxStatus[num_nodes];
	for (int i = 0; i < num_nodes; i++)
		pVrtxStatus[i] = UN_VISITED;

	ppEdgeStatus = new EdgeStatus*[num_nodes];
	for (int i = 0; i < num_nodes; i++)
		ppEdgeStatus[i] = new EdgeStatus[num_nodes];
	for (int i = 0; i < num_nodes; i++)
		for (int j = 0; j < num_nodes; j++)
			ppEdgeStatus[i][j] = EDGE_UN_VISITED;

	ppConnectivity = new int*[num_nodes];
	for (int i = 0; i < num_nodes; i++)
		ppConnectivity[i] = new int[num_nodes];
	for (int i = 0; i < num_nodes; i++)
		for (int j = 0; j < num_nodes; j++)
			ppConnectivity[i][j] = PLUS_INF;

	Vertex vrtx_1, vrtx_2;
	int vID_1, vID_2;
	EdgeList edges;
	edges.clear();
	graph.edges(edges);
	for (EdgeItor pe = edges.begin(); pe != edges.end(); ++pe)
	{
		vrtx_1 = *(*pe).getpVrtx_1();
		vID_1 = vrtx_1.getID();
		vrtx_2 = *(*pe).getpVrtx_2();
		vID_2 = vrtx_2.getID();
		ppConnectivity[vID_1][vID_2] = (*pe).getDistance();
	}
	for (	int i = 0; i < num_nodes; i++)
		ppConnectivity[i][i] = 0;
}

void DepthFirstSearch::initialize()
{
	int num_nodes = graph.getNumVertices();
	VrtxList verts;
	graph.vertices(verts);
	Vertex vrtx_1, vrtx_2;

	done = false;

	for (int i = 0; i < num_nodes; i++)
		pVrtxStatus[i] = UN_VISITED;

	for (int i = 0; i < num_nodes; i++)
		for (int j = 0; j < num_nodes; j++)
			ppEdgeStatus[i][j] = EDGE_UN_VISITED;
}

void DepthFirstSearch::showConnectivity(ofstream& fout)
{
	int num_nodes = graph.getNumVertices();
	int dist;

	fout << "Connectivity of graph: " << endl;
	fout << "   |";

	for (int i = 0; i < num_nodes; i++)
		fout << setw(5) << setfill(' ') << (char)(i + 'A');
	fout << endl;

	fout << "-----+";
	for (int i = 0; i < num_nodes; i++)
		fout << "-----";
	fout << endl;

	for (int i = 0; i < num_nodes; i++)
	{
		fout << " " << (char)(i + 'A') << " | ";
		for (int j = 0; j < num_nodes; j++)
		{
			dist = ppConnectivity[i][j];
			if (dist == PLUS_INF)
				fout << setw(5)<< setfill(' ')<<" +oo";
			else
				fout << setw(5) << setfill(' ') << dist;

		}
		fout << endl;
	}
}

void DepthFirstSearch::dfsTraversal(Vertex& v, Vertex& target, VrtxList& path)
{
	visit(v);
	if (v == target)
	{
		done = true;
		return;
	}
	EdgeList incidentEdges;
	incidentEdges.clear();
	graph.incidentEdges(v, incidentEdges);
	EdgeItor pe = incidentEdges.begin();
	VertexItor vi = path.begin();

	int count = 0;

	while (!isDone() && pe != incidentEdges.end())
	{
		Edge e = *pe++;
		
		EdgeStatus eStat = getEdgeStatus(e);
		if (eStat == EDGE_UN_VISITED)
		{
			visit(e);
			Vertex w = e.opposite(v);

			if (!isVisited(w))
			{
				path.push_back(w);
				setEdgeStatus(e, DISCOVERY);
				if (!isDone())
				{
					dfsTraversal(w, target, path);
					if (!isDone())
					{
						traverseBack(e, v);
						Vertex last_pushed = path.back();
						path.pop_back();
						
					}
				}
			}
			else
			{
				setEdgeStatus(e, BACK);
			}
			
		}
		
	}

}

void DepthFirstSearch::findPath(Vertex& s, Vertex& t, VrtxList& path)
{
	initialize();
	path.clear();

	path.push_back(s);
	dfsTraversal(s, t, path);
}

void DepthFirstSearch::visit(Vertex& v)
{
	int numNodes = getGraph().getNumVertices();
	int vrtx_ID = v.getID();

	if(isValidVrtxID(vrtx_ID))
		pVrtxStatus[vrtx_ID] = VISITED;
	else
	{
		cout << "Vertex (" << v << ") ID is out-of-range (";
		cout << numNodes << ")" << endl;
	}
}

void DepthFirstSearch::unvisit(Vertex& v)
{
	int numNodes = getGraph().getNumVertices();
	int vrtx_ID = v.getID();

	if (isValidVrtxID(vrtx_ID))
		pVrtxStatus[vrtx_ID] = UN_VISITED;
	else
	{
		cout << "Vertex (" << v << ") ID is out-of-range (";
		cout << numNodes << ")" << endl;
	}
}

void DepthFirstSearch::visit(Edge& e)
{
	Vertex vrtx_1, vrtx_2;
	int vID_1, vID_2;
	int numNodes = getGraph().getNumVertices();

	vrtx_1 = *e.getpVrtx_1();
	vID_1 = vrtx_1.getID();
	vrtx_2 = *e.getpVrtx_2();
	vID_2 = vrtx_2.getID();

	if (isValidVrtxID(vID_1) && isValidVrtxID(vID_2))
		ppEdgeStatus[vID_1][vID_2] = EDGE_VISITED;
	else
	{
		cout << "Vertex IDs (" << vID_1 << ", " << vID_2;
		cout << ") of Edge (" << e << ") is out-of-range (" << numNodes << ")" << endl;
	}
}

void DepthFirstSearch::unvisit(Edge& e)
{
	Vertex vrtx_1, vrtx_2;
	int vID_1, vID_2;
	int numNodes = getGraph().getNumVertices();

	vrtx_1 = *e.getpVrtx_1();
	vID_1 = vrtx_1.getID();
	vrtx_2 = *e.getpVrtx_2();
	vID_2 = vrtx_2.getID();

	if (isValidVrtxID(vID_1) && isValidVrtxID(vID_2))
		ppEdgeStatus[vID_1][vID_2] = EDGE_UN_VISITED;
	else
	{
		cout << "Vertex IDs (" << vID_1 << ", " << vID_2;
		cout << ") of Edge (" << e << ") is out-of-range (" << numNodes << ")" << endl;
	}
}

bool DepthFirstSearch::isVisited(Vertex& v)
{
	int numNodes = getGraph().getNumVertices();
	int vrtx_ID = v.getID();

	if (isValidVrtxID(vrtx_ID))
		return (pVrtxStatus[vrtx_ID] == VISITED);
	else
	{
		cout << "Vertex (" << v << ") ID is out-of-range ("<< numNodes << ")" << endl;
		return false;
	}
}

bool DepthFirstSearch::isVisited(Edge& e)
{
	Vertex vrtx_1, vrtx_2;
	int vID_1, vID_2;
	int numNodes = getGraph().getNumVertices();
	EdgeStatus eStat;

	vrtx_1 = *e.getpVrtx_1();
	vID_1 = vrtx_1.getID();
	vrtx_2 = *e.getpVrtx_2();
	vID_2 = vrtx_2.getID();

	if (isValidVrtxID(vID_1) && isValidVrtxID(vID_2))
	{
		eStat = ppEdgeStatus[vID_1][vID_2];
		if ((eStat == EDGE_VISITED) || (eStat == DISCOVERY) || (eStat == BACK))
			return true;
		else
			return false;
	}
	else
	{
		cout << "Vertex IDs (" << vID_1 << ", " << vID_2 << ") of Edge (" << e << ") is out-of-range (" << numNodes << ")" << endl;
	}
	return false;
}

void DepthFirstSearch::setEdgeStatus(Edge& e, EdgeStatus es)
{
	Vertex vrtx_1, vrtx_2;
	int vID_1, vID_2;
	int numNodes = getGraph().getNumVertices();

	vrtx_1 = *e.getpVrtx_1();
	vID_1 = vrtx_1.getID();
	vrtx_2 = *e.getpVrtx_2();
	vID_2 = vrtx_2.getID();

	if (isValidVrtxID(vID_1) && isValidVrtxID(vID_2))
		ppEdgeStatus[vID_1][vID_2] = es;
	else
	{
		cout << "Vertex IDs (" << vID_1 << ", " << vID_2 << ") of Edge (" << e << ") is out-of-range (" << numNodes << ")" << endl;
	}
}

EdgeStatus DepthFirstSearch::getEdgeStatus(Edge& e)
{
	Vertex vrtx_1, vrtx_2;
	int vID_1, vID_2;
	int numNodes = getGraph().getNumVertices();

	vrtx_1 = *e.getpVrtx_1();
	vID_1 = vrtx_1.getID();
	vrtx_2 = *e.getpVrtx_2();
	vID_2 = vrtx_2.getID();

	if (isValidVrtxID(vID_1) && isValidVrtxID(vID_2))
		return ppEdgeStatus[vID_1][vID_2];
	else 
	{
		cout << "Edge (" << e << ") was not found from AdjacencyList" << endl;
		return EDGE_NOT_FOUND;
	}
}