#ifndef _GZN_PGK_CORE_MAINLOOP_
#define _GZN_PGK_CORE_MAINLOOP_

#include "../mainScene.hpp"

class MainLoop {
public:
	void update();
	void render();
	void run();

private:
	MainScene scene;
};

#endif
