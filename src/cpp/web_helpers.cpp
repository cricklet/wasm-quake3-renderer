#include "binding_helpers.h"

#ifdef __APPLE__
#else

#include <emscripten/bind.h>
#include <emscripten/val.h>

unsigned long MemoryHelpers::createBuffer(int bytes) {
  cout << "malloc for " <<  bytes << " bytes\n";
  void* pointer = malloc(bytes * sizeof(char));
  unsigned long address = (unsigned long) pointer;
  return address;
}
void MemoryHelpers::destroyBuffer(unsigned long address) {
  void* pointer = (void*) address;
  free(pointer);
}
EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("sendMessageToCPP", &MessageBindings::sendMessageToCPP);
  emscripten::function("createBuffer", &MemoryHelpers::createBuffer, emscripten::allow_raw_pointers());
  emscripten::function("destroyBuffer", &MemoryHelpers::destroyBuffer, emscripten::allow_raw_pointers());
}

json MemoryHelpers::cppToJsonPointer(void* pointer) {
  return (unsigned long) pointer;
}
void* MemoryHelpers::jsonToCppPointer(const json& pointerJson) {
  return (void*) (unsigned long) pointerJson;
}

#endif
