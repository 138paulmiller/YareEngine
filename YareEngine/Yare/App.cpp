/**
 * App.hpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */

#include "App.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

 // file reading - move to os layer
void getFileContents(const std::string& filename, std::string& contents) {
	std::ifstream file(filename, std::ios_base::binary);
	if (file) {
		file.seekg(0, std::ios_base::end);
		std::streamsize size = file.tellg();
		if (size > 0) {
			file.seekg(0, std::ios_base::beg);
			contents.resize(static_cast<size_t>(size));
			file.read(&contents[0], size);
		}
		contents.push_back('\0');
	}
	else {
		throw std::invalid_argument(std::string("The file ") + filename +
			" doesn't exists");
	}
}




using namespace std;

App* currentApp = NULL;

App& App::getInstance() {
  if (currentApp)
    return *currentApp;
  else
    throw std::runtime_error("There is no current App");
}

App::App()
    : state(stateReady), width(640), height(480), title("App") {
  currentApp = this;

  cout << "[Info] GLFW initialisation" << endl;

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
  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Couldn't create a window");
  }

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();

  if (err != GLEW_OK) {
    glfwTerminate();
    throw std::runtime_error(string("Could initialize GLEW, error = ") +
                             (const char*)glewGetErrorString(err));
  }

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  cout << "Renderer: " << renderer << endl;
  cout << "OpenGL version supported " << version << endl;

  // opengl configuration
  glEnable(GL_DEPTH_TEST);  // enable depth-testing
  glDepthFunc(GL_LESS);  // depth-testing interprets a smaller value as "closer"

  // vsync
  // glfwSwapInterval(false);
}

GLFWwindow* App::getWindow() const {
  return window;
}

void App::exit() {
  state = stateExit;
}

float App::getFrameDeltaTime() const {
  return deltaTime;
}

float App::getTime() const {
  return time;
}

void App::run() {
  state = stateRun;

  // Make the window's context current
  glfwMakeContextCurrent(window);

  time = glfwGetTime();

  while (state == stateRun) {
    // compute new time and delta time
    float t = glfwGetTime();
    deltaTime = t - time;
    time = t;

    // detech window related changes
    detectWindowDimensionChange();

    // execute the frame code
    loop();

    // Swap Front and Back buffers (double buffering)
    glfwSwapBuffers(window);

    // Pool and process events
    glfwPollEvents();
  }

  glfwTerminate();
}

void App::detectWindowDimensionChange() {
  int w, h;
  glfwGetWindowSize(getWindow(), &w, &h);
  dimensionChanged = (w != width || h != height);
  if (dimensionChanged) {
    width = w;
    height = h;
    glViewport(0, 0, width, height);
  }
}

void App::loop() {
  cout << "[INFO] : loop" << endl;
}

int App::getWidth() {
  return width;
}

int App::getHeight() {
  return height;
}

float App::getWindowRatio() {
  return float(width) / float(height);
}

bool App::windowDimensionChanged() {
  return dimensionChanged;
}
