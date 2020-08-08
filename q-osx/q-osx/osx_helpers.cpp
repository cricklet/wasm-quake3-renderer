#include "osx_helpers.h"

#include "webview.h"

OSXWebView::OSXWebView(std::string url) {
  _w = new webview::webview {true, nullptr};
  _w->set_title("Minimal example");
  _w->set_size(300, 600, WEBVIEW_HINT_NONE);
  _w->navigate(url);
}

OSXWebView::~OSXWebView() {
  delete _w;
}

void OSXWebView::queueDispatch(void (*fn)(webview_t, void *)) {
  webview_dispatch(static_cast<webview_t>(_w), fn, this);
}

void OSXWebView::run() {
  _w->run();
}

void OSXWebView::eval(std::string javascript) {
  _w->eval(javascript);
  _w->resolve("asdf", 0, "asdf");
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
