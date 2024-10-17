// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include <cmath>
#include <stdlib.h>

#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"

// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================
class MyTri : public AGLDrawable {
public:
	MyTri() : AGLDrawable(0) {
		setShaders();
	}
	void setShaders() {
		compileShaders(R"END(

				 #version 330 
				 out vec4 vcolor;
				 out vec2 vpos;

				 void main(void) {
					 const vec2 vertices[6] = vec2[6](
						 vec2(-0.9, -0.9),
						 vec2( 0.9,  0.9),
						 vec2( 0.9, -0.9),
						 vec2(-0.9, -0.9),
						 vec2( 0.9,  0.9),
						 vec2(-0.9,  0.9)
					 );
					 const vec4 colors[]    = vec4[6](
						 vec4(1.0, 0.0, 0.0, 1.0),
						 vec4(0.0, 1.0, 0.0, 1.0),
						 vec4(1.0, 1.0, 0.0, 1.0),
						 vec4(1.0, 0.0, 0.0, 1.0),
						 vec4(0.0, 1.0, 0.0, 1.0),
						 vec4(0.0, 0.0, 1.0, 1.0)
					 );

					 vcolor      = colors[gl_VertexID];
					 vpos        = vertices[gl_VertexID];
					 gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
				 }

				 )END", R"END(

				 #version 330 

				 in  vec4 vcolor;
				 in  vec2 vpos;
				 out vec4 color;

				 void main(void) {
					 color = vcolor;
					 if(length(vpos) < 0.1) {
						 color = vec4(1.0, 1.0, 1.0, 1.0);
					 }
				 } 

				 )END");
	}
	void draw() {
		bindProgram();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};


// ==========================================================================
// Drawable object with some data in buffer and vertex/fragment programs
// ==========================================================================
class MyCross : public AGLDrawable {
public:
	MyCross() : AGLDrawable(0) {
		setShaders();
		setBuffers();
	}
	void setShaders() {
		compileShaders(R"END(

				 #version 330 
				 #extension GL_ARB_explicit_uniform_location : require
				 #extension GL_ARB_shading_language_420pack : require
				 layout(location = 0) in vec2 pos;
				 layout(location = 0) uniform float scale;
				 layout(location = 1) uniform vec2  center;
				 out vec4 vtex;

				 void main(void) {
				 vec2 p = (pos * scale + center);
				 gl_Position = vec4(p, 0.0, 1.0);
				 }

				 )END", R"END(

				 #version 330 
				 #extension GL_ARB_explicit_uniform_location : require
				 layout(location = 3) uniform vec3  cross_color;
				 out vec4 color;

				 void main(void) {
				 color = vec4(cross_color,1.0);
				 } 

				 )END");
	}
	void setBuffers() {
		bindBuffers();
		GLfloat vert[4][2] = {  // Cross lines
			{ -1,  0  },
			{  1,  0  },
			{  0, -1  },
			{  0,  1  }
		};

		glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), vert, GL_STATIC_DRAW );
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                 // attribute 0, must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,//24,             // stride
			(void*)0            // array buffer offset
		);
	}
	void draw(float tx, float ty) {
		bindProgram();
		bindBuffers();
		glUniform1f(0, 1.0/16);  // scale  in vertex shader
		glUniform2f(1, tx, ty);  // center in vertex shader
		glUniform3f(3, cross_color[0],cross_color[1],cross_color[2]);

		glDrawArrays(GL_LINES, 0, 4);
	}
	void setColor(float r, float g, float b){
		cross_color[0]=r;cross_color[1]=g;cross_color[2]=b;
	}
private:
	GLfloat cross_color[3] = { 0.0, 1.0, 0.0 };
};

