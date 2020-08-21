#ifndef MESSAGES_H
#define MESSAGES_H

#include "support.h"
#include "json.hpp"

using namespace nlohmann;

struct IMessageHandler;

namespace MemoryHelpers {
  json cppToJsonPointer(void* pointer);
  void* jsonToCppPointer(const json& value);

#ifdef __APPLE__
  // In the native build, we have to copy data between Javascript and CPP.
  // We use base-64 encoding to transfer data and pointer addresses.
  std::string createBuffer(int bytes);
  void destroyBuffer(const std::string& pointer);
  void memcpyJsonDataToJsonPointer(const json& data, const json& pointer);
#else
  // In the web build, Javascript can access the CPP heap. So, Javascript
  // asks CPP to allocate heap space (using createBuffer/destroyBuffer) and
  // then directly copies data into it.
  static unsigned long createBuffer(int bytes);
  static void destroyBuffer(unsigned long pointer);
#endif
}

namespace MessageBindings {
  void sendMessageToCPP(string value);
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