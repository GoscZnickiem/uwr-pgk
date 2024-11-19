#ifndef _GZN_PGK_CORE_APPLICATION_
#define _GZN_PGK_CORE_APPLICATION_

class Application {
public:
	Application();
	~Application();

	void update();
	void render();
	void run();

	void atResize(int width, int height);

};

#endif
