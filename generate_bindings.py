from jinja2 import Template

from schema import messages

h_template = Template("""
#ifndef BINDINGS_H
#define BINDINGS_H

#include "support.h"
#include "json.h"

using namespace nlohmann;

{% for (name, values) in messages %}

struct {{ name }} {
  {% for (property_name, property_type) in values %}
  {{ property_type }} {{ property_name }};
  {% endfor %}

  string toJson() const {
    json j;
    j["type"] = "{{ name }}";
    {% for (property_name, property_type) in values %}
    j["{{ property_name }}"] = {{ property_name }};
    {% endfor %}
    return j.dump();
  }

  static {{ name }} fromJson(const json& j) {
    return {{ name }} {
      {% for (property_name, property_type) in values %}
        j["{{ property_name }}"]
      {% endfor %}
    };
  }
};

{% endfor %}

class IMessageHandler {
public:
  virtual ~IMessageHandler() {}
  {% for (name, values) in messages %}
  virtual void handleMessageFromWeb(const {{ name }}& message) {}
  {% endfor %}
};


namespace MessageBindings {
  {% for (name, values) in messages %}
  void sendMessageToWeb(const {{ name }}& message);
  {% endfor %}
};


#endif
""")

cpp_template = Template("""
#include "bindings.h"
#include <emscripten/val.h>

{% for (name, values) in messages %}
void MessageBindings::sendMessageToWeb(const {{ name }}& message) {
  emscripten::val MessageHandler = emscripten::val::global("MessageHandler");
  if (!MessageHandler.as<bool>()) {
    cerr << "No global MessageHandler\\n";
    return;
  }

  MessageHandler.call<void>("handleMessageFromCPP", emscripten::val(message.toJson()));
}
{% endfor %}
""")

def generate_h_file(messages):
  h_result = h_template.render(messages=messages)
  h_result = '\n'.join([line for line in h_result.split('\n') if line.strip() != ''])

  h_file = open('src/cpp/bindings.h', 'w')
  h_file.truncate(0)
  h_file.write(h_result)
  h_file.close()

  cpp_result = cpp_template.render(messages=messages)
  cpp_result = '\n'.join([line for line in cpp_result.split('\n') if line.strip() != ''])

  cpp_file = open('src/cpp/bindings.cpp', 'w')
  cpp_file.truncate(0)
  cpp_file.write(cpp_result)
  cpp_file.close()

messages = [
  ( 'TestMessage', [
      ('text', 'string')
    ]
  ),
  ( 'CPPLoaded', []
  )
]

generate_h_file(messages)