#ifndef MESSAGES_H
#define MESSAGES_H

#include "support.h"
#include "json.hpp"

using namespace nlohmann;

struct IMessageHandler;

namespace TypeConverters {
  json cppToJsonPointer(void* pointer);
  void* jsonToCppPointer(const json& value);

#ifdef __APPLE__
  void memcpyJsonDataToJsonPointer(const json& data, const json& pointer);
#endif
}

namespace MessageBindings {
  void sendMessageToCPP(string value);

#ifdef __APPLE__
#else
static unsigned long createBuffer(int bytes);
static void destroyBuffer(unsigned long pointer);
#endif
};

struct MessagesFromWeb {
  static shared_ptr<MessagesFromWeb> getInstance();

  void registerHandler(shared_ptr<IMessageHandler> handler);
  void sendMessage(const json& value);

private:
  vector<shared_ptr<IMessageHandler>> _handlers;

  static shared_ptr<MessagesFromWeb> _instance;
};

#endif