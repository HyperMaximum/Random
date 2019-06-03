/*
 * test2.cpp
 *
 *  Created on: May 28, 2019
 *      Author: Jiamin Pan
 */
#include <assert.h>
#include <cmath>
#include <iostream>
#include <utility>
#include "dcel.h"
#include "vec.h"

Point computeVoronoiVertex(PEdge e1, PEdge e2) {
	PVertex start1 = e1->origin, start2 = e2->origin;
	PVertex end1 = e1->destination(), end2 = e2->destination();
	double dx1 = end1->x - start1->x, dy1 = end1->y - start1->y;
	double dx2 = end2->x - start2->x, dy2 = end2->y - start2->y;
	double dr1 = dx1 * (end1->x + start1->x) + dy1 * (end1->y + start1->y);
	double dr2 = dx2 * (end2->x + start2->x) + dy2 * (end2->y + start2->y);
	double D = 4 * (dx1 * dy2 - dx2 * dy1);
	double x = 2 * (dr1 * dy2 - dr2 * dy1) / D;
	double y = 2 * (dx1 * dr2 - dx2 * dr1) / D;
	return Point(x, y);
}

Point bisectorBoundary(PVertex p1, PVertex p2, std::pair<Point, Point> rectangle) {
	double x_min = std::min(rectangle.first.x, rectangle.second.x);
	double x_max = std::max(rectangle.first.x, rectangle.second.x);
	double y_min = std::min(rectangle.first.y, rectangle.second.y);
	double y_max = std::max(rectangle.first.y, rectangle.second.y);
	double dx = p2->x - p1->x, dy = p2->y - p1->y;
	double dis = (p2->x * p2->x + p2->y * p2->y) - (p1->x * p1->x + p1->y * p1->y);
	Vector d = Vector(dy, -dx);
	// Vertical and horizontal cases
	if (d.x == 0.0) {
		if (d.y >= 0.0) {
			return Point((p1->x + p2->x) / 2.0, y_max);
		} else {
			return Point((p1->x + p2->x) / 2.0, y_min);
		}
	}
	if (d.y == 0.0) {
		if (d.x >= 0.0) {
			return Point(x_max, (p1->y + p2->y) / 2.0);
		} else {
			return Point(x_min, (p1->y + p2->y) / 2.0);
		}
	}
	// General cases
	if (d.x > 0 && d.y > 0) {
		Vector v = Vector(x_max - x_min, y_max - y_min);
		if (CrossProduct(v, d) >= 0) {
			double x = (dis - 2 * dy * y_max) / (2 * dx);
			return Point(x, y_max);
		} else {
			double y = (dis - 2 * dx * x_max) / (2 * dy);
			return Point(x_max, y);
		}
	} else if (d.x < 0 && d.y > 0) {
		Vector v = Vector(x_min - x_max, y_max - y_min);
		if (CrossProduct(v, d) >= 0) {
			double y = (dis - 2 * dx * x_min) / (2 * dy);
			return Point(x_min, y);
		} else {
			double x = (dis - 2 * dy * y_max) / (2 * dx);
			return Point(x, y_max);
		}
	} else if (d.x < 0 && d.y < 0) {
		Vector v = Vector(x_min - x_max, y_min - y_max);
		if (CrossProduct(v, d) >= 0) {
			double x = (dis - 2 * dy * y_min) / (2 * dx);
			return Point(x, y_min);
		} else {
			double y = (dis - 2 * dx * x_min) / (2 * dy);
			return Point(x_min, y);
		}
	} else if (d.x > 0 && d.y < 0) {
		Vector v = Vector(x_max - x_min, y_max - y_min);
		if (CrossProduct(v, d) >= 0) {
			double y = (dis - 2 * dx * x_max) / (2 * dy);
			return Point(x_max, y);
		} else {
			double x = (dis - 2 * dy * y_min) / (2 * dx);
			return Point(x, y_min);
		}
	}
	return Point();
}

std::vector<Point> computeVoronoiRegion(DCEL d, PVertex p, std::pair<Point, Point> rectangle) {
	PEdge edge = p->leaving, next;
	std::vector<Point> vertices;
	do {
		next = p->nextLeaving(edge);
		if (edge->twin->face == d.faces[0]) {
			vertices.push_back(bisectorBoundary(p, p->leaving->destination(), rectangle));
		} else {
			vertices.push_back(computeVoronoiVertex(edge, next));
			if (edge->face == d.faces[0]) {
				vertices.push_back(bisectorBoundary(edge->destination(), p, rectangle));
			}
		}
		edge = next;
	} while (edge != p->leaving);
	return vertices;
}

void printps(std::vector<Point> ps)
{
	for (auto p = ps.begin(); p != ps.end(); ++p)
	{
		std::cout << *p;
		if (p != ps.end() - 1)
			std::cout << ", ";
		else
			std::cout << std::endl;
	}
}

