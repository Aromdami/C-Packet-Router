#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;

#define PLUS_INF INT_MAX/2
enum VrtxStatus { UN_VISITED, VISITED, VRTX_NOT_FOUND };
enum EdgeStatus { DISCOVERY, BACK, CROSS, EDGE_UN_VISITED, EDGE_VISITED, EDGE_NOT_FOUND };

class Graph
{
public:
	class Vertex;
	class Edge;

	typedef std::list<Graph::Vertex> VrtxList;
	typedef std::list<Graph::Edge> EdgeList;
	typedef std::list<Vertex>::iterator VrtxItor;
	typedef std::list<Edge>::iterator EdgeItor;

public:
	Graph() : name(""), pVrtxArray(NULL), pAdjLstArray(NULL) { }
	Graph(string nm, int num_nodes) : name(nm), pVrtxArray(NULL), pAdjLstArray(NULL)
	{
		typedef Edge* EdgePtr;
		num_vertices = num_nodes;
		pVrtxArray = new Graph::Vertex[num_vertices];

		for (int i = 0; i < num_nodes; i++)
			pVrtxArray[i] = NULL;
		
		pAdjLstArray = new EdgeList[num_vertices];

		for (int i = 0; i < num_vertices; i++)
			pAdjLstArray[i].clear();
	}
	~Graph() { }
	/*Graph Accessor*/
	string getName() { return name; }
	int getNumVertices() { return num_vertices; }
	Vertex* getpVrtxArray() { return pVrtxArray; }
	EdgeList* getpAdjLstArray() { return pAdjLstArray;}

	void vertices(VrtxList& vrtxLst);
	void edges(EdgeList&);
	bool isAdjacentTo(Vertex v, Vertex w);

	void insertVertex(Vertex& v);
	void eraseVertex(Vertex v);
	void insertEdge(Edge& e);
	void eraseEdge(Edge e);
	

	void incidentEdges(Vertex v, EdgeList& edges);
	bool isValidVrtxID(int vid);
	void fprintGraph(ofstream& fout);

private:
	string name;
	Vertex* pVrtxArray;
	EdgeList* pAdjLstArray;
	int num_vertices;

	///////////////////////////////
	//////VERTEX CLASS//////
	///////////////////////////////

public:
	class Vertex
	{
		friend ostream& operator<<(ostream& fout, Vertex v)
		{
			fout << v.getName();
			return fout;
		}

	public:
		Vertex() : name(), ID(-1) {}
		Vertex(string n, int id) : name(n), ID(id) { }
		Vertex(int id) : ID(id) { }
		~Vertex() { }

		/*Vertex Initializer*/
		void setName(string c_name) { name = c_name; }
		void setID(int id) { ID = id; }
		void setVrtxStatus(VrtxStatus vs) { vrtxStatus = vs; }

		/*Vertex Accesor*/
		string getName() { return name; }
		int getID() { return ID; }
		VrtxStatus getvrtxStatus() { return vrtxStatus; }

		bool operator==(Vertex v) { return ((ID == v.getID()) && (name == v.getName())); }
		bool operator!=(Vertex v) { return ((ID != v.getID()) || (name != v.getName())); }

	private:
		string name;
		int ID;
		VrtxStatus vrtxStatus;
	};

	///////////////////////////////
	////////EDGE CLASS///////
	///////////////////////////////

	class Edge
	{
		friend ostream& operator<<(ostream& fout, Edge& e)
		{
			fout <<*e.getpVrtx_1()<<", "<<*e.getpVrtx_2()<<", " <<e.getDistance();
			return fout;
		}
	public:
		Edge() : pVrtx_1(NULL), pVrtx_2(NULL), distance(PLUS_INF) { }
		Edge(Vertex& v1, Vertex& v2, int d) :distance(d), pVrtx_1(&v1), pVrtx_2(&v2), edgeStatus(EDGE_UN_VISITED) { }
		~Edge() { }

		void endVertices(VrtxList& vrtxLst)
		{
			vrtxLst.push_back(*pVrtx_1);
			vrtxLst.push_back(*pVrtx_2);
		}
		Vertex opposite(Vertex v)
		{
			if (v == *pVrtx_1)
				return *pVrtx_2;
			else if (v == *pVrtx_2)
				return *pVrtx_1;
			else
				return Vertex(NULL);
		}

		/*Edge Initializer*/
		void setpVrtx_1(Vertex* pV) { pVrtx_1 = pV; }
		void setpVrtx_2(Vertex* pV) { pVrtx_2 = pV; }
		void setDistance(int d) { distance = d; }
		void setEdgeStatus(EdgeStatus es) { edgeStatus = es; }

		/*Edge Accessor*/
		Vertex* getpVrtx_1() { return pVrtx_1; }
		Vertex* getpVrtx_2() { return pVrtx_2; }
		int getDistance() { return distance; }
		EdgeStatus getEdgeStatus() { return edgeStatus; }

		bool operator!=(Edge e) { return ((pVrtx_1 != e.getpVrtx_1()) || (pVrtx_2 != e.getpVrtx_2())); }
		bool operator==(Edge e) { return ((pVrtx_1 == e.getpVrtx_1()) && (pVrtx_2 == e.getpVrtx_2())); }


	private:
		Vertex* pVrtx_1;
		Vertex* pVrtx_2;
		int distance;
		EdgeStatus edgeStatus;
	};
};



#endif