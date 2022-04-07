
#include "application.h"

Application::Application() {
  this->startTime = getTimestamp();
  frameStartTime = this->startTime;
  frameEndTime = 0;
  frameDelta = 1/120; // assuming 120 FPS to give a small delta for first frame
}

double Application::getTimestamp() {
  std::chrono::duration<double, std::milli> chronotime = std::chrono::high_resolution_clock::now().time_since_epoch();

  double doubletime = std::chrono::duration< double >(chronotime).count();

  return doubletime;
}

double Application::timeSinceStart() { // TODO needs a better name. ideas: uptime, runtime, lifetime, 
    return getTimestamp() - startTime; 
};

void Application::close() {
    glfwSetWindowShouldClose(window, true);
};

