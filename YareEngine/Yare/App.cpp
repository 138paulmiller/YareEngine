

#include "App.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>


#include "Graphics/OpenGL/OpenGLError.hpp"
namespace yare
{

App::App(const AppConfig & config)
    : _state(AppState::Ready), _time(0), _deltaTime(0), _config(config){
	
	std::cout << "[Info] GLFW initialisation" << std::endl;

	// initialize the GLFW library
	if (!glfwInit()) {
	throw std::runtime_error("Couldn't init GLFW");
	}

	// setting the opengl version
	int major = 3;
	int minor = 2;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window
	_window = glfwCreateWindow(_config.width, _config.height, _config.title.c_str(), NULL, NULL);
	if (!_window) {
		glfwTerminate();
		throw std::runtime_error("Couldn't create a window");
	}

	glfwMakeContextCurrent(_window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (err != GLEW_OK) {
		glfwTerminate();
		throw std::runtime_error(std::string("Could initialize GLEW, error = ") +
									(const char*)glewGetErrorString(err));
	}

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported " << version << std::endl;


	_renderer = Renderer::Create(config.renderAPI);

	resizeWindow(_config.width, _config.height);
	

	// vsync
	// glfwSwapInterval(false);
}


App::~App()
{
	delete _renderer;
}


void App::exit() {
  _state = AppState::Exit;
}

float App::getDeltaTime() const {
  return _deltaTime;
}

float App::getTime() const {
  return _time;
}

void App::run() {


	onEnter();

	_state = AppState::Running;

	// Make the window's context current
	glfwMakeContextCurrent(_window);

	_time = glfwGetTime();

	while (_state == AppState::Running) {

		// exit on window close button pressed
		if (glfwWindowShouldClose(_window))
		{
			break;
		}

		// compute new time and delta time
		float t = glfwGetTime();
		_deltaTime = t - _time;
		_time = t;

		// detech window related changes
		detectWindowResize();

		// execute the rendering
		onRender();
		//present the render data
		_renderer->render();

		// Swap Front and Back buffers (double buffering)
		glfwSwapBuffers(_window);

		// Pool and process events
		glfwPollEvents();
	}

	onExit();

  glfwTerminate();

}


void App::detectWindowResize()
{
	int newWidth, newHeight;
	glfwGetWindowSize(_window, &newWidth, &newHeight);
	bool dimensionChanged = (newWidth != _config.width || newHeight != _config.height);
	if ( dimensionChanged) {
		if (newWidth > 0 && newHeight > 0)
		{
			onWindowResize(newWidth, newHeight);
		}
		else
		{
			//Window was collapsed
		}
	}

}
void App::resizeWindow(int newWidth, int newHeight) {
	_config.width = newWidth;
	_config.height = newHeight;
	this->_renderer->resizeViewport(_config.width, _config.height);
}


int App::getWidth() {
  return _config.width;
}

int App::getHeight() {
  return _config.height;
}

float App::getWindowRatio() {
  return float(_config.width) / float(_config.height);
}

// Interface Events
void App::onWindowResize(int newWidth, int newHeight) {
	resizeWindow(newWidth, newHeight);
}




}