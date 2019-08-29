

#pragma once

#include <string>
struct GLFWwindow;

#include <cstdlib>
#include <fstream>

#include <iostream>
#include <stdexcept>
#include <vector>
// file reading - move to os layer
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
  float getFrameDeltaTime() const;
  float getTime() const;

  // App run
  void run();

  // App informations
  //
  int getWidth();
  int getHeight();
  float getWindowRatio();
  bool windowDimensionChanged();

 private:
  enum State { stateReady, stateRun, stateExit };

  State state;

  App& operator=(const App&) { return *this; }

  GLFWwindow* window;

  // Time:
  float time;
  float deltaTime;

  // Dimensions:
  int width;
  int height;
  bool dimensionChanged;
  void detectWindowDimensionChange();

 protected:
  App(const App&){};

  std::string title;

  virtual void loop();
};

