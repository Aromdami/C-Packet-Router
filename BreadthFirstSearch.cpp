#include "BreadthFirstSearch.h"
#include "Graph.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <algorithm>

using namespace std;
enum BFS_PROCESS_STATUS { NOT_SELECTED, SELECTED };

void BreadthFirstSearch::initialize()
{
	int num_nodes = graph.getNumVertices();
	VrtxList vrtxLst;
	graph.vertices(vrtxLst);
	EdgeList edges;

	done = false;

	for (VertexItor pv = vrtxLst.begin(); pv != vrtxLst.end(); ++pv)
		unvisit(*pv);
	

	graph.edges(edges);
	for (EdgeItor pe = edges.begin(); pe != edges.end(); ++pe)
		unvisit(*pe);
}

void BreadthFirstSearch::initDistMtrx()
{
	int** ppDistMtrx;
	int num_nodes;
	Vertex* pVrtxArray;
	EdgeList* pAdjLstArray;
	int curVID, vID;

	num_nodes = graph.getNumVertices();
	pVrtxArray = graph.getpVrtxArray();
	pAdjLstArray = graph.getpAdjLstArray();

	ppDistMtrx = getppDistMtrx();
	
	for (int i = 0; i < num_nodes; i++)
	{
		curVID = pVrtxArray[i].getID();
		EdgeItor pe = pAdjLstArray[curVID].begin();

		while (pe != pAdjLstArray[curVID].end())
		{
			vID = (*(*pe).getpVrtx_2()).getID();
			ppDistMtrx[curVID][vID] = (*pe).getDistance();
			pe++;
		}
		ppDistMtrx[curVID][curVID] = 0;
	}
}

void BreadthFirstSearch::printDistMtrx(ostream &fout)
{
	int** ppDistMtrx;
	ppDistMtrx = getppDistMtrx();
	Vertex* pVrtxArray;
	pVrtxArray = graph.getpVrtxArray();
	int num_nodes = graph.getNumVertices();
	int dist;
	string vName;

	fout << "Distance Matrix of Graph : " << graph.getName() << endl;
	fout << "   |";

	for (int i = 0; i < num_nodes; i++)
	{
		vName = pVrtxArray[i].getName();
		fout << setw(5) << setfill(' ') << vName;
	}
	fout << endl;

	fout << "-----+";
	for (int i = 0; i < num_nodes; i++)
		fout << "-----";
	fout << endl;

	for (int i = 0; i < num_nodes; i++)
	{
		vName = pVrtxArray[i].getName();
		fout << setw(5) << setfill(' ') << vName << " | ";
		for (int j = 0; j < num_nodes; j++)
		{
			dist = ppDistMtrx[i][j];
			if (dist == PLUS_INF)
				fout << setw(5) << setfill(' ') << " +oo";
			else
				fout << setw(5) << setfill(' ') << dist;
		}
		fout << endl;
	}
	fout << endl;
}

void BreadthFirstSearch::bfsTraversal(Vertex& start, Vertex& target, VrtxList& path)
{
	int *nodeLevel;
	int num_nodes, num_visited;
	int* pPrev;
	int vID, start_vID, target_vID;

	Vertex vrtx, v, *pVrtxArray, *pVrtx;
	Edge e;
	VrtxList vrtxs;
	VertexItor vItor;
	EdgeList edges;
	EdgeItor eItor;

	if (start == target)
	{
		done = true;
		path.push_back(start);
		return;
	}

	/*Initialization*/
	pVrtxArray = graph.getpVrtxArray();
	start_vID = start.getID();
	target_vID = target.getID();
	num_nodes = graph.getNumVertices();
	pPrev = new int[num_nodes];
	nodeLevel = new int[num_nodes];

	for (int i = 0; i < num_nodes; i++)
	{
		nodeLevel[i] = -1;
		pPrev[i] = -1;
	}

	graph.vertices(vrtxs);
	visit(start);

	vItor = vrtxs.begin();
	pPrev[start_vID] = start_vID;
	num_visited = 0;
	int level = 0;
	nodeLevel[start_vID] = 0;

	/*Actual BFS*/
	while (num_visited < num_nodes)
	{
		cout << "Level (" << setw(2) << level << ") : ";
		vItor = vrtxs.begin();

		/*Finding "unvisitied" Vertex*/
		while (vItor != vrtxs.end())
		{
			vID = vItor->getID();
			if (isVisited(*vItor) && (nodeLevel[vID] == level))
			{
				cout << *vItor << ", "; 
				graph.incidentEdges(*vItor, edges);
				eItor = edges.begin();

				/*Visiting Vertex of other side*/
				while (eItor != edges.end())
				{
					pVrtx = eItor->getpVrtx_2();
					if (pVrtx->getvrtxStatus() != VISITED)
					{
						nodeLevel[pVrtx->getID()] = level + 1;
						pVrtx->setVrtxStatus(VISITED);
						eItor->setEdgeStatus(DISCOVERY);
						pPrev[pVrtx->getID()] = vItor->getID();
					}
					else {
						eItor->setEdgeStatus(CROSS);
					}
					++eItor;
				} 
				num_visited++;
			}
			++vItor;
		}
		cout << endl;
		if (num_visited >= num_nodes)
			break;
		level++;
	}
	vID = target_vID;
	/*Adding Path*/
	while (vID != start_vID)
	{
		pVrtx = &pVrtxArray[vID];
		path.push_front(*pVrtx);
		vID = pPrev[vID];
	}
	path.push_front(pVrtxArray[vID]);
}

