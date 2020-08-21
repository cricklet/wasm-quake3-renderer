#include "binding_helpers.h"
#include "bindings.h"

void MessageBindings::sendMessageToCPP(string value) {
  json j = json::parse(value);
  MessagesFromWeb::getInstance()->sendMessage(j);
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
