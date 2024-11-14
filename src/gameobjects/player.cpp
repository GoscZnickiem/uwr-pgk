#include "player.hpp"
#include "../appdata.hpp"
#include "../core/input.hpp"

#include <algorithm>
#include <cmath>
#include <queue>

// chat gpt moment

struct Point3D {
	glm::vec3 pos;
	std::size_t index;

    float distanceSquared(const Point3D &other) const {
        return (pos.x - other.pos.x) * (pos.x - other.pos.x) +
               (pos.y - other.pos.y) * (pos.y - other.pos.y) +
               (pos.z - other.pos.z) * (pos.z - other.pos.z);
    }
};

// Structure to represent a KD-Tree node
struct KDNode {
    Point3D point;
    KDNode *left;
    KDNode *right;

    KDNode(const Point3D &pt) : point(pt), left(nullptr), right(nullptr) {}
};

struct Neighbor {
    float distance;
	std::size_t id;
    Point3D point;

    Neighbor(float d, std::size_t i, const Point3D &p) : distance(d), id(i), point(p) {}

    bool operator<(const Neighbor &other) const {
        return distance < other.distance; // Max-heap based on distance
    }
};

struct KDTree {
public:
    KDTree(const std::vector<Point3D> &points) {
        root = build(points, 0);
    }

    ~KDTree() {
        clear(root);
    }

	// Find the k-nearest neighbors of a given point
	std::vector<std::size_t> kNearestNeighborIDs(const Point3D &target, std::size_t k) {
        std::priority_queue<Neighbor> nearestNeighbors;
        kNearestNeighbors(root, target, k, 0, nearestNeighbors);

        // Extract the IDs from the priority queue
        std::vector<std::size_t> result;
        while (!nearestNeighbors.empty()) {
            result.push_back(nearestNeighbors.top().id);
            nearestNeighbors.pop();
        }
        std::reverse(result.begin(), result.end()); // Closest-first order
        return result;
    }

private:
    KDNode *root;

    // Recursive function to build the KD-Tree
    KDNode *build(std::vector<Point3D> points, int depth) {
        if (points.empty()) return nullptr;

        // Determine the current dimension (0 = x, 1 = y, 2 = z)
        int axis = depth % 3;

        // Sort points by the current dimension
        auto compare = [axis](const Point3D &a, const Point3D &b) {
            if (axis == 0) return a.pos.x < b.pos.x;
            if (axis == 1) return a.pos.y < b.pos.y;
            return a.pos.z < b.pos.z;
        };
        std::sort(points.begin(), points.end(), compare);

        // Find the median point
		std::size_t medianIndex = points.size() / 2;
        Point3D medianPoint = points[medianIndex];

        // Create a new node with the median point
        KDNode *node = new KDNode(medianPoint);

        // Recursively build the left and right subtrees
        std::vector<Point3D> leftPoints(points.begin(), points.begin() + static_cast<long>(medianIndex));
        std::vector<Point3D> rightPoints(points.begin() + static_cast<long>(medianIndex) + 1, points.end());

        node->left = build(leftPoints, depth + 1);
        node->right = build(rightPoints, depth + 1);

        return node;
    }

    // Recursive function to free memory
    void clear(KDNode *node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

	void kNearestNeighbors(KDNode *node, const Point3D &target, std::size_t k, int depth,
                           std::priority_queue<Neighbor> &nearestNeighbors) {
        if (!node) return;

        // Calculate distance to the current node
        float distanceSquared = target.distanceSquared(node->point);
        if (nearestNeighbors.size() < k) {
            // If we don't have enough neighbors, add this one
            nearestNeighbors.push({distanceSquared, node->point.index, node->point});
        } else if (distanceSquared < nearestNeighbors.top().distance) {
            // Replace the farthest neighbor if the current one is closer
            nearestNeighbors.pop();
            nearestNeighbors.push({distanceSquared, node->point.index, node->point});
        }

        // Determine which side of the splitting plane to search
        int axis = depth % 3;
        float diff;
        if (axis == 0) diff = target.pos.x - node->point.pos.x;
        else if (axis == 1) diff = target.pos.y - node->point.pos.y;
        else diff = target.pos.z - node->point.pos.z;

        KDNode *nearBranch = (diff <= 0) ? node->left : node->right;
        KDNode *farBranch = (diff <= 0) ? node->right : node->left;

        // Search the near branch first
        kNearestNeighbors(nearBranch, target, k, depth + 1, nearestNeighbors);

        // If there might be closer points on the far branch, search it
        if (nearestNeighbors.size() < k || diff * diff < nearestNeighbors.top().distance) {
            kNearestNeighbors(farBranch, target, k, depth + 1, nearestNeighbors);
        }
    }
};



Player::Player(const std::vector<Obstacle>& obs)
:	transform{},
	m_vis(transform, AppData::data().modelPlayer, AppData::data().shaderSingle),
	obstacles(&obs) { 

	std::vector<Point3D> points;
	for(std::size_t i = 0; i < obs.size(); i++) points.emplace_back(obs[i].transform.position, i);

	tree = new KDTree(points);

}

Player::~Player() {
	delete tree;
}

void Player::update(float bound, const glm::vec3& boardCenter, const glm::vec3& direction, const glm::vec3& up) {
	const float radius = transform.scale.x;
	const float speed = 4.0f * AppData::deltaT * radius;

	if(Input::isKeyPressed("W")) {
		transform.position += speed * direction;
	}
	if(Input::isKeyPressed("S")) {
		transform.position -= speed * direction;
	}
	if(Input::isKeyPressed("A")) {
		transform.position -= speed * glm::normalize(glm::cross(direction, up));
	}
	if(Input::isKeyPressed("D")) {
		transform.position += speed * glm::normalize(glm::cross(direction, up));
	}
	if(Input::isKeyPressed("R")) {
		transform.position += speed * up;
	}
	if(Input::isKeyPressed("F")) {
		transform.position -= speed * up;
	}

	const float limit = bound - radius;
	if(transform.position.x <= -limit + boardCenter.x) transform.position.x = -limit + boardCenter.x;
	else if(transform.position.x >= limit + boardCenter.x) transform.position.x = limit + boardCenter.x;
	if(transform.position.y <= -limit + boardCenter.y) transform.position.y = -limit + boardCenter.y;
	else if(transform.position.y >= limit + boardCenter.y) transform.position.y = limit + boardCenter.y;
	if(transform.position.z <= -limit + boardCenter.z) transform.position.z = -limit + boardCenter.z;
	else if(transform.position.z >= limit + boardCenter.z) transform.position.z = limit + boardCenter.z;

	if(poweredUp > 0) {
		poweredUp -= AppData::deltaT;
		return;
	} else {
		poweredUp = 0;
	}

	Point3D p{transform.position, 0};

	auto obsIndexes = tree->kNearestNeighborIDs(p, 8);

	for(auto i : obsIndexes) {
		auto collision = (*obstacles)[i].collider.collides(transform.position, radius);
		if(collision) transform.position += *collision;
	}
}

void Player::render() {
	m_vis.render();
}

