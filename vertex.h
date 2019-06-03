/*
 * vertex.h
 *
 *  Created on: May 20, 2019
 *      Author: Jiamin Pan
 */

#ifndef VERTEX_H_
#define VERTEX_H_

class HalfEdge;

class Vertex {
public:
	HalfEdge* leaving;
	double x, y;

	// Construction Methods
	Vertex(): leaving(nullptr), x(0.0), y(0.0) { };

	Vertex(double _x, double _y):
		leaving(nullptr), x(_x), y(_y) { };

	Vertex(HalfEdge* _l, double _x, double _y):
		leaving(_l), x(_x), y(_y) { };

	// Returns the next edge leaving the vertex
	HalfEdge* nextLeaving(HalfEdge*);

	// Returns the half edge from current vertex to another one
	HalfEdge* getConnectEdge(Vertex*);

	// Returns the number of edges out of the vertex
	int outDegree();
};

typedef Vertex* PVertex;

#endif /* VERTEX_H_ */
