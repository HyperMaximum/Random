/*
 * dcel.cpp
 *
 *  Created on: May 20, 2019
 *      Author: Jiamin Pan
 */
#include <algorithm>
#include <vector>
#include "dcel.h"
#include "face.h"
#include "halfedge.h"
#include "vertex.h"

// Construction Method
DCEL::DCEL() {
	// Initialize three lists
	this->vertices = std::vector<PVertex>();
	this->edges = std::vector<PEdge>();
	this->faces = std::vector<PFace>();
	// Create an unbounded face and add it to the list
	this->faces.push_back(new Face());
}

// Destruction Method
DCEL::~DCEL() {
	// Free the spaces pointed by these pointers
	for (PVertex pv: this->vertices) {
		delete pv;
	}
	for (PEdge pe: this->edges) {
		delete pe;
	}
	for (PFace pf: this->faces) {
		delete pf;
	}
	// Clear all the vertices
	this->vertices.clear();
	this->edges.clear();
	this->faces.clear();
}

// Creates and returns a new vertex at the given point location
PVertex DCEL::createVertex(double x, double y) {
	PVertex pv = new Vertex(x, y);
	this->vertices.push_back(pv);
	return pv;
}

// Returns all faces that are adjacent to a vertex
std::vector<PFace> DCEL::findFaces(PVertex pv) {
	std::vector<PFace> pfs;
	if (pv->leaving != nullptr) {
		PEdge edge = pv->leaving;
		do {
			pfs.push_back(edge->face);
			edge = pv->nextLeaving(edge);
		} while (edge != pv->leaving && edge != nullptr);
	} else {
		pfs.push_back(getUnboundedFace());
	}
	return pfs;
}

// Finds a common face between two vertices
PFace DCEL::findCommonFace(PVertex pv1, PVertex pv2) {
	// Find all faces adjacent to these two vertices
	std::vector<PFace> pfs1 = findFaces(pv1);
	std::vector<PFace> pfs2 = findFaces(pv2);
	std::vector<PFace> common = std::vector<PFace>();
	// Search for common
	for (PFace pf: pfs1) {
		if (std::find(pfs2.begin(), pfs2.end(), pf) != pfs2.end()) {
			common.push_back(pf);
		}
	}
	// Remove unbounded faces
	std::vector<PFace>::iterator ub = std::find(common.begin(), common.end(), getUnboundedFace());
	if (ub != common.end()) {
		common.erase(ub);
	}
	return (common.size() != 0)? common[0] : getUnboundedFace();
}

// Returns true <==> The two given vertices have a common edge between them
bool DCEL::isConnected(PVertex pv1, PVertex pv2) {
	return (pv1->getConnectEdge(pv2) != nullptr)? true : false;
}

// Return the incident edge from a vertex to a surface
PEdge DCEL::findIncidentEdge(PVertex pv, PFace pf) {
	if (pv->leaving != nullptr) {
		PEdge edge = pv->leaving;
		do {
			if (edge->face == pf) {
				return edge;
			} else {
				edge = pv->nextLeaving(edge);
			}
		} while (edge != pv->leaving && edge != nullptr);
	}
	return nullptr;
}

// Return the incident edge to a vertex with specific face
PEdge DCEL::findPrevEdge(PVertex pv, PFace pf) {
	if (pv->leaving != nullptr) {
		PEdge edge = pv->leaving;
		do {
			if (edge->twin->face == pf) {
				return edge->twin;
			} else {
				edge = pv->nextLeaving(edge);
			}
		} while (edge != pv->leaving && edge != nullptr);
	}
	return nullptr;
}

// Returns the single unbounded face in this DCEL
PFace DCEL::getUnboundedFace() {
	return this->faces[0];
}

// Connects the two vertices that belong to the same face
// without splitting that face
PEdge DCEL::createEdge(PFace pf, PVertex pv1, PVertex pv2) {
	// Search incident edges to two vertices with specific face
	PEdge prevLeft = findPrevEdge(pv1, pf);
	PEdge prevRight = findPrevEdge(pv2, pf);
	PEdge left = new HalfEdge(pv1, pf);
	PEdge right = new HalfEdge(pv2, pf);
	// Set twin pointers
	left->twin = right; right->twin = left;
	if (pv1->leaving == nullptr || pv1->outDegree() == 1) {
		pv1->leaving = left;
	}
	if (pv2->leaving == nullptr) {
		pv2->leaving = right;
	}
	// Set next pointers
	if (prevRight != nullptr) {
		left->next = (prevRight->next != nullptr)? prevRight->next : prevRight->twin;
		prevRight->next = right;
	}
	if (prevLeft != nullptr) {
		right->next = (prevLeft->next != nullptr)? prevLeft->next : prevLeft->twin;
		prevLeft->next = left;
	}
	// Add edges to the list
	this->edges.push_back(left); this->edges.push_back(right);
	if (pf->edge == nullptr) {
		pf->edge = right;
	}
	return left;
}

// Creates a new half edge between the vertices
// which splits the face into two new faces
PEdge DCEL::splitFace(PFace pf, PVertex pv1, PVertex pv2) {
	PEdge left = createEdge(pf, pv1, pv2);
	PFace newface = new Face(left);
	PEdge edge = left;
	do {
		edge->face = newface;
		edge = edge->next;
	} while (edge != left && edge != nullptr);
	this->faces.push_back(newface);
	return left;
}

// Return the number of faces
int DCEL::getFaceCount() {
	return this->faces.size();
}

// Deletes an existing edge between two vertices
bool DCEL::deleteEdge(PVertex v1, PVertex v2) {
	PEdge edge = v1->getConnectEdge(v2);
	if (edge == nullptr) {
		return false;
	} else {
		PFace face = edge->face, a_face = edge->twin->face;
		PEdge prevEdge = edge->getPrevious(), prevTwin = edge->twin->getPrevious();
		PEdge nextEdge = edge->next, nextTwin = edge->twin->next;
		// Merge two faces into one
		prevEdge->next = nextTwin; prevTwin->next = nextEdge;
		a_face->edge = nextEdge;
		PEdge e = nextEdge;
		do {
			e->face = a_face;
			e = e->next;
		} while (e != nextEdge && e != nullptr);
		// Remove the face and the edges
		faces.erase(std::find(faces.begin(), faces.end(), face));
		delete face;
		edges.erase(std::find(edges.begin(), edges.end(), edge));
		edges.erase(std::find(edges.begin(), edges.end(), edge->twin));
		delete edge->twin; delete edge;
		return true;
	}
}

// Combines two DCEL structures together into a new DCEL
DCEL mergeAndDestroy(DCEL& d1, DCEL& d2) {
	DCEL dcel = DCEL();
	dcel.edges.insert(dcel.edges.begin(), d1.edges.begin(), d1.edges.end());
	dcel.edges.insert(dcel.edges.begin(), d2.edges.begin(), d2.edges.end());
	dcel.vertices.insert(dcel.vertices.begin(), d1.vertices.begin(), d1.vertices.end());
	dcel.vertices.insert(dcel.vertices.begin(), d2.vertices.begin(), d2.vertices.end());
	dcel.faces.insert(dcel.faces.begin(), d1.faces.begin() + 1, d1.faces.end());
	dcel.faces.insert(dcel.faces.begin(), d2.faces.begin() + 1, d2.faces.end());
	d1.~DCEL(); d2.~DCEL();
	return DCEL();
}
