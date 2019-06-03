/*
 * face.h
 *
 *  Created on: May 20, 2019
 *      Author: Jiamin Pan
 */

#ifndef FACE_H_
#define FACE_H_

#include <vector>

class HalfEdge;

class Face {
public:
	HalfEdge* edge;

	// Construction Methods
	Face(): edge(nullptr) { }

	Face(HalfEdge* _e): edge(_e) { }

	// Get a list of boundary
	std::vector<HalfEdge*> getBoundary();

	// Get the number of boundary edges
	int getEdgecount();

	// Get all adjacent faces to current face
	std::vector<Face*> getAdjacentFaces();
};

typedef Face* PFace;

#endif /* FACE_H_ */
