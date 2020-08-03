#include "bindings.h"
#include "binding_helpers.h"
#ifdef __APPLE__
////////////////////////////////////////////////
// Native bindings
void MessageBindings::sendMessageToWeb(const TestMessage& message) {
  // TODO
}
void MessageBindings::sendMessageToWeb(const CPPLoaded& message) {
  // TODO
}
void MessageBindings::sendMessageToWeb(const LoadResource& message) {
  // TODO
}
void MessageBindings::sendMessageToWeb(const LoadShaders& message) {
  // TODO
}
void MessageBindings::sendMessageToWeb(const LoadedShaders& message) {
  // TODO
}
void MessageBindings::sendMessageToWeb(const LoadedTexture& message) {
  // TODO
}
void MessageBindings::sendMessageToWeb(const MissingTexture& message) {
  // TODO
}
void MessageBindings::sendMessageToWeb(const LoadedBSP& message) {
  // TODO
}
void MessageBindings::sendMessageToWeb(const LoadedTextureOptions& message) {
  // TODO
}
#else
////////////////////////////////////////////////
// Emscripten bindings
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
void MessageBindings::sendMessageToWeb(const LoadedShaders& message) {
  emscripten::val MessageHandler = emscripten::val::global("MessageHandler");
  if (!MessageHandler.as<bool>()) {
    cerr << "No global MessageHandler\n";
    return;
  }
  MessageHandler.call<void>("handleMessageFromCPP", emscripten::val(message.toJson()));
}
void MessageBindings::sendMessageToWeb(const LoadedTexture& message) {
  emscripten::val MessageHandler = emscripten::val::global("MessageHandler");
  if (!MessageHandler.as<bool>()) {
    cerr << "No global MessageHandler\n";
    return;
  }
  MessageHandler.call<void>("handleMessageFromCPP", emscripten::val(message.toJson()));
}
void MessageBindings::sendMessageToWeb(const MissingTexture& message) {
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
void MessageBindings::sendMessageToWeb(const LoadedTextureOptions& message) {
  emscripten::val MessageHandler = emscripten::val::global("MessageHandler");
  if (!MessageHandler.as<bool>()) {
    cerr << "No global MessageHandler\n";
    return;
  }
  MessageHandler.call<void>("handleMessageFromCPP", emscripten::val(message.toJson()));
}
#endif
void MessagesFromWeb::sendMessage(const json& j) {
  if (j["type"] == "TestMessage") {
    auto message = TestMessage::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
  if (j["type"] == "CPPLoaded") {
    auto message = CPPLoaded::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
  if (j["type"] == "LoadResource") {
    auto message = LoadResource::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
  if (j["type"] == "LoadShaders") {
    auto message = LoadShaders::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
  if (j["type"] == "LoadedShaders") {
    auto message = LoadedShaders::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
  if (j["type"] == "LoadedTexture") {
    auto message = LoadedTexture::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
  if (j["type"] == "MissingTexture") {
    auto message = MissingTexture::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
  if (j["type"] == "LoadedBSP") {
    auto message = LoadedBSP::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
  if (j["type"] == "LoadedTextureOptions") {
    auto message = LoadedTextureOptions::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
}