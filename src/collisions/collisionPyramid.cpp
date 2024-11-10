#include "collisionPyramid.hpp"

#include <cmath>
#include <optional>

CollisionPyramid::CollisionPyramid(const Transform& t) : transform(&t) {
	update();
}

glm::vec3 closestPointTriangle(glm::vec3 const& p, glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& c) {
   const glm::vec3 ab = b - a;
   const glm::vec3 ac = c - a;
   const glm::vec3 ap = p - a;

   const float d1 = dot(ab, ap);
   const float d2 = dot(ac, ap);
   if (d1 <= 0.f && d2 <= 0.f) return a; //#1

   const glm::vec3 bp = p - b;
   const float d3 = dot(ab, bp);
   const float d4 = dot(ac, bp);
   if (d3 >= 0.f && d4 <= d3) return b; //#2

   const glm::vec3 cp = p - c;
   const float d5 = dot(ab, cp);
   const float d6 = dot(ac, cp);
   if (d6 >= 0.f && d5 <= d6) return c; //#3

   const float vc = d1 * d4 - d3 * d2;
   if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f) {
      const float v = d1 / (d1 - d3);
      return a + v * ab; //#4
   }

   const float vb = d5 * d2 - d1 * d6;
   if (vb <= 0.f && d2 >= 0.f && d6 <= 0.f) {
      const float v = d2 / (d2 - d6);
      return a + v * ac; //#5
   }

   const float va = d3 * d6 - d5 * d4;
   if (va <= 0.f && (d4 - d3) >= 0.f && (d5 - d6) >= 0.f) {
      const float v = (d4 - d3) / ((d4 - d3) + (d5 - d6));
      return b + v * (c - b); //#6
   }

   const float denom = 1.f / (va + vb + vc);
   const float v = vb * denom;
   const float w = vc * denom;
   return a + v * ab + w * ac; //#0
}

std::optional<glm::vec3> CollisionPyramid::collides(const glm::vec3& pos, float radius) const {
	glm::vec3 closestABC = pos - closestPointTriangle(pos, a, b, c);
	glm::vec3 closestACD = pos - closestPointTriangle(pos, a, c, d);
	glm::vec3 closestABD = pos - closestPointTriangle(pos, a, b, d);
	glm::vec3 closestBCD = pos - closestPointTriangle(pos, b, c, d);

	glm::vec3* closest = &closestABC;

	if(dot(*closest, *closest) > dot(closestACD, closestACD)) closest = &closestACD;
	if(dot(*closest, *closest) > dot(closestABD, closestABD)) closest = &closestABD;
	if(dot(*closest, *closest) > dot(closestBCD, closestBCD)) closest = &closestBCD;

	if(dot(*closest, *closest) < radius * radius) return glm::normalize(*closest) * radius - *closest; 
	return std::nullopt;
}

void CollisionPyramid::update() {
	auto mat = transform->getMatrix();
	a = mat * glm::vec4(rawA, 1.f);
	b = mat * glm::vec4(rawB, 1.f);
	c = mat * glm::vec4(rawC, 1.f);
	d = mat * glm::vec4(rawD, 1.f);
}