int main(void) {
	DCEL d = DCEL();
	PVertex p1 = d.createVertex(1, 1);
	PVertex p2 = d.createVertex(5, 1);
	PVertex p3 = d.createVertex(3, 4);
	PVertex p4 = d.createVertex(3, 2);

	PEdge e1 = d.createEdge(d.faces[0], p1, p2); PEdge e2 = e1->twin;
	PEdge e3 = d.createEdge(d.faces[0], p2, p3); PEdge e4 = e3->twin;
	PEdge e5 = d.splitFace(d.faces[0], p3, p1); PEdge e6 = e5->twin;
	PEdge e7 = d.createEdge(d.faces[1], p4, p2); PEdge e8 = e7->twin;
	PEdge e9 = d.splitFace(d.faces[1], p3, p4); PEdge e10 = e9->twin;
	PEdge e11 = d.splitFace(d.faces[1], p1, p4); PEdge e12 = e11->twin;

	PFace f0 = d.faces[0]; PFace f1 = d.faces[1];
	PFace f2 = d.faces[2]; PFace f3 = d.faces[3];

	/*
	PFace f0 = d.faces[0];
	PFace f1 = new Face(); PFace f2 = new Face(); PFace f3 = new Face();
	d.faces.push_back(f1); d.faces.push_back(f2); d.faces.push_back(f3);

	PVertex p1 = d.createVertex(1, 1);
	PVertex p2 = d.createVertex(5, 1);
	PVertex p3 = d.createVertex(3, 4);
	PVertex p4 = d.createVertex(3, 2);

	PEdge e1 = new HalfEdge(p1, f1); PEdge e2 = new HalfEdge(p2, f0);
	PEdge e3 = new HalfEdge(p2, f2); PEdge e4 = new HalfEdge(p3, f0);
	PEdge e5 = new HalfEdge(p3, f3); PEdge e6 = new HalfEdge(p1, f0);
	PEdge e7 = new HalfEdge(p4, f2); PEdge e8 = new HalfEdge(p2, f1);
	PEdge e9 = new HalfEdge(p3, f2); PEdge e10 = new HalfEdge(p4, f3);
	PEdge e11 = new HalfEdge(p1, f3); PEdge e12 = new HalfEdge(p4, f1);

	f0->edge = e2; f1->edge = e1; f2->edge = e3; f3->edge = e5;
	p1->leaving = e1; p2->leaving = e3; p3->leaving = e5; p4->leaving = e7;

	e1->twin = e2; e1->next = e8; d.edges.push_back(e1);
	e2->twin = e1; e2->next = e6; d.edges.push_back(e2);
	e3->twin = e4; e3->next = e9; d.edges.push_back(e3);
	e4->twin = e3; e4->next = e2; d.edges.push_back(e4);
	e5->twin = e6; e5->next = e11; d.edges.push_back(e5);
	e6->twin = e5; e6->next = e4; d.edges.push_back(e6);
	e7->twin = e8; e7->next = e3; d.edges.push_back(e7);
	e8->twin = e7; e8->next = e12; d.edges.push_back(e8);
	e9->twin = e10; e9->next = e7; d.edges.push_back(e9);
	e10->twin = e9; e10->next = e5; d.edges.push_back(e10);
	e11->twin = e12; e11->next = e10; d.edges.push_back(e11);
	e12->twin = e11; e12->next = e1; d.edges.push_back(e12);
	*/


	assert(e1->destination() == p2);
	assert(e9->destination() == p4);
	assert(e7->origin->nextLeaving(e7) == e12);
	assert(e7->origin->nextLeaving(e7->origin->nextLeaving(e7)) == e10);
	assert(e7->origin->nextLeaving(e7->origin->nextLeaving(e7->origin->nextLeaving(e7))) == e7);
	assert(d.findFaces(p4).size() == 3);
	assert(d.findFaces(p1).size() == 3);
	assert(d.isConnected(p1,p4) == true);
	assert(d.findIncidentEdge(p1,f1) == e1);
	assert(d.findIncidentEdge(p1,f2) == nullptr);
	assert(d.findIncidentEdge(p1,f0) == e6);

	std::cout << "v1: " << computeVoronoiVertex(e7, e11) << std::endl;
	std::cout << "v2: " << computeVoronoiVertex(e7, e9) << std::endl;
	std::cout << "v3: " << computeVoronoiVertex(e9, e11) << std::endl;

	std::pair<Point, Point> rectangle(Point(0, -1), Point(7, 5));
	std::cout << "v4: " << bisectorBoundary(p3, p1, rectangle) << std::endl;
	std::cout << "v5: " << bisectorBoundary(p1, p2, rectangle) << std::endl;
	std::cout << "v6: " << bisectorBoundary(p2, p3, rectangle) << std::endl;

	std::cout << "The voronoi vertices of p2 are: ";
	printps(computeVoronoiRegion(d, p2, rectangle));

	return 0;
}
