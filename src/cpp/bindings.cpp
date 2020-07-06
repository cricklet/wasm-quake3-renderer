#include "bindings.h"
#include <emscripten/val.h>
void MessageBindings::sendMessageToWeb(const TestMessage& message) {
  emscripten::val MessageHandler = emscripten::val::global("MessageHandler");
  if (!MessageHandler.as<bool>()) {
    cerr << "No global MessageHandler\n";
    return;
  }
  MessageHandler.call<void>("handleMessageFromCPP", emscripten::val(message.toJson()));
}
void MessageBindings::sendMessageToWeb(const CPPLoaded& message) {
  emscripten::val MessageHandler = emscripten::val::global("MessageHandler");
  if (!MessageHandler.as<bool>()) {
    cerr << "No global MessageHandler\n";
    return;
  }
  MessageHandler.call<void>("handleMessageFromCPP", emscripten::val(message.toJson()));
}
void MessageBindings::sendMessageToWeb(const LoadResource& message) {
  emscripten::val MessageHandler = emscripten::val::global("MessageHandler");
  if (!MessageHandler.as<bool>()) {
    cerr << "No global MessageHandler\n";
    return;
  }
  MessageHandler.call<void>("handleMessageFromCPP", emscripten::val(message.toJson()));
}
void MessageBindings::sendMessageToWeb(const LoadShaders& message) {
  emscripten::val MessageHandler = emscripten::val::global("MessageHandler");
  if (!MessageHandler.as<bool>()) {
    cerr << "No global MessageHandler\n";
    return;
  }
  MessageHandler.call<void>("handleMessageFromCPP", emscripten::val(message.toJson()));
}
void MessageBindings::sendMessageToWeb(const LoadedImage& message) {
  emscripten::val MessageHandler = emscripten::val::global("MessageHandler");
  if (!MessageHandler.as<bool>()) {
    cerr << "No global MessageHandler\n";
    return;
  }
  MessageHandler.call<void>("handleMessageFromCPP", emscripten::val(message.toJson()));
}
void MessageBindings::sendMessageToWeb(const LoadedBSP& message) {
  emscripten::val MessageHandler = emscripten::val::global("MessageHandler");
  if (!MessageHandler.as<bool>()) {
    cerr << "No global MessageHandler\n";
    return;
  }
  MessageHandler.call<void>("handleMessageFromCPP", emscripten::val(message.toJson()));
}