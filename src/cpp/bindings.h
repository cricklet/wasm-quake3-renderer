#ifndef BINDINGS_H
#define BINDINGS_H
#include "support.h"
#include "json.hpp"
using namespace nlohmann;
enum ResourceType {
  BSP_FILE,
  IMAGE_FILE,
  UNKNOWN
};
struct TestMessage {
  string text;
  string toJson() const;
  static TestMessage fromJson(const json& j);
};
struct TestPointer {
  void* pointer;
  string toJson() const;
  static TestPointer fromJson(const json& j);
};
struct OSXReady {
  string toJson() const;
  static OSXReady fromJson(const json& j);
};
struct LoadResource {
  string url;
  ResourceType resourceType;
  int resourceID;
  string toJson() const;
  static LoadResource fromJson(const json& j);
};
struct LoadShaders {
  string vertUrl;
  string fragUrl;
  int resourceID;
  string toJson() const;
  static LoadShaders fromJson(const json& j);
};
struct LoadedShaders {
  int resourceID;
  void* vertPointer;
  void* fragPointer;
  int vertLength;
  int fragLength;
  string toJson() const;
  static LoadedShaders fromJson(const json& j);
};
struct LoadedTexture {
  int resourceID;
  void* pointer;
  int width;
  int height;
  string toJson() const;
  static LoadedTexture fromJson(const json& j);
};
struct MissingTexture {
  int resourceID;
  string toJson() const;
  static MissingTexture fromJson(const json& j);
};
struct LoadedBSP {
  int resourceID;
  void* pointer;
  string toJson() const;
  static LoadedBSP fromJson(const json& j);
};
struct LoadedTextureOptions {
  int resourceID;
  bool surfaceParamTrans;
  string toJson() const;
  static LoadedTextureOptions fromJson(const json& j);
};
class IMessageHandler {
public:
  virtual ~IMessageHandler() {}
  virtual void handleMessageFromWeb(const TestMessage& message) {}
  virtual void handleMessageFromWeb(const TestPointer& message) {}
  virtual void handleMessageFromWeb(const OSXReady& message) {}
  virtual void handleMessageFromWeb(const LoadResource& message) {}
  virtual void handleMessageFromWeb(const LoadShaders& message) {}
  virtual void handleMessageFromWeb(const LoadedShaders& message) {}
  virtual void handleMessageFromWeb(const LoadedTexture& message) {}
  virtual void handleMessageFromWeb(const MissingTexture& message) {}
  virtual void handleMessageFromWeb(const LoadedBSP& message) {}
  virtual void handleMessageFromWeb(const LoadedTextureOptions& message) {}
};
namespace MessageBindings {
  void sendMessageToWeb(const TestMessage& message);
  void sendMessageToWeb(const TestPointer& message);
  void sendMessageToWeb(const OSXReady& message);
  void sendMessageToWeb(const LoadResource& message);
  void sendMessageToWeb(const LoadShaders& message);
  void sendMessageToWeb(const LoadedShaders& message);
  void sendMessageToWeb(const LoadedTexture& message);
  void sendMessageToWeb(const MissingTexture& message);
  void sendMessageToWeb(const LoadedBSP& message);
  void sendMessageToWeb(const LoadedTextureOptions& message);
};
struct MessageLogger : IMessageHandler {
public:
  void handleMessageFromWeb(const TestMessage& message) override {
    cout << "TS => CPP w/ " << message.toJson() << "\n";
  }
  void handleMessageFromWeb(const TestPointer& message) override {
    cout << "TS => CPP w/ " << message.toJson() << "\n";
  }
  void handleMessageFromWeb(const OSXReady& message) override {
    cout << "TS => CPP w/ " << message.toJson() << "\n";
  }
  void handleMessageFromWeb(const LoadResource& message) override {
    cout << "TS => CPP w/ " << message.toJson() << "\n";
  }
  void handleMessageFromWeb(const LoadShaders& message) override {
    cout << "TS => CPP w/ " << message.toJson() << "\n";
  }
  void handleMessageFromWeb(const LoadedShaders& message) override {
    cout << "TS => CPP w/ " << message.toJson() << "\n";
  }
  void handleMessageFromWeb(const LoadedTexture& message) override {
    cout << "TS => CPP w/ " << message.toJson() << "\n";
  }
  void handleMessageFromWeb(const MissingTexture& message) override {
    cout << "TS => CPP w/ " << message.toJson() << "\n";
  }
  void handleMessageFromWeb(const LoadedBSP& message) override {
    cout << "TS => CPP w/ " << message.toJson() << "\n";
  }
  void handleMessageFromWeb(const LoadedTextureOptions& message) override {
    cout << "TS => CPP w/ " << message.toJson() << "\n";
  }
};
#endif