#include "bindings.h"
#include "binding_helpers.h"
#ifdef __APPLE__
////////////////////////////////////////////////
// Native bindings
#include "osx_helpers.h"
void MessageBindings::sendMessageToWeb(const TestMessage& message) {
  auto json = message.toJson();
  std::replace(json.begin(), json.end(), '"', '\'');
  std::string evalString = "window.MessageHandler.handleMessageFromCPP(JSON.stringify(" + json + "));";
  OSXWebView::getInstance()->eval(evalString);
}
void MessageBindings::sendMessageToWeb(const TestPointer& message) {
  auto json = message.toJson();
  std::replace(json.begin(), json.end(), '"', '\'');
  std::string evalString = "window.MessageHandler.handleMessageFromCPP(JSON.stringify(" + json + "));";
  OSXWebView::getInstance()->eval(evalString);
}
void MessageBindings::sendMessageToWeb(const OSXReady& message) {
  auto json = message.toJson();
  std::replace(json.begin(), json.end(), '"', '\'');
  std::string evalString = "window.MessageHandler.handleMessageFromCPP(JSON.stringify(" + json + "));";
  OSXWebView::getInstance()->eval(evalString);
}
void MessageBindings::sendMessageToWeb(const LoadResource& message) {
  auto json = message.toJson();
  std::replace(json.begin(), json.end(), '"', '\'');
  std::string evalString = "window.MessageHandler.handleMessageFromCPP(JSON.stringify(" + json + "));";
  OSXWebView::getInstance()->eval(evalString);
}
void MessageBindings::sendMessageToWeb(const LoadShaders& message) {
  auto json = message.toJson();
  std::replace(json.begin(), json.end(), '"', '\'');
  std::string evalString = "window.MessageHandler.handleMessageFromCPP(JSON.stringify(" + json + "));";
  OSXWebView::getInstance()->eval(evalString);
}
void MessageBindings::sendMessageToWeb(const LoadedShaders& message) {
  auto json = message.toJson();
  std::replace(json.begin(), json.end(), '"', '\'');
  std::string evalString = "window.MessageHandler.handleMessageFromCPP(JSON.stringify(" + json + "));";
  OSXWebView::getInstance()->eval(evalString);
}
void MessageBindings::sendMessageToWeb(const LoadedTexture& message) {
  auto json = message.toJson();
  std::replace(json.begin(), json.end(), '"', '\'');
  std::string evalString = "window.MessageHandler.handleMessageFromCPP(JSON.stringify(" + json + "));";
  OSXWebView::getInstance()->eval(evalString);
}
void MessageBindings::sendMessageToWeb(const MissingTexture& message) {
  auto json = message.toJson();
  std::replace(json.begin(), json.end(), '"', '\'');
  std::string evalString = "window.MessageHandler.handleMessageFromCPP(JSON.stringify(" + json + "));";
  OSXWebView::getInstance()->eval(evalString);
}
void MessageBindings::sendMessageToWeb(const LoadedBSP& message) {
  auto json = message.toJson();
  std::replace(json.begin(), json.end(), '"', '\'');
  std::string evalString = "window.MessageHandler.handleMessageFromCPP(JSON.stringify(" + json + "));";
  OSXWebView::getInstance()->eval(evalString);
}
void MessageBindings::sendMessageToWeb(const LoadedTextureOptions& message) {
  auto json = message.toJson();
  std::replace(json.begin(), json.end(), '"', '\'');
  std::string evalString = "window.MessageHandler.handleMessageFromCPP(JSON.stringify(" + json + "));";
  OSXWebView::getInstance()->eval(evalString);
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
void MessageBindings::sendMessageToWeb(const TestPointer& message) {
  emscripten::val MessageHandler = emscripten::val::global("MessageHandler");
  if (!MessageHandler.as<bool>()) {
    cerr << "No global MessageHandler\n";
    return;
  }
  MessageHandler.call<void>("handleMessageFromCPP", emscripten::val(message.toJson()));
}
void MessageBindings::sendMessageToWeb(const OSXReady& message) {
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
string TestMessage::toJson() const {
  json j;
  j["type"] = "TestMessage";
  j["text"] = (text);
  return j.dump();
}
TestMessage TestMessage::fromJson(const json& j) {
  return TestMessage {
    (j["text"]),
  };
}
string TestPointer::toJson() const {
  json j;
  j["type"] = "TestPointer";
  j["pointer"] = TypeConverters::cppToJsonPointer(pointer);
  return j.dump();
}
TestPointer TestPointer::fromJson(const json& j) {
  return TestPointer {
    TypeConverters::jsonToCppPointer(j["pointer"]),
  };
}
string OSXReady::toJson() const {
  json j;
  j["type"] = "OSXReady";
  return j.dump();
}
OSXReady OSXReady::fromJson(const json& j) {
  return OSXReady {
  };
}
string LoadResource::toJson() const {
  json j;
  j["type"] = "LoadResource";
  j["url"] = (url);
  j["resourceType"] = (resourceType);
  j["resourceID"] = (resourceID);
  return j.dump();
}
LoadResource LoadResource::fromJson(const json& j) {
  return LoadResource {
    (j["url"]),
    (j["resourceType"]),
    (j["resourceID"]),
  };
}
string LoadShaders::toJson() const {
  json j;
  j["type"] = "LoadShaders";
  j["vertUrl"] = (vertUrl);
  j["fragUrl"] = (fragUrl);
  j["resourceID"] = (resourceID);
  return j.dump();
}
LoadShaders LoadShaders::fromJson(const json& j) {
  return LoadShaders {
    (j["vertUrl"]),
    (j["fragUrl"]),
    (j["resourceID"]),
  };
}
string LoadedShaders::toJson() const {
  json j;
  j["type"] = "LoadedShaders";
  j["resourceID"] = (resourceID);
  j["vertPointer"] = TypeConverters::cppToJsonPointer(vertPointer);
  j["fragPointer"] = TypeConverters::cppToJsonPointer(fragPointer);
  j["vertLength"] = (vertLength);
  j["fragLength"] = (fragLength);
  return j.dump();
}
LoadedShaders LoadedShaders::fromJson(const json& j) {
  return LoadedShaders {
    (j["resourceID"]),
    TypeConverters::jsonToCppPointer(j["vertPointer"]),
    TypeConverters::jsonToCppPointer(j["fragPointer"]),
    (j["vertLength"]),
    (j["fragLength"]),
  };
}
string LoadedTexture::toJson() const {
  json j;
  j["type"] = "LoadedTexture";
  j["resourceID"] = (resourceID);
  j["pointer"] = TypeConverters::cppToJsonPointer(pointer);
  j["width"] = (width);
  j["height"] = (height);
  return j.dump();
}
LoadedTexture LoadedTexture::fromJson(const json& j) {
  return LoadedTexture {
    (j["resourceID"]),
    TypeConverters::jsonToCppPointer(j["pointer"]),
    (j["width"]),
    (j["height"]),
  };
}
string MissingTexture::toJson() const {
  json j;
  j["type"] = "MissingTexture";
  j["resourceID"] = (resourceID);
  return j.dump();
}
MissingTexture MissingTexture::fromJson(const json& j) {
  return MissingTexture {
    (j["resourceID"]),
  };
}
string LoadedBSP::toJson() const {
  json j;
  j["type"] = "LoadedBSP";
  j["resourceID"] = (resourceID);
  j["pointer"] = TypeConverters::cppToJsonPointer(pointer);
  return j.dump();
}
LoadedBSP LoadedBSP::fromJson(const json& j) {
  return LoadedBSP {
    (j["resourceID"]),
    TypeConverters::jsonToCppPointer(j["pointer"]),
  };
}
string LoadedTextureOptions::toJson() const {
  json j;
  j["type"] = "LoadedTextureOptions";
  j["resourceID"] = (resourceID);
  j["surfaceParamTrans"] = (surfaceParamTrans);
  return j.dump();
}
LoadedTextureOptions LoadedTextureOptions::fromJson(const json& j) {
  return LoadedTextureOptions {
    (j["resourceID"]),
    (j["surfaceParamTrans"]),
  };
}
void MessagesFromWeb::sendMessage(const json& j) {
  if (j["type"] == "TestMessage") {
    auto message = TestMessage::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
  if (j["type"] == "TestPointer") {
    auto message = TestPointer::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
  if (j["type"] == "OSXReady") {
    auto message = OSXReady::fromJson(j);
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