void BreadthFirstSearch::findShortestPath(Vertex& s, Vertex& t, VrtxList& path)
{
	initialize();
	path.clear();

	initDistMtrx();
	bfsTraversal(s, t, path);
}

void BreadthFirstSearch::visit(Vertex& v)
{
	Graph::Vertex* pVrtx;
	int vID = v.getID();
	int numNodes = getGraph().getNumVertices();

	if (isValidVrtxID(vID))
	{
		pVrtx = getGraph().getpVrtxArray();
		pVrtx[vID].setVrtxStatus(VISITED);
	}
	else
		cout << "Vertex (" << v << ") ID is out-of-range (" << numNodes << ")" << endl;
}

void BreadthFirstSearch::unvisit(Vertex& v)
{
	int vID = v.getID();
	int numNodes = getGraph().getNumVertices();
	Vertex* pVrtx;
	pVrtx = getGraph().getpVrtxArray();

	if (isValidVrtxID(vID))
		pVrtx[vID].setVrtxStatus(UN_VISITED);
	else
		cout << "Vertex (" << v << ") ID is out-of-range (" << numNodes << ")" << endl;
}

void BreadthFirstSearch::visit(Edge& e)
{
	EdgeItor eItor;
	Graph::EdgeList* pEdgeLst;

	Vertex vrtx_1, vrtx_2;
	int vID_1, vID_2;
	int numNodes = getGraph().getNumVertices();

	vrtx_1 = *e.getpVrtx_1();
	vID_1 = vrtx_1.getID();
	vrtx_2 = *e.getpVrtx_2();
	vID_2 = vrtx_2.getID();

	if (isValidVrtxID(vID_1) && isValidVrtxID(vID_2))
	{
		pEdgeLst = getGraph().getpAdjLstArray();
		eItor = find(pEdgeLst[vID_1].begin(), pEdgeLst[vID_1].end(), e);

		if (eItor == pEdgeLst[vID_1].end())
			cout << "Edge (" << e << ") is not found from the graph !!" << endl;
		else
			(*eItor).setEdgeStatus(EDGE_VISITED);
	}
	else
	{
		cout << "Vertex IDs (" << vID_1 << ", " << vID_2;
		cout << ") of Edge (" << e << ") is out-of-range (" << numNodes << ")" << endl;
	}
}

void BreadthFirstSearch::unvisit(Edge& e)
{
	EdgeItor eItor;
	EdgeList* pAdjListArray;
	Vertex vrtx_1, vrtx_2;
	int vID_1, vID_2;
	int numNodes = getGraph().getNumVertices();

	vrtx_1 = *e.getpVrtx_1();
	vID_1 = vrtx_1.getID();
	vrtx_2 = *e.getpVrtx_2();
	vID_2 = vrtx_2.getID();

	if (isValidVrtxID(vID_1) && isValidVrtxID(vID_2))
	{
		pAdjListArray = getGraph().getpAdjLstArray();
		eItor = find(pAdjListArray[vID_1].begin(), pAdjListArray[vID_1].end(), e);

		if (eItor == pAdjListArray[vID_1].end())
			cout << "Edge (" << e << ") is not found from the graph !!" << endl;
		else
			(*eItor).setEdgeStatus(EDGE_UN_VISITED);
	}
	else
	{
		cout << "Vertex IDs (" << vID_1;
		cout << ") of Edge (" << e << ") is out-of-range (" << numNodes << ")" << endl;
	}
}	

