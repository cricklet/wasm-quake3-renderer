#include "messages.h"
#include "bindings.h"

#include <emscripten/bind.h>
#include <emscripten/val.h>

void MessageBindings::sendMessageToCPP(string value) {
  json j = json::parse(value);
  MessagesFromWeb::getInstance()->sendMessage(j);
}

EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("sendMessageToCPP", &MessageBindings::sendMessageToCPP);
}

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