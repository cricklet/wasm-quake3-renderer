#include "binding_helpers.h"
#include "bindings.h"

#ifdef __APPLE__
#else
#include <emscripten/bind.h>
#include <emscripten/val.h>
#endif

void MessageBindings::sendMessageToCPP(string value) {
  json j = json::parse(value);
  MessagesFromWeb::getInstance()->sendMessage(j);
}

#ifdef __APPLE__
#else
EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("sendMessageToCPP", &MessageBindings::sendMessageToCPP);
}

extern "C" {
  EMSCRIPTEN_KEEPALIVE void* CPP_createBuffer(int bytes) {
    cout << "malloc for " <<  bytes << " bytes\n";
    return malloc(bytes * sizeof(char));
  }

  EMSCRIPTEN_KEEPALIVE void CPP_destroyBuffer(void* pointer) {
    free(pointer);
  }
}
#endif

shared_ptr<MessagesFromWeb> MessagesFromWeb::_instance = nullptr;

shared_ptr<MessagesFromWeb> MessagesFromWeb::getInstance() {
  if (!_instance) {
    _instance = make_shared<MessagesFromWeb>();
  }

  return _instance;
}

void MessagesFromWeb::registerHandler(shared_ptr<IMessageHandler> handler) {
  _handlers.push_back(handler);
}
