#include "obstacle.hpp"

Obstacle::Obstacle() : collider(transform) {}

Obstacle::Obstacle(Obstacle&& other) : transform(std::move(other.transform)), collider(transform) { }
