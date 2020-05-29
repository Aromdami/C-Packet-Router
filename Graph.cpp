#include "Graph.h"

using namespace std;

void Graph::vertices(VrtxList& vrtxLst)
{
	vrtxLst.clear();
	
	for (int i = 0; i < getNumVertices(); i++)
		vrtxLst.push_back(pVrtxArray[i]);
}

void Graph::edges(EdgeList& edges)
{
	EdgeItor eItor;
	Graph::Edge e;

	edges.clear();
	for (int i = 0; i < getNumVertices(); i++)
	{
		eItor = pAdjLstArray[i].begin();

		while (eItor != pAdjLstArray[i].end())
		{
			e = *eItor;
			edges.push_back(e);
			eItor++;
		}
	}
}

bool Graph::isAdjacentTo(Vertex v, Vertex w)
{
	if (pVrtxArray[v.getID()] == pVrtxArray[w.getID()])
		return true;
	else
		return false;
}

void Graph::insertVertex(Vertex& v)
{
	int vrtx_ID = v.getID();

	if (pVrtxArray[vrtx_ID] == NULL) 
		pVrtxArray[vrtx_ID] = v;
}

void Graph::eraseVertex(Vertex v)
{
	if (pVrtxArray[v.getID()] != NULL)
		pVrtxArray[v.getID()] = NULL;
}

void Graph::insertEdge(Edge& e)
{
	Vertex vrtx_1, vrtx_2;
	Vertex* pVtx;
	int vID_1, vID_2;

	vrtx_1 = *e.getpVrtx_1();
	vrtx_2 = *e.getpVrtx_2();
	vID_1 = vrtx_1.getID();
	vID_2 = vrtx_2.getID();

	if (pVrtxArray[vID_1] == NULL)
		pVrtxArray[vID_1] = vrtx_1;

	if (pVrtxArray[vID_2] == NULL)
		pVrtxArray[vID_2] = vrtx_2;

	e.setpVrtx_1(&pVrtxArray[vID_1]);
	e.setpVrtx_2(&pVrtxArray[vID_2]);

	pAdjLstArray[vID_1].push_back(e);
}

void Graph::eraseEdge(Edge e)
{
	Vertex vrtx_1, vrtx_2;
	Vertex* pVtx;
	int vID_1, vID_2;

	vrtx_1 = *e.getpVrtx_1();
	vrtx_2 = *e.getpVrtx_2();
	vID_1 = vrtx_1.getID();
	vID_2 = vrtx_2.getID();
	
	pAdjLstArray[vID_1].clear();

	if (pVrtxArray[vID_1] != NULL)
		pVrtxArray[vID_1] = NULL;
	if (pVrtxArray[vID_2] != NULL)
		pVrtxArray[vID_2] = NULL;
}

void Graph::incidentEdges(Vertex v, EdgeList& edges)
{
	Graph::Edge e;
	EdgeItor eItor;
	int vrtx_id = v.getID();

	eItor = pAdjLstArray[vrtx_id].begin();
	while (eItor != pAdjLstArray[vrtx_id].end())
	{
		e = *eItor;
		edges.push_back(e);
		eItor++;
	}
}

bool Graph::isValidVrtxID(int vid)
{
	if (vid < 0 || vid >= num_vertices)
		return false;
	else
		return true;
}

void Graph::fprintGraph(ofstream &fout)
{
	EdgeItor eItor;
	Graph::Edge e;
	int numOutgoingEdges;

	for (int i = 0; i < num_vertices; i++)
	{
		fout <<"Vertex("<< setw(2) << (char)(i + 'A') << ") : ";
		numOutgoingEdges = pAdjLstArray[i].size();
		eItor = pAdjLstArray[i].begin();

		while (eItor != pAdjLstArray[i].end())
		{
			e = *eItor;
			fout << "Edges["<<e << "]  ";
			eItor++;
		}
		fout << endl;
	}
}