bool BreadthFirstSearch::isVisited(Vertex& v)
{
	Vertex* pVrtx;
	Vertex* pVrtxArray;
	int numNodes = getGraph().getNumVertices();
	int vrtx_ID = v.getID();

	if (isValidVrtxID(vrtx_ID))
	{
		pVrtxArray = getGraph().getpVrtxArray();
		pVrtx = &(pVrtxArray[vrtx_ID]);
		return (pVrtx->getvrtxStatus() == VISITED);
	}
	else
	{
		cout << "Vertex (" << v << ") ID is out-of-range (" << numNodes << ")" << endl;
		return false;
	}
}

bool BreadthFirstSearch::isVisited(Edge& e)
{
	EdgeItor eItor;
	Vertex vrtx_1, vrtx_2;
	int vID_1, vID_2;
	int numNodes = getGraph().getNumVertices();
	EdgeList* pAdjListArray;
	EdgeStatus eStat;

	vrtx_1 = *e.getpVrtx_1();
	vID_1 = vrtx_1.getID();
	vrtx_2 = *e.getpVrtx_2();
	vID_2 = vrtx_2.getID();

	if (isValidVrtxID(vID_1) && isValidVrtxID(vID_2))
	{
		pAdjListArray = getGraph().getpAdjLstArray();
		eItor = find(pAdjListArray[vID_1].begin(), pAdjListArray[vID_1].end(), e);

		if (eItor == pAdjListArray[vID_1].end())
		{
			cout << "Edge (" << e << ") is not found from the graph !!" << endl;
			return false;
		}
		else
		{
			eStat = (*eItor).getEdgeStatus();
			if ((eStat == EDGE_VISITED) || (eStat == DISCOVERY) || (eStat == BACK))
				return true;
			else
				return false;
		}
	}
	else
	{
		cout << "Vertex IDs (" << vID_1 << ", " << vID_2 << ") of Edge (" << e << ") is out-of-range (" << numNodes << ")" << endl;
	}
	return false;
}

void BreadthFirstSearch::setEdgeStatus(Edge& e, EdgeStatus es)
{
	EdgeItor eItor;
	int vID_1 = e.getpVrtx_1()->getID(), vID_2 = e.getpVrtx_2()->getID();
	EdgeList* pAdjListArray;
	pAdjListArray = getGraph().getpAdjLstArray();

	eItor = pAdjListArray[vID_1].begin();

	while (eItor != pAdjListArray[vID_1].end())
	{
		if (*eItor == e)
			break;
		else
			eItor++;
	}
	if (eItor != pAdjListArray[vID_1].end())
		(*eItor).setEdgeStatus(es);
	else
	{
		cout << "Edge (" << e << ") was not found from AdjacencyList" << endl;
	}
}

EdgeStatus BreadthFirstSearch::getEdgeStatus(Edge& e)
{
	EdgeItor eItor;
	int vID_1 = e.getpVrtx_1()->getID(), vID_2 = e.getpVrtx_2()->getID();
	EdgeList* pAdjListArray;
	pAdjListArray = getGraph().getpAdjLstArray();

	eItor = pAdjListArray[vID_1].begin();

	while (eItor != pAdjListArray[vID_1].end())
	{
		if (*eItor == e)
			break;
		else
			eItor++;
	}
	if (eItor != pAdjListArray[vID_1].end())
		return (*eItor).getEdgeStatus();
	else
	{
		cout << "Edge (" << e << ") was not found from AdjacencyList" << endl;
		return EDGE_NOT_FOUND;
	}
}

