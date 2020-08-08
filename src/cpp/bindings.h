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
  string toJson() const {
    json j;
    j["type"] = "TestMessage";
    j["text"] =  text;
    return j.dump();
  }
  static TestMessage fromJson(const json& j) {
    return TestMessage {
      j["text"],
    };
  }
};
struct TSLoaded {
  string toJson() const {
    json j;
    j["type"] = "TSLoaded";
    return j.dump();
  }
  static TSLoaded fromJson(const json& j) {
    return TSLoaded {
    };
  }
};
struct LoadResource {
  string url;
  ResourceType resourceType;
  int resourceID;
  string toJson() const {
    json j;
    j["type"] = "LoadResource";
    j["url"] =  url;
    j["resourceType"] =  resourceType;
    j["resourceID"] =  resourceID;
    return j.dump();
  }
  static LoadResource fromJson(const json& j) {
    return LoadResource {
      j["url"],
      j["resourceType"],
      j["resourceID"],
    };
  }
};
struct LoadShaders {
  string vertUrl;
  string fragUrl;
  int resourceID;
  string toJson() const {
    json j;
    j["type"] = "LoadShaders";
    j["vertUrl"] =  vertUrl;
    j["fragUrl"] =  fragUrl;
    j["resourceID"] =  resourceID;
    return j.dump();
  }
  static LoadShaders fromJson(const json& j) {
    return LoadShaders {
      j["vertUrl"],
      j["fragUrl"],
      j["resourceID"],
    };
  }
};
struct LoadedShaders {
  int resourceID;
  void* vertPointer;
  void* fragPointer;
  int vertLength;
  int fragLength;
  string toJson() const {
    json j;
    j["type"] = "LoadedShaders";
    j["resourceID"] =  resourceID;
    j["vertPointer"] = (unsigned long) vertPointer;
    j["fragPointer"] = (unsigned long) fragPointer;
    j["vertLength"] =  vertLength;
    j["fragLength"] =  fragLength;
    return j.dump();
  }
  static LoadedShaders fromJson(const json& j) {
    return LoadedShaders {
      j["resourceID"],
      (void*)(unsigned long)j["vertPointer"],
      (void*)(unsigned long)j["fragPointer"],
      j["vertLength"],
      j["fragLength"],
    };
  }
};
struct LoadedTexture {
  int resourceID;
  void* pointer;
  int width;
  int height;
  string toJson() const {
    json j;
    j["type"] = "LoadedTexture";
    j["resourceID"] =  resourceID;
    j["pointer"] = (unsigned long) pointer;
    j["width"] =  width;
    j["height"] =  height;
    return j.dump();
  }
  static LoadedTexture fromJson(const json& j) {
    return LoadedTexture {
      j["resourceID"],
      (void*)(unsigned long)j["pointer"],
      j["width"],
      j["height"],
    };
  }
};
struct MissingTexture {
  int resourceID;
  string toJson() const {
    json j;
    j["type"] = "MissingTexture";
    j["resourceID"] =  resourceID;
    return j.dump();
  }
  static MissingTexture fromJson(const json& j) {
    return MissingTexture {
      j["resourceID"],
    };
  }
};
struct LoadedBSP {
  int resourceID;
  void* pointer;
  string toJson() const {
    json j;
    j["type"] = "LoadedBSP";
    j["resourceID"] =  resourceID;
    j["pointer"] = (unsigned long) pointer;
    return j.dump();
  }
  static LoadedBSP fromJson(const json& j) {
    return LoadedBSP {
      j["resourceID"],
      (void*)(unsigned long)j["pointer"],
    };
  }
};
struct LoadedTextureOptions {
  int resourceID;
  bool surfaceParamTrans;
  string toJson() const {
    json j;
    j["type"] = "LoadedTextureOptions";
    j["resourceID"] =  resourceID;
    j["surfaceParamTrans"] =  surfaceParamTrans;
    return j.dump();
  }
  static LoadedTextureOptions fromJson(const json& j) {
    return LoadedTextureOptions {
      j["resourceID"],
      j["surfaceParamTrans"],
    };
  }
};
class IMessageHandler {
public:
  virtual ~IMessageHandler() {}
  virtual void handleMessageFromWeb(const TestMessage& message) {}
  virtual void handleMessageFromWeb(const TSLoaded& message) {}
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
  void sendMessageToWeb(const TSLoaded& message);
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
  void handleMessageFromWeb(const TSLoaded& message) override {
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