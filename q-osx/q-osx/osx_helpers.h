#ifndef osx_helpers_h
#define osx_helpers_h

#include <string>
#include <thread>

struct OSXWebView {
  static void setupInstance(std::string url);
  static OSXWebView* getInstance();

  void think();
  void eval(std::string javascript);
private:
  OSXWebView(std::string url);
  ~OSXWebView();
};

#endif
