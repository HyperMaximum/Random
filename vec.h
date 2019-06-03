/*
 * vec.h
 *
 *  Created on: April 8, 2019
 *      Author: jiamin
 */

#ifndef VEC_H_
#define VEC_H_

#include <cmath>
#include <iostream>

#define PI 3.1415926535897932234626

class Point
{
public:
	double x, y;	// x and y coordinates
	// Default constructor
	Point(): x(0.0), y(0.0) {	}
	// Explicit constructor
	Point(const double& dx, const double& dy): x(dx), y(dy) {	}
	// Method for addition and subtraction of points
	// e.g. p' + p
	Point operator + (const Point& p) const
	{
		return Point(this->x + p.x, this->y + p.y);
	}
	// e.g. p' - p
	Point operator - (const Point& p) const
	{
		return Point(this->x - p.x, this->y - p.y);
	}
	// Methods for scalar product of points
	// e.g. p * c
	Point operator * (const double& c) const
	{
		return Point(this->x * c, this->y * c);
	}
	// e.g. p / c
	Point operator / (const double& c) const
	{
		return Point(this->x / c, this->y / c);
	}
	// Method for equality of two points
	bool operator == (const Point& p) const
	{
		return (this->x == p.x) && (this->y == p.y);
	}
	// Method for inequality of two points
	bool operator != (const Point& p) const
	{
		return (this->x != p.x) || (this->y != p.y);
	}
};

// Scalar multiplication of points
// e.g. c * p
Point operator * (const double& c, const Point& p)
{
	return Point(p.x * c, p.y * c);
}

// Compare points by x-coordinates
bool comparePointX(Point& p1, Point& p2)
{
	if (p1.x != p2.x)
		return (p1.x < p2.x);
	else
		return (p1.y < p2.y);
}

// Compare points by y-coordinates
bool comparePointY(Point& p1, Point& p2)
{
	if (p1.y != p2.y)
		return (p1.y < p2.y);
	else
		return (p1.x < p2.x);
}

class Vector
{
public:
	double x, y;		// Two endpoints of the vector
	// Default constructor
	Vector(): x(0.0), y(0.0) {	}
	// Construct by double numbers
	Vector(const double& dx, const double& dy): x(dx), y(dy) {	}
	// Construct by Point objects
	Vector(const Point& start, const Point& end)
	{
		x = end.x - start.x;
		y = end.y - start.y;
	}
	// Construct by pointers to Point
	Vector(const Point*& start, const Point*& end)
	{
		x = end->x - start->x;
		y = end->y - start->y;
	}
	// Convert a point to a vector
	Vector(const Point& p): x(p.x), y(p.y) {	}
	// Convert a pointer to a point to a vector
	Vector(const Point*& p): x(p->x), y(p->y) {	}
	// Return to magnitude of the vector
	double magnitude()
	{
		return sqrt(x * x + y * y);
	}
	// Methods for sign of vector
	// e.g. +v
	Vector operator + () const
	{
		return *this;
	}
	// e.g. -v
	Vector operator - () const
	{
		return Vector(-this->x, -this->y);
	}
	// Method for addition and subtraction of vectors
	// e.g. u + v
	Vector operator + (const Vector& v) const
	{
		return Vector(this->x + v.x, this->y + v.y);
	}
	// e.g. u - v
	Vector operator - (const Vector& v) const
	{
		return Vector(this->x - v.x, this->y - v.y);
	}
	// Methods for scalar product of vectors
	// e.g. v * c
	Vector operator * (const double& c) const
	{
		return Vector(this->x * c, this->y * c);
	}
	// e.g. v / c
	Vector operator / (const double& c) const
	{
		return Vector(this->x / c, this->y / c);
	}
	// Methods for equality and inequality of vectors
	// e.g. v1 == v2
	bool operator == (const Vector& v) const
	{
		return (this->x == v.x) && (this->y == v.y);
	}
	// e.g. v1 != v2
	bool operator != (const Vector& v) const
	{
		return (this->x != v.x) || (this->y != v.y);
	}
	// Return the angle of this vector
	// Perhaps useless, just for fun
	double angle()
	{
		// Parallel the y-axis
		if (this->x == 0.0)
		{
			if (this->y >= 0.0)
				return 90.0;
			else
				return 270.0;
		}
		// Not parallel to the y-axis (tan is valid)
		double base = atan(this->y / this->x);
		if (this->x > 0.0)
		{
			if (base >= 0.0)
				return (base / PI) * 180.0;
			else
				return 360.0 + (base / PI) * 180.0;
		}
		else
			return 180.0 + (base / PI) * 180.0;
	}
};

// Scalar multiplication of points
// e.g. c * v
Vector operator * (const double& c, const Vector& v)
{
	return Vector(v.x * c, v.y * c);
}

// Compute the cross product of two vectors
double CrossProduct(Vector v1, Vector v2)
{
	return (v1.x * v2.y - v2.x * v1.y);
}

// Compute the dot product of two vectors
double DotProduct(Vector v1, Vector v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}

// Determine whether three points lie on the same line
bool IsCollinear(Point p1, Point p2, Point p3)
{
	Vector* v1 = new Vector(p1, p2);
	Vector* v2 = new Vector(p1, p3);
	return (CrossProduct(*v1, *v2) == 0.0);
}

// Find the relationship between a line and another point
// Return value:
// -1: left to the line
// 0: on the line
// 1: right to the line
// Method 1: compute the determinant:
// | 1  p1.x  p1.y |
// | 1  p2.x  p2.y |
// | 1  p3.x  p3.y |
// Method 2: Compute the cross product
int Relate(Point p1, Point p2, Point p3)
{
	Vector v1 = Vector(p1, p2);
	Vector v2 = Vector(p1, p3);
	double d = CrossProduct(v1, v2);
	if (d > 0)
		return 1;
	else if (d == 0)
		return 0;
	else
		return -1;
}

// Compute the intersection between two lines
// Return:
// 1 - Parallel and disjoint
// 2 - Identical
// 3 - Intersection existing
// Compute method:
// p = l * p1 + (1 - l) * p2
// p' = l' * p3 + (1 - l') * p4
// l = [p4 - p2, p4 - p3] / [p1 - p2, p4 - p3]
// l' = [p1 - p2, p4 - p2] / [p1 - p2, p4 - p3]
int Intersection(Point p1, Point p2, Point p3, Point p4, Point& p0)
{
	Vector line_1 = Vector(p1, p2);
	Vector line_2 = Vector(p3, p4);
	Vector v = Vector(p2, p4);
	if (CrossProduct(line_1, line_2) == 0)
	{
		if (!CrossProduct(line_1, v))
			return 1;
		else
			return 2;
	}
	else
	{
		double l = CrossProduct(v, line_2) / CrossProduct(-line_1, line_2);
		p0 = l * p1 + (1.0 - l) * p2;
		return 3;
	}
}

// Override << operator for Point class
std::ostream& operator << (std::ostream& out, const Point& p)
{
	out << "(" << p.x << ", " << p.y << ")";
	return out;
}

// Override << operator for Vector class
std::ostream& operator << (std::ostream& out, const Vector& v)
{
	out << "(" << v.x << ", " << v.y << ")";
	return out;
}

#endif /* VEC_H_ */
