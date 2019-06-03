/*
 * test1.cpp
 *
 *  Created on: May 20, 2019
 *      Author: Jiamin Pan
 */
#include <assert.h>
#include "dcel.h"

int main(void) {
	DCEL dcel;
	PVertex v1 = dcel.createVertex(1,0);
	PVertex v2 = dcel.createVertex(2,1);
	PVertex v3 = dcel.createVertex(0,1);
	PVertex v4 = dcel.createVertex(0,0);
	// v1 has one face (the unbounded face)
	assert(dcel.findFaces(v1).size() == 1);

	PEdge e1 = dcel.createEdge(dcel.getUnboundedFace(), v1, v2);
	// v1 still has one face which is not NULL
	assert(dcel.findFaces(v1).size() == 1);
	assert(dcel.findFaces(v1).front() != NULL);
	// v2 also has one unbounded face
	assert(dcel.findFaces(v2).size() == 1);
	// All vertices have one common face which is the unbounded face
	assert(dcel.findCommonFace(v1, v2) != NULL);
	assert(dcel.findCommonFace(v1, v3) != NULL);
	// Find the newly created edge using the findIncidentEdge function
	assert(dcel.findIncidentEdge(e1->origin, e1->face) == e1);
	dcel.createEdge(dcel.getUnboundedFace(), v2, v3);
	// v1 and v2 are still connected
	assert(dcel.isConnected(v1, v2));
	// v1 and v3 are not connected (i.e., not adjacent)
	assert(!dcel.isConnected(v1, v3));
	// Create two new edges to create the first face
	dcel.createEdge(dcel.getUnboundedFace(), v3, v4);
	dcel.splitFace(dcel.getUnboundedFace(), v4, v1);
	// Now there are two faces, the newly created face and the unbounded face
	assert(dcel.getFaceCount() == 2);
	// All the four vertices are adjacent to the two faces
	assert(dcel.findFaces(v1).size() == 2);
	assert(dcel.findFaces(v2).size() == 2);
	assert(dcel.findFaces(v3).size() == 2);
	assert(dcel.findFaces(v4).size() == 2);
	// v1 and v2 have two common faces, but the bounded face should be returned
	assert(dcel.findCommonFace(v1, v2) != dcel.getUnboundedFace());
	assert(dcel.findCommonFace(v1, v2)->getBoundary().size() == 4);
	assert(dcel.getUnboundedFace()->getAdjacentFaces().size() == 1);
	// Create a new edge that will result in a new face
	dcel.splitFace(dcel.findCommonFace(v1, v2), v4, v2);
	assert(dcel.getFaceCount() == 3);
	assert(dcel.findFaces(v4).size() == 3);
	// Both bounded faces are adjacent to the unbounded face
	assert(dcel.getUnboundedFace()->getAdjacentFaces().size() == 2);
	dcel.deleteEdge(v2, v4);
	assert(dcel.getFaceCount() == 2);
	return 0;
}


