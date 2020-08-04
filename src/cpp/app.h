#ifndef APP_H
#define APP_H

#include "support.h"

struct MessageLogger;
struct IScenario;

struct App {
  App();

  void loop(GLFWwindow* window);

private:
  shared_ptr<MessageLogger> _messageLogger;
  shared_ptr<IScenario> _currentScenario;
};

#endif
