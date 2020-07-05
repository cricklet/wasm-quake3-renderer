#ifndef BINDINGS_H
#define BINDINGS_H
#include "support.h"
#include "json.h"
using namespace nlohmann;
struct TestMessage {
  string text;
  string toJson() const {
    json j;
    j["type"] = "TestMessage";
    j["text"] = text;
    return j.dump();
  }
  static TestMessage fromJson(const json& j) {
    return TestMessage {
        j["text"]
    };
  }
};
struct CPPLoaded {
  string toJson() const {
    json j;
    j["type"] = "CPPLoaded";
    return j.dump();
  }
  static CPPLoaded fromJson(const json& j) {
    return CPPLoaded {
    };
  }
};
class IMessageHandler {
public:
  virtual ~IMessageHandler() {}
  virtual void handleMessageFromWeb(const TestMessage& message) {}
  virtual void handleMessageFromWeb(const CPPLoaded& message) {}
};
namespace MessageBindings {
  void sendMessageToWeb(const TestMessage& message);
  void sendMessageToWeb(const CPPLoaded& message);
};
#endif