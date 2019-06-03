/*
 * halfedge.h
 *
 *  Created on: May 20, 2019
 *      Author: Jiamin Pan
 */

#ifndef HALFEDGE_H_
#define HALFEDGE_H_

#include "face.h"
#include "vertex.h"

class HalfEdge;

class HalfEdge {
public:
	Vertex* origin;
	Face* face;
	HalfEdge* twin;
	HalfEdge* next;

	// Construction Methods
	HalfEdge(): origin(nullptr), face(nullptr),
			twin(nullptr), next(nullptr) { };

	HalfEdge(Vertex* _v, Face* _f): origin(_v), face(_f),
			twin(nullptr), next(nullptr) { };

	// Return the destination of this HalfEdge
	Vertex* destination();

	// Return the edge whose next is current edge
	HalfEdge* getPrevious();
};

typedef HalfEdge* PEdge;

#endif /* HALFEDGE_H_ */
