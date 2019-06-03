/*
 * dcel.h
 *
 *  Created on: May 20, 2019
 *      Author: Jiamin Pan
 */

#ifndef DCEL_H_
#define DCEL_H_

#include <vector>
#include "face.h"
#include "halfedge.h"
#include "vertex.h"

class DCEL {
public:
	std::vector<PVertex> vertices;
	std::vector<PEdge> edges;
	std::vector<PFace> faces;

	// Construction method
	DCEL();

	// Destruction method
	~DCEL();

	// Creates and returns a new vertex at the given point location
	PVertex createVertex(double, double);

	// Returns all faces that are adjacent to a vertex
	std::vector<PFace> findFaces(PVertex);

	// Finds a common face between two vertices
	PFace findCommonFace(PVertex, PVertex);

	// Returns true <==> The two given vertices have a common edge between them
	bool isConnected(PVertex, PVertex);

	// Return the incident edge from a vertex to a surface
	PEdge findIncidentEdge(PVertex, PFace);

	// Return the incident edge to a vertex with specific face
	PEdge findPrevEdge(PVertex, PFace);

	// Returns the single unbounded face in this DCEL
	PFace getUnboundedFace();

	// Connects the two vertices that belong to the same face
	// without splitting that face
	PEdge createEdge(PFace, PVertex, PVertex);

	// Creates a new half edge between the vertices
	// which splits the face f into two new faces
	PEdge splitFace(PFace, PVertex, PVertex);

	// Return the number of faces
	int getFaceCount();

	// Deletes an existing edge between two vertices
	bool deleteEdge(PVertex, PVertex);
};

#endif /* DCEL_H_ */
