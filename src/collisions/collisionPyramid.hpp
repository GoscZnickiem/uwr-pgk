#ifndef _GZN_PGK_COLLISIONS_COLLISIONPYRAMID_
#define _GZN_PGK_COLLISIONS_COLLISIONPYRAMID_

#include "../transform.hpp"
#include <glm/ext/vector_float3.hpp>
#include <optional>

class CollisionPyramid {
public:
	explicit CollisionPyramid(const Transform& t);

	~CollisionPyramid() = default;
	CollisionPyramid(const CollisionPyramid&) = delete;
	CollisionPyramid operator=(const CollisionPyramid&) = delete;
	CollisionPyramid(CollisionPyramid&&) = delete;
	CollisionPyramid operator=(CollisionPyramid&&) = delete;

	[[nodiscard]] std::optional<glm::vec3> collides(const glm::vec3& pos, float radius) const;

	void update();

	static constexpr glm::vec3 rawA{-1.f, -1.f, -1.f};
	static constexpr glm::vec3 rawB{ 0.f,  1.f, -1.f};
	static constexpr glm::vec3 rawC{ 1.f, -1.f, -1.f};
	static constexpr glm::vec3 rawD{ 0.f,  0.f,  1.f};

	glm::vec3 a{rawA};
	glm::vec3 b{rawB};
	glm::vec3 c{rawC};
	glm::vec3 d{rawD};

	const Transform* transform;
};

#endif // !_GZN_PGK_COLLISIONS_COLLISIONPYRAMID_
