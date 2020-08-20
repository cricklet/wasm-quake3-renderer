#include "osx_helpers.h"

#include "binding_helpers.h"

#include "webview.h"
#include <iostream>

#include "base64.h"


OSXWebView::OSXWebView(std::string url) {
  _w = new webview::webview {true, nullptr};
  _w->set_title("Minimal example");
  _w->set_size(300, 600, WEBVIEW_HINT_NONE);
  _w->navigate(url);
  
  _w->bind("_OSXsendMessageToCPP", [](std::string arguments) -> std::string {
    json arg0 = json::parse(arguments).at(0);
    MessageBindings::sendMessageToCPP(arg0);

    json result;
    result["success"] = true;
    return (std::string) result.dump();
  });

  _w->bind("_OSXheap8Set", [](std::string arguments) -> std::string {
    json dataJson = json::parse(arguments).at(0);
    json pointerJson = json::parse(arguments).at(1);
    
    void* decodedPointer;

    { // Change the address for decodedPointer
      string encodedPointer = pointerJson;

      int decodedLength;
      unsigned char* decoded = unbase64(encodedPointer.c_str(), encodedPointer.size(), &decodedLength);
      
      memcpy(&decodedPointer, decoded, decodedLength);
    }
    
    { // Clobber the data at decodedPointer
      string data = dataJson;

      int decodedLength;
      unsigned char* decoded = unbase64(data.c_str(), data.size(), &decodedLength);

      memcpy(decodedPointer, decoded, decodedLength);
    }
    
    json result;
    result["success"] = true;
    return result.dump();
  });

  _w->bind("_OSXcreateBuffer", [](std::string arguments) -> std::string {
    int bytes = json::parse(arguments).at(0);
    void* pointer = malloc(bytes * sizeof(char));

    // Put some random stuff in pointer
    for (int i = 0; i < bytes; i ++) {
      ((char*) pointer)[i] = 'a';
    }
    
    int encodedLength;
    char* encodedString = base64(&pointer, sizeof(void*) / sizeof(unsigned char), &encodedLength);
    
    {
      // Demonstrate decoding
      int decodedLength;
      unsigned char* decoded = unbase64(encodedString, encodedLength, &decodedLength);
      
      void* decodedPointer;
      memcpy(&decodedPointer, decoded, decodedLength);
      
      assert(decodedPointer == pointer);
      
      free(decoded);
    }

    json result;
    result["success"] = true;
    result["pointer"] = encodedString;
    
    free(encodedString);
    return (std::string) result.dump();
  });
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
