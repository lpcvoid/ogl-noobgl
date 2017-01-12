#include "nglWindow.h"



nglWindow::nglWindow(int width, int height, const char* title)
{

	this->_frameCountThisSecond = 0;
	this->_lastFrameTime = 0.0;
	this->_windowHeigth = height;
	this->_windowWidth = width;

	// Initialise GLFW 
	if( !glfwInit() ) 
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}

	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);

	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	

	// Open a window and create its OpenGL context
	this->_window = glfwCreateWindow( width, height, title, NULL, NULL);
	if( this->_window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
	}


	glfwMakeContextCurrent(this->_window); 
	glewExperimental=true; 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(this->_window, GLFW_STICKY_KEYS, GL_TRUE);
}

nglWindow::~nglWindow()
{

}

double nglWindow::GetCounterTime()
{
	return glfwGetTime();
}

bool nglWindow::IsKeyDown(int key)
{
	return (glfwGetKey(this->_window,key) == GLFW_PRESS);
}

void nglWindow::SwapBuffers()
{
	glfwSwapBuffers(this->_window);
}

void nglWindow::HookInputs()
{
	//glfwSetKeyCallback(this->_window, (GLFWkeyfun) &nglWindow::OnKeyPress);
}

void __stdcall nglWindow::OnKeyPress(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	printf("key=%d, action=%d",key,action);
}

nglWindowMouseCoords nglWindow::GetCurserPos()
{
	nglWindowMouseCoords coords;
	glfwGetCursorPos(this->_window,&coords.x, &coords.y);
	return coords;
}

void nglWindow::SetMouseToCenter()
{
	glfwSetCursorPos(this->_window,_windowWidth / 2,_windowHeigth / 2);
}

void nglWindow::FrameTick()
{
	this->_frameCountThisSecond++;
}

int nglWindow::GetFPS()
{
	double time = this->GetCounterTime();
	//check if last fps calculation is more than 1 second ago
	if (time - this->_lastFrameTime > 1.0){
		this->_lastFrameTime = this->GetCounterTime();
		this->_lastFPS = _frameCountThisSecond;
		_frameCountThisSecond = 0;
	} 

	return this->_lastFPS;
}

void nglWindow::SetWindowTitle(std::string& title)
{
	glfwSetWindowTitle(this->_window,title.c_str());
}

bool nglWindow::IsMouseKeyDown(int key)
{
	return (glfwGetMouseButton(this->_window,key) == GLFW_PRESS);
}

void nglWindow::ToggleVSync(bool vsync)
{
	if (vsync){
		glfwSwapInterval(1);
	} else {
		glfwSwapInterval(0);
	}
	
}

void nglWindow::PollEvents()
{
	glfwPollEvents();
}

int nglWindow::GetWindowHeight()
{
	return _windowHeigth;
}

int nglWindow::GetWindowWidth()
{
	return _windowWidth;
}
