

#pragma once

#include <string>
struct GLFWwindow;

#include <cstdlib>
#include <fstream>

#include <iostream>
#include <stdexcept>
#include <vector>
// file reading - move to os layer
//To do create a window manager ! App can spawn and manage windowes c classes
void getFileContents(const std::string& filename, std::string& contents);

class App {
 public:
  App();

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
 private:
  enum State { stateReady, stateRun, stateExit };

  State state;

  App& operator=(const App&) { return *this; }

  //TODO Move to window class
  GLFWwindow* window;

  // Time:
  float time;
  float deltaTime;

  // Dimensions:
  int width;
  int height;

  void detectWindowResize();

 protected:
  //The Event Interface
  virtual void onRender();
 
  // By default resizes viewport 
  virtual void onWindowResize(int newWidth, int newHeigh);

  std::string title;

};
