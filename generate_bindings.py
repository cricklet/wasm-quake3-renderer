from jinja2 import Template

import schema

def convert_ts_type(t):
  if t == 'int':
    return 'number'
  if t == 'void*':
    return 'number'
  return t

def cpp_to_json_cast(t):
  if t == 'void*':
    return '(unsigned long)'
  return ''

def json_to_cpp_cast(t):
  if t == 'void*':
    return '(void*)(unsigned long)'
  return ''

ts_template = Template("""

{% for (enum_name, enum_prefix, values) in enums %}
export enum {{ enum_name }} {
  {% for value in values %}
  {{ value }},
  {% endfor %}
  UNKNOWN
};
{% endfor %}

{% for (name, values) in messages %}

export interface {{ name }} {
  type: '{{ name }}'

  {% for (property_name, property_type) in values %}
  {{ property_name }}: {{ convert_ts_type(property_type) }};
  {% endfor %}
}

{% endfor %}

export type Message = { type: 'Unknown' } {% for (name, values) in messages %} | {{ name }} {% endfor %}

export function parseMessage(json: string): Message {
  const val = JSON.parse(json)
  switch (val.type) {
    {% for (name, values) in messages %}
    case '{{ name }}': return val as {{ name }}
    {% endfor %}
  }
  return { type: 'Unknown' }
}

""")

h_template = Template("""
#ifndef BINDINGS_H
#define BINDINGS_H

#include "support.h"
#include "json.h"

using namespace nlohmann;

{% for (enum_name, enum_prefix, values) in enums %}
enum {{ enum_name }} {
  {% for value in values %}
  {{ value }},
  {% endfor %}
  UNKNOWN
};
{% endfor %}

{% for (name, values) in messages %}

struct {{ name }} {
  {% for (property_name, property_type) in values %}
  {{ property_type }} {{ property_name }};
  {% endfor %}

  string toJson() const {
    json j;
    j["type"] = "{{ name }}";
    {% for (property_name, property_type) in values %}
    j["{{ property_name }}"] = {{ cpp_to_json_cast(property_type) }} {{ property_name }};
    {% endfor %}
    return j.dump();
  }

  static {{ name }} fromJson(const json& j) {
    return {{ name }} {
      {% for (property_name, property_type) in values %}
      {{ json_to_cpp_cast(property_type) }}j["{{ property_name }}"],
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

struct MessageLogger : IMessageHandler {
public:
  {% for (name, values) in messages %}
  void handleMessageFromWeb(const {{ name }}& message) override {
    cout << "TS => CPP w/ " << message.toJson() << "\\n";
  }
  {% endfor %}
};


#endif
""")

cpp_template = Template("""
#include "bindings.h"
#include "messages.h"
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

void MessagesFromWeb::sendMessage(const json& j) {
{% for (name, values) in messages %}
  if (j["type"] == "{{ name }}") {
    auto message = {{ name }}::fromJson(j);
    for (const auto& handler : _handlers) {
      handler->handleMessageFromWeb(message);
    }
  }
{% endfor %}
}
""")

def generate_h_file(messages, enums):
  h_result = h_template.render(messages=messages, enums=enums, cpp_to_json_cast=cpp_to_json_cast, json_to_cpp_cast=json_to_cpp_cast)
  h_result = '\n'.join([line for line in h_result.split('\n') if line.strip() != ''])

  h_file = open('src/cpp/bindings.h', 'w')
  h_file.truncate(0)
  h_file.write(h_result)
  h_file.close()

  cpp_result = cpp_template.render(messages=messages, enums=enums, cpp_to_json_cast=cpp_to_json_cast, json_to_cpp_cast=json_to_cpp_cast)
  cpp_result = '\n'.join([line for line in cpp_result.split('\n') if line.strip() != ''])

  cpp_file = open('src/cpp/bindings.cpp', 'w')
  cpp_file.truncate(0)
  cpp_file.write(cpp_result)
  cpp_file.close()

def generate_ts_file(messages, enums):
  ts_result = ts_template.render(messages=messages, enums=enums, convert_ts_type=convert_ts_type)
  ts_result = '\n'.join([line for line in ts_result.split('\n') if line.strip() != ''])

  ts_file = open('src/ts/bindings.ts', 'w')
  ts_file.truncate(0)
  ts_file.write(ts_result)
  ts_file.close()

generate_h_file(schema.messages, schema.enums)
generate_ts_file(schema.messages, schema.enums)
