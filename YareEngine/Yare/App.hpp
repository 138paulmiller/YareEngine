

#pragma once

#include <string>

struct GLFWwindow;

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

