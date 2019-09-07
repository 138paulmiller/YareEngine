

#include "App.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>



namespace yare
{

App* currentApp = NULL;

App& App::getInstance() {
  if (currentApp)
    return *currentApp;
  else
    throw std::runtime_error("There is no current App");
}

App::App(const AppConfig & config)
    : _state(stateReady), _config(config) {
  currentApp = this;

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

  // opengl configuration
  glEnable(GL_DEPTH_TEST);  // enable depth-testing
  glDepthFunc(GL_LESS);  // depth-testing interprets a smaller value as "closer"

  // vsync
  // glfwSwapInterval(false);
}

GLFWwindow* App::getWindow() const {
  return _window;
}

void App::exit() {
  _state = stateExit;
}

float App::getDeltaTime() const {
  return _deltaTime;
}

float App::getTime() const {
  return _time;
}

void App::run() {
  _state = stateRun;

  // Make the window's context current
  glfwMakeContextCurrent(_window);

  _time = glfwGetTime();

  while (_state == stateRun) {

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

    // execute the frame code
    onRender();

    // Swap Front and Back buffers (double buffering)
    glfwSwapBuffers(_window);

    // Pool and process events
    glfwPollEvents();
  }

  glfwTerminate();
}


void App::detectWindowResize()
{
	int newWidth, newHeight;
	glfwGetWindowSize(getWindow(), &newWidth, &newHeight);
	bool dimensionChanged = (newWidth != _config.width || newHeight != _config.height);
	if (dimensionChanged) {

		onWindowResize(newWidth, newHeight);
	}

}
void App::resizeWindow(int newWidth, int newHeight) {
	_config.width = newWidth;
	_config.height = newHeight;
	glViewport(0, 0, _config.width, _config.height);
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

void App::onRender() {
	std::cout << "[INFO] : loop" << std::endl;
}

void App::onLoad(const AppConfig& config)
{

	std::cout << "[INFO] : Loaded" << std::endl;
}


}