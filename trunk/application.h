
#pragma once

#include <chrono>

#include <GLFW/glfw3.h>


class Application{
public:
  GLFWwindow* window;
  double time;
  double startTime;
  double timeSinceStart();

  double frameStartTime;
  double frameEndTime;
  double frameDelta;

  void close();
  double getTimestamp();
  Application();
  static Application& the() {
    static Application* my_application = nullptr;
    if (!my_application) {
      my_application = new Application;
    }
    return *my_application;
  }
};

