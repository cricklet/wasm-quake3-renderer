#include "messages.h"

using namespace Messages;

void Messages::sendMessageToWeb(const TestMessage& message) {
  EM_ASM({
    console.log("value:", $0);
  }, message.toJson().c_str());
}

void Messages::handleMessageFromWeb(const TestMessage& message) {
  printf("receieved test message %s\n", message.text.c_str());
}

extern "C" {
  void EMSCRIPTEN_KEEPALIVE sendMessageToCPP(const void* value) {
    json j = (string) (const char*) value;
    if (j["type"] == "TestMessage") {
      TestMessage message = TestMessage::fromJson(j);
      Messages::handleMessageFromWeb(message);
    }
  }
}

