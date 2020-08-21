#include "binding_helpers.h"

#ifdef __APPLE__
#else

#include <emscripten/bind.h>
#include <emscripten/val.h>

unsigned long MessageBindings::createBuffer(int bytes) {
  cout << "malloc for " <<  bytes << " bytes\n";
  void* pointer = malloc(bytes * sizeof(char));
  unsigned long address = (unsigned long) pointer;
  return address;
}
void MessageBindings::destroyBuffer(unsigned long address) {
  void* pointer = (void*) address;
  free(pointer);
}
EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("sendMessageToCPP", &MessageBindings::sendMessageToCPP);
  emscripten::function("createBuffer", &MessageBindings::createBuffer, emscripten::allow_raw_pointers());
  emscripten::function("destroyBuffer", &MessageBindings::destroyBuffer, emscripten::allow_raw_pointers());
}

json TypeConverters::cppToJsonPointer(void* pointer) {
  return (unsigned long) pointer;
}
void* TypeConverters::jsonToCppPointer(const json& pointerJson) {
  return (void*) (unsigned long) pointerJson;
}

#endif
