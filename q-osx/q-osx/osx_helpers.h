#ifndef osx_helpers_h
#define osx_helpers_h

#include <string>
#include <thread>
#include <functional>

namespace webview {
  class webview;
}

typedef void *webview_t;

struct OSXWebView {
  static void setupInstance(std::string url);
  static OSXWebView* getInstance();

  void queueDispatch(void (*fn)(webview_t, void *));
  void run();
  void eval(std::string javascript);
private:
  OSXWebView(std::string url);
  ~OSXWebView();
  
  webview::webview* _w;
};

#endif
