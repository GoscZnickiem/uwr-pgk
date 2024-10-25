#include "collisionTriangle.hpp"

#include <algorithm>
#include <cmath>

CollisionTriangle::CollisionTriangle(const Transform& t) : transform(&t) { }

bool CollisionTriangle::isNear(const CollisionTriangle& other) const {
	float x = transform->x - other.transform->x;
	float y = transform->y - other.transform->y;
	float biggerSide = transform->yScale + other.transform->yScale;
	float smallerSide = transform->xScale + other.transform->xScale;
	return x * x + y * y <= smallerSide * smallerSide + biggerSide * biggerSide;
}

float crossProduct(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool onEdge(const Point& a, const Point& b, const Point& p) {
    return std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
           std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

bool doEdgesIntersect(const Point& A1, const Point& A2, const Point& B1, const Point& B2) {
    float d1 = crossProduct(B1, B2, A1);
    float d2 = crossProduct(B1, B2, A2);
    float d3 = crossProduct(A1, A2, B1);
    float d4 = crossProduct(A1, A2, B2);

    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
        return true;

    if (d1 == 0 && onEdge(B1, B2, A1)) return true;
    if (d2 == 0 && onEdge(B1, B2, A2)) return true;
    if (d3 == 0 && onEdge(A1, A2, B1)) return true;
    if (d4 == 0 && onEdge(A1, A2, B2)) return true;

    return false;
}

bool CollisionTriangle::isPointInTriangle(const Point& p) const {
    float d1 = crossProduct(p, a, b);
    float d2 = crossProduct(p, b, c);
    float d3 = crossProduct(p, c, a);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

bool CollisionTriangle::collides(const CollisionTriangle& other) const {
	if (doEdgesIntersect(a, b, other.a, other.b)) return true;
	if (doEdgesIntersect(a, c, other.a, other.b)) return true;
	if (doEdgesIntersect(b, c, other.a, other.b)) return true;

	if (doEdgesIntersect(a, b, other.a, other.c)) return true;
	if (doEdgesIntersect(a, c, other.a, other.c)) return true;
	if (doEdgesIntersect(b, c, other.a, other.c)) return true;

	if (doEdgesIntersect(a, b, other.b, other.c)) return true;
	if (doEdgesIntersect(a, c, other.b, other.c)) return true;
	if (doEdgesIntersect(b, c, other.b, other.c)) return true;

	if (other.isPointInTriangle(a) || isPointInTriangle(other.a))
		return true;

	return false;
}

Point CollisionTriangle::updatePoint(Point p) {
	float sin = std::sinf(transform->angle);
	float cos = std::cosf(transform->angle);
	float x = p.x * transform->xScale;
	float y = p.y * transform->yScale;
	return {x * cos - y * sin + transform->x, y * cos + x * sin + transform->y};
}

void CollisionTriangle::update() {
	a = updatePoint(rawA);
	b = updatePoint(rawB);
	c = updatePoint(rawC);
}
