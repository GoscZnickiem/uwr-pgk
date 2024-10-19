#include "input.hpp"

bool Input::isKeyPressed(const std::string& key) {
	return keys[key];
}
