/*
 * halfedge.cpp
 *
 *  Created on: May 21, 2019
 *      Author: Jiamin Pan
 */
#include "halfedge.h"
#include "vertex.h"

// Return the destination of this HalfEdge
Vertex* HalfEdge::destination() {
	return (this->twin != nullptr)? this->twin->origin : nullptr;
}

// Return the edge whose next is current edge
HalfEdge* HalfEdge::getPrevious() {
	HalfEdge* edge = this;
	do {
		if (edge->twin->next == this) {
			return edge;
		}
		edge = this->origin->nextLeaving(edge);
	} while (edge != this && edge != nullptr);
	return nullptr;
}
