#ifndef MESSAGES_H
#define MESSAGES_H

#include "support.h"
#include "json.h"

using namespace nlohmann;

struct IMessageHandler;

namespace MessageBindings {
  void sendMessageToCPP(string value);
};

struct MessagesFromWeb {
  static MessagesFromWeb* getInstance();

  void registerHandler(shared_ptr<IMessageHandler> handler);
  void sendMessage(const json& value);

private:
  vector<shared_ptr<IMessageHandler>> _handlers;

  static MessagesFromWeb* _instance;
};

#endif