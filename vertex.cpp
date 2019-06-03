/*
 * vertex.cpp
 *
 *  Created on: May 21, 2019
 *      Author: Jiamin Pan
 */
#include "halfedge.h"
#include "vertex.h"

// Returns the next edge leaving the vertex
HalfEdge* Vertex::nextLeaving(HalfEdge* pe) {
	return (pe->twin != nullptr)? pe->twin->next : nullptr;
}

// Returns the half edge from current vertex to another one
HalfEdge* Vertex::getConnectEdge(Vertex* pv) {
	if (this->leaving != nullptr) {
		HalfEdge* he = this->leaving;
		do {
			if (he->destination() == pv) {
				return he;
			} else {
				he = this->nextLeaving(he);
			}
		} while (he != this->leaving && he != nullptr);
	}
	return nullptr;
}

// Returns the number of edges out of the vertex
int Vertex::outDegree() {
	int count = 0;
	if (this->leaving != nullptr) {
		HalfEdge* edge = this->leaving;
		do {
			++count;
			edge = this->nextLeaving(edge);
		} while (edge != this->leaving && edge != nullptr);
	}
	return count;
}
