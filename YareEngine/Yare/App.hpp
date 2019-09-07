

#pragma once

#include <string>
struct GLFWwindow;

#include <cstdlib>
#include <fstream>

#include <iostream>
#include <stdexcept>
#include <vector>

namespace yare
{

	struct AppConfig 
	{
		
		std::string title;
		int width, height;
	};

class App {
 public:
	 App(const AppConfig & config = {"Yare App", 1920 , 1080 });

  static App& getInstance();

  // get the window id
  GLFWwindow* getWindow() const;

  // window control
  void exit();

  // delta time between frame and time from beginning
  float getDeltaTime() const;
  float getTime() const;

  // App run
  void run();

  // App informations
  //
  int getWidth();
  int getHeight();
  float getWindowRatio();
  void resizeWindow(int newWidth, int newHeight);

 protected:
  //The Event Interface
  virtual void onLoad(const AppConfig & config );

  //The Event Interface
  virtual void onRender();
 
  // By default resizes viewport 
  virtual void onWindowResize(int newWidth, int newHeigh);
  void detectWindowResize();


private:
	enum State { stateReady, stateRun, stateExit };

	State _state;

	App& operator=(const App&) { return *this; }

	//TODO Move to window class
	GLFWwindow* _window;

	// Time:
	float _time;
	float _deltaTime;

	AppConfig _config;

};

}

