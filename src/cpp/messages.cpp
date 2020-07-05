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

MessagesFromWeb* MessagesFromWeb::_instance = nullptr;

MessagesFromWeb* MessagesFromWeb::getInstance() {
  if (!_instance) {
    _instance = new MessagesFromWeb();
  }

  return _instance;
}

void MessagesFromWeb::sendMessage(const json& j) {
  if (j["type"] == "TestMessage") {
    TestMessage message = TestMessage::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
}

void MessagesFromWeb::registerHandler(shared_ptr<IMessageHandler> handler) {
  _handlers.push_back(handler);
}