#ifndef BINDINGS_H
#define BINDINGS_H
#include "support.h"
#include "json.h"
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
struct LoadedImage {
  int resourceID;
  void* pointer;
  int width;
  int height;
  string toJson() const {
    json j;
    j["type"] = "LoadedImage";
    j["resourceID"] =  resourceID;
    j["pointer"] = (unsigned long) pointer;
    j["width"] =  width;
    j["height"] =  height;
    return j.dump();
  }
  static LoadedImage fromJson(const json& j) {
    return LoadedImage {
      j["resourceID"],
      (void*)(unsigned long)j["pointer"],
      j["width"],
      j["height"],
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
class IMessageHandler {
public:
  virtual ~IMessageHandler() {}
  virtual void handleMessageFromWeb(const TestMessage& message) {}
  virtual void handleMessageFromWeb(const CPPLoaded& message) {}
  virtual void handleMessageFromWeb(const LoadResource& message) {}
  virtual void handleMessageFromWeb(const LoadShaders& message) {}
  virtual void handleMessageFromWeb(const LoadedImage& message) {}
  virtual void handleMessageFromWeb(const LoadedBSP& message) {}
};
namespace MessageBindings {
  void sendMessageToWeb(const TestMessage& message);
  void sendMessageToWeb(const CPPLoaded& message);
  void sendMessageToWeb(const LoadResource& message);
  void sendMessageToWeb(const LoadShaders& message);
  void sendMessageToWeb(const LoadedImage& message);
  void sendMessageToWeb(const LoadedBSP& message);
};
#endif