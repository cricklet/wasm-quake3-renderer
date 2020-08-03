#include "osx_helpers.h"

#include <thread>

OSXWebView::OSXWebView(std::string url) {
}

OSXWebView::~OSXWebView() {
}

void OSXWebView::think() {
}

void OSXWebView::eval(std::string javascript) {
}

static OSXWebView* instance = nullptr;

OSXWebView* OSXWebView::getInstance() {
  assert(instance);
  return instance;
}

void OSXWebView::setupInstance(std::string url) {
  assert(!instance);
  instance = new OSXWebView(url);
}