class MyCircle : public AGLDrawable {
public:
	MyCircle(int _points) : AGLDrawable(0) {
		points = _points;
		setShaders();
		setBuffers();
	}
	void setShaders() {
		compileShaders(R"END(

				 #version 330 
				 #extension GL_ARB_explicit_uniform_location : require
				 #extension GL_ARB_shading_language_420pack : require
				 layout(location = 0) in vec2 pos;
				 layout(location = 0) uniform float scale;
				 layout(location = 1) uniform vec2  center;
				 out vec4 vtex;

				 void main(void) {
				 vec2 p = (pos * scale + center);
				 gl_Position = vec4(p, 0.0, 1.0);
				 }

				 )END", R"END(

				 #version 330 
				 #extension GL_ARB_explicit_uniform_location : require
				 layout(location = 3) uniform vec3  cross_color;
				 out vec4 color;

				 void main(void) {
				 color = vec4(cross_color,1.0);
				 } 

				 )END");
	}
	void setBuffers() {
		bindBuffers();
		GLfloat vert[points][2];

		float angle = 0.f;
		float step = 2 * 3.141592653f / points;
		for(int i = 0; i < points; i++) {
			vert[i][0] = std::cosf(angle);
			vert[i][1] = std::sinf(angle);
			angle += step;
		}

		glBufferData(GL_ARRAY_BUFFER, points*2*sizeof(float), vert, GL_STATIC_DRAW );
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                 // attribute 0, must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,//24,             // stride
			(void*)0            // array buffer offset
		);
	}
	void draw(float tx, float ty) {
		bindProgram();
		bindBuffers();
		glUniform1f(0, 1.0/16);  // scale  in vertex shader
		glUniform2f(1, tx, ty);  // center in vertex shader
		glUniform3f(3, cross_color[0],cross_color[1],cross_color[2]);

		glDrawArrays(GL_LINE_LOOP, 0, points);
	}
	void setColor(float r, float g, float b){
		cross_color[0]=r;cross_color[1]=g;cross_color[2]=b;
	}
private:
	GLfloat cross_color[3] = { 1.0, 0.7, 0.8 };
	int points;
};


// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow {
public:
	MyWin() {};
	MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
	: AGLWindow(_wd, _ht, name, vers, fullscr) {};
	virtual void KeyCB(int key, int scancode, int action, int mods);
	void MainLoop();
};


// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods) {
	AGLWindow::KeyCB(key,scancode, action, mods); // f-key full screen switch
	if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) {
		; // do something
	}
	if (key == GLFW_KEY_HOME  && (action == GLFW_PRESS)) {
		; // do something
	}
}

bool collide(float tx, float ty, float cx, float cy) {
	return (tx - cx) * (tx - cx) + (ty - cy) * (ty - cy) <= 1.f/16 * 1.f/16;
}

// ==========================================================================
void MyWin::MainLoop() {
	ViewportOne(0,0,wd,ht);
	glLineWidth((GLfloat)4);

	MyCross cross;
	MyTri   trian;
	MyCircle circle(50);

	float   tx=0.0, ty=0.5;
	float   cx=0.0, cy=0.0;
	do {
		glClear( GL_COLOR_BUFFER_BIT );

		AGLErrors("main-loopbegin");
		// =====================================================        Drawing
		trian.draw();
		cross.draw(tx,ty);
		circle.draw(cx,cy);
		AGLErrors("main-afterdraw");

		glfwSwapBuffers(win()); // =============================   Swap buffers
		glfwPollEvents();
		//glfwWaitEvents();   

		if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
			tx += 0.01;
		}
		if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
			tx -= 0.01;
		}
		if (glfwGetKey(win(), GLFW_KEY_W ) == GLFW_PRESS) {
			cy += 0.01;
		}
		if (glfwGetKey(win(), GLFW_KEY_S ) == GLFW_PRESS) {
			cy -= 0.01;
		}
		if (glfwGetKey(win(), GLFW_KEY_A ) == GLFW_PRESS) {
			cx -= 0.01;
		}
		if (glfwGetKey(win(), GLFW_KEY_D ) == GLFW_PRESS) {
			cx += 0.01;
		}

		if(collide(tx, ty, cx, cy)) {
			std::cout << "A\n";
			cx = 0.f; cy = 0.f;
		}

	} while( glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(win()) == 0 );
}

int main(int argc, char *argv[]) {
	MyWin win;
	win.Init(800,600,"AGL3 example",0,33);
	win.MainLoop();
	return 0;
}
