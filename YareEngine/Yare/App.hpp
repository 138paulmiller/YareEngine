

#pragma once

#include <string>
struct GLFWwindow;
#include <cstdlib>
#include <fstream>

#include <iostream>
#include <stdexcept>
#include <vector>
#include "Graphics/Renderer.hpp"

namespace yare
{

	struct AppConfig 
	{
		
		std::string title;
		int width, height;
		graphics::RenderAPI renderAPI;
	};
	enum class AppState { Ready, Running, Exit };

class App {
 public:
	App(const AppConfig & config = {"Yare App", 1920 , 1080, graphics::RenderAPI::OpenGL });
	virtual ~App() ;
	


	// get the window id
	GLFWwindow* getWindow() const;

	// window control
	void exit();

	// delta time between frame and time from beginning
	float getDeltaTime() const;
	float getTime() const;

	// runs the application
	void run();

	// App information
	//
	int getWidth();
	int getHeight();
	float getWindowRatio();
	void resizeWindow(int newWidth, int newHeight);

protected:
	//The Event Interface
	virtual void onEnter() = 0;
	virtual void onExit() = 0;

	virtual void onRender(graphics::Renderer * renderer) = 0;
	//End interface 
	
	// By default resizes viewport 
	virtual void onWindowResize(int newWidth, int newHeight);
	
	void detectWindowResize();


private:
	
	AppState _state;

	App& operator=(const App&) { return *this; }

	//TODO Move to window class
	GLFWwindow* _window;

	// Time:
	float _time;
	float _deltaTime;
	AppConfig _config;
	graphics::Renderer* _renderer;

	//Add layers. Each layer is render in order and will have its own render graph

};

}

