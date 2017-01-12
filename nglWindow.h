

#ifndef nglWindow_h__
#define nglWindow_h__

#include <ostream>

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW\glfw3.h>


struct nglWindowMouseCoords {

double x;
double y;

};

enum NGL_WINDOW_INPUT_TYPE {

	NGL_WINDOW_INPUT_TYPE_MOUSE,
	NGL_WINDOW_INPUT_TYPE_KEYBOARD
};

class nglWindow {

public:

	static void __stdcall OnKeyPress(GLFWwindow * window, int key, int scancode, int action, int mods);

	nglWindow(int width, int height, const char* title);
	~nglWindow();

	double GetCounterTime();
	bool IsKeyDown(int key);
	bool IsMouseKeyDown(int key);
	void SwapBuffers();
	void HookInputs();
	void PollEvents();
	nglWindowMouseCoords GetCurserPos();
	void SetMouseToCenter();
	//void SetCallbackMouse(void* func_ptr);

	//call this once in every render iteration.
	//used for fps
	void FrameTick();
	int GetFPS();

	void ToggleVSync(bool vsync);

	void SetWindowTitle(std::string& title);

	int GetWindowHeight();
	int GetWindowWidth();


private:
	GLFWwindow* _window; 
	int _windowWidth;
	int _windowHeigth;
	double _lastFrameTime;
	//number of frames drawn this second
	int _frameCountThisSecond;
	//last FPS value
	int _lastFPS;
	

};

#endif // nglWindow_h__