/*****************Dajikstra*****************/
void BreadthFirstSearch::dijkstraBFS(Vertex& s, int* pPrev)
{
	int num_nodes = graph.getNumVertices();
	int** ppDistMtrx;
	ppDistMtrx = getppDistMtrx();
	int* pLeastCost;
	pLeastCost = new int[num_nodes];
	int num_selected;
	int minID, minCost;

	BFS_PROCESS_STATUS* pBFS_Process_Stat;
	pBFS_Process_Stat = new BFS_PROCESS_STATUS[num_nodes];
	Vertex* pVrtxArray;
	pVrtxArray = graph.getpVrtxArray();
	EdgeList* pAdjLstArray;
	pAdjLstArray = graph.getpAdjLstArray();
	
	Vertex vrtx,  v;
	Edge e;
	int start_vrtxid = start.getID();

	for (int i = 0; i < num_nodes; i++)
	{
		pLeastCost[i] = ppDistMtrx[start_vrtxid][i];
		pPrev[i] = start_vrtxid;
		pBFS_Process_Stat[i] = NOT_SELECTED;
	}
	/*Default Initilization Over*/

	pBFS_Process_Stat[start_vrtxid] = SELECTED;
	num_selected = 1;
	int round = 0;
	int cost;

	/*Start Algorithm*/
	cout << "Least Cost from start at each round : " << endl;
	while (num_selected < num_nodes)
	{
		round++;
		cout << "round [" << setw(2) << round << "] ";
		
		/* find current node with LeastCost*/
		minID = -1;
		minCost = PLUS_INF;
		for (int i = 0; i < num_nodes; i++)
		{
			if ((pLeastCost[i] < minCost) && (pBFS_Process_Stat[i] != SELECTED)) 
			{
				minID = i;
				minCost = pLeastCost[i];
			}
		}

		if (minID == -1) //Could not find  node with Least cost
		{
			cout << "Error in Dijkstra() -- could not find connected vertex !!" << endl;
			break;
		}
		else 
		{
			pBFS_Process_Stat[minID] = SELECTED;
			num_selected++; 
			//Removed codes : Change in parameters.
			/*
			if (minID == target_vrtxid)
			{
			cout << "reached to the target node !!" << endl;
			cout << "Least Cost = " << minCost << endl;
			vrtxID = minID;
			do {
			vrtx = pVrtxArray[vrtxID];
			path.push_front(vrtx);
			vrtxID = pPrev[vrtxID];
			} while (vrtxID != start_vrtxid);
			vrtx = pVrtxArray[vrtxID];
			path.push_front(vrtx); // start node
			break;
			}
			*/
		}

		int ppDistMtrx_i;		// i -th Distance
		for (int i = 0; i < num_nodes; i++)
		{
			ppDistMtrx_i = ppDistMtrx[minID][i];
			/*Find unselected node and see distance by way*/
			if ((pBFS_Process_Stat[i] != SELECTED) &&(pLeastCost[i] > (pLeastCost[minID] + ppDistMtrx[minID][i])))
			{
				pPrev[i] = minID;
				pLeastCost[i] = pLeastCost[minID] + ppDistMtrx[minID][i];
			}
		}
		//Monitoring Distance for Debugs
		for (int i = 0; i < num_nodes; i++)
		{
			cost = pLeastCost[i];
			if (cost == PLUS_INF)
				cout << setw(2) << i << " +oo, ";
			else
				cout << setw(2) << i << setw(5) << pLeastCost[i] << ", ";
		} cout << endl;
	}
}

void BreadthFirstSearch::ShortestPathsTree(Vertex& start)
{
	int start_id = start.getID();
	initialize();

	std::list<int> path;
	std::list<int>::iterator pItor;

	int *prev;
	int num_nodes = graph.getNumVertices();
	prev = new int[num_nodes];

	Vertex *pVrtxArray;
	pVrtxArray = graph.getpVrtxArray();
	Vertex vrtx,  v;	

	for (int i = 0; i < num_nodes; i++)
		prev[i] = start_id;

	initDistMtrx();
	dijkstraBFS(start, prev);

	cout << "Previous Node in the Shortest  Path : " << endl;
	for (int i = 0; i < num_nodes; i++)
		cout << setw(2) << prev[i] << " ¡ç " << setw(2) << i << " , ";
	cout << endl;

	int j;
	for (int i = 0; i < num_nodes; i++)
	{
		cout << "Shortest Path from " << start_id << " to " << i << " : ";
		path.clear();

		j = i;
		
		/*Adding route in start - path - target*/
		do
		{
			path.push_front(j);
			j = prev[j];
		} while (j != start_id);
		path.push_front(start_id);

		/*Print Path in order*/
		pItor = path.begin();
		do
		{
			cout << *pItor;
			pItor++;

			if (pItor == path.end())
				break;

			else if (*pItor == start_id)
				break;

			else
				cout << " ¡æ ";
		} while (pItor != path.end());
		cout << endl;
	}
}