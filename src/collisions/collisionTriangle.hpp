#ifndef _GZN_PGK_COLLISIONS_COLLISIONTRIANGLE_
#define _GZN_PGK_COLLISIONS_COLLISIONTRIANGLE_

#include "../transform.hpp"

struct Point {
	float x, y;
};

class CollisionTriangle {
public:
	explicit CollisionTriangle(const Transform& t);
	[[nodiscard]] bool isNear(const CollisionTriangle& other) const;
	[[nodiscard]] bool collides(const CollisionTriangle& other) const;

	void update();

private:
	const Transform* transform;


	Point rawA{-1.f, -1.f};
	Point rawB{0.f, 1.f};
	Point rawC{1.f, -1.f};

	Point a{rawA};
	Point b{rawB};
	Point c{rawC};

	[[nodiscard]] bool isPointInTriangle(const Point& p) const;
	[[nodiscard]] Point updatePoint(Point p);
};

#endif // !_GZN_PGK_COLLISIONS_COLLISIONTRIANGLE_
