#ifndef _GZN_PGK_INPUT_
#define _GZN_PGK_INPUT_

#include <map>
#include <string>

class Input {
public:
	static bool isKeyPressed(const std::string& key);

private:
	inline static std::map<std::string, bool> keys{};

	friend class Window;
};

#endif
