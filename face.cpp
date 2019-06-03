/*
 * face.cpp
 *
 *  Created on: May 27, 2019
 *      Author: Jiamin Pan
 */
#include <vector>
#include "face.h"
#include "halfedge.h"

// Get a list of boundary
std::vector<HalfEdge*> Face::getBoundary() {
	HalfEdge* edge = this->edge;
	std::vector<HalfEdge*> boundary;
	do {
		boundary.push_back(edge);
		edge = edge->next;
	} while (edge != this->edge && edge != nullptr);
	return boundary;
}

// Get the number of boundary edges
int Face::getEdgecount() {
	return (int) this->getBoundary().size();
}

// Get all adjacent faces to current face
std::vector<Face*> Face::getAdjacentFaces() {
	HalfEdge* edge = this->edge;
	Face* f = nullptr;
	std::vector<Face*> faces;
	do {
		if (edge->twin->face != f) {
			faces.push_back(edge->twin->face);
			f = edge->twin->face;
		}
		edge = edge->next;
	} while (edge != this->edge && edge != nullptr);
	return faces;
}
