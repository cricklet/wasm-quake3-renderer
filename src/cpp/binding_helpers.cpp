#include "binding_helpers.h"
#include "bindings.h"

#ifdef __APPLE__

#include "base64.h"

json TypeConverters::cppToJsonPointer(void* pointer) {
  int encodedLength;
  char* encodedString = base64(&pointer, sizeof(void*) / sizeof(unsigned char), &encodedLength);
  
  {
    // Demonstrate decoding
    int decodedLength;
    unsigned char* decoded = unbase64(encodedString, encodedLength, &decodedLength);
    
    void* decodedPointer;
    memcpy(&decodedPointer, decoded, decodedLength);
    
    assert(decodedPointer == pointer);
    
    free(decoded);
  }

  std::string result = encodedString;
  free(encodedString);

  return result;
}
void* TypeConverters::jsonToCppPointer(const json& pointerJson) {
  void* decodedPointer;

  { // Change the address for decodedPointer
    string encodedPointer = pointerJson;

    int decodedLength;
    unsigned char* decoded = unbase64(encodedPointer.c_str(), encodedPointer.size(), &decodedLength);
    
    memcpy(&decodedPointer, decoded, decodedLength);
  }
  
  return decodedPointer;
}


void TypeConverters::memcpyJsonDataToJsonPointer(const json& dataJson, const json& pointerJson) {
  void* decodedPointer = jsonToCppPointer(pointerJson);
  
  { // Clobber the data at decodedPointer
    string data = dataJson;

    int decodedLength;
    unsigned char* decoded = unbase64(data.c_str(), data.size(), &decodedLength);

    memcpy(decodedPointer, decoded, decodedLength);
  }
}

#else
#include <emscripten/bind.h>
#include <emscripten/val.h>
#endif

void MessageBindings::sendMessageToCPP(string value) {
  json j = json::parse(value);
  MessagesFromWeb::getInstance()->sendMessage(j);
}

#ifdef __APPLE__
#else
EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("sendMessageToCPP", &MessageBindings::sendMessageToCPP);
}

extern "C" {
  EMSCRIPTEN_KEEPALIVE void* CPP_createBuffer(int bytes) {
    cout << "malloc for " <<  bytes << " bytes\n";
    return malloc(bytes * sizeof(char));
  }

  EMSCRIPTEN_KEEPALIVE void CPP_destroyBuffer(void* pointer) {
    free(pointer);
  }
}
#endif

shared_ptr<MessagesFromWeb> MessagesFromWeb::_instance = nullptr;

shared_ptr<MessagesFromWeb> MessagesFromWeb::getInstance() {
  if (!_instance) {
    _instance = make_shared<MessagesFromWeb>();
  }

  return _instance;
}

void MessagesFromWeb::registerHandler(shared_ptr<IMessageHandler> handler) {
  _handlers.push_back(handler);
}
