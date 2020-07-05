#ifndef MESSAGES_H
#define MESSAGES_H

#include "support.h"
#include "json.h"

using namespace nlohmann;

namespace Messages {
  struct TestMessage {
    string text;

    string toJson() const {
      json j;
      j["text"] = text;
      return j.dump();
    }
    static TestMessage fromJson(const json& j) {
      return TestMessage { j["text"] };
    }
  };

  void sendMessageToWeb(const TestMessage& message);
  void handleMessageFromWeb(const TestMessage& message);
}

extern "C" {
  void EMSCRIPTEN_KEEPALIVE sendMessageToCPP(const char* value);
}

#endif