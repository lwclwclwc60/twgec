#ifndef CODGEN_FORMATTER_H
#define CODGEN_FORMATTER_H

#include "utils/utils.h"
#include <memory>
#include <string>
#include <vector>

namespace codegen {
namespace formatter {

using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;

class JsonNode {
public:
  JsonNode() {};
  virtual string to_string(int indentation) { return ""; };
};

class JsonValueNode : public JsonNode {
public:
  string value;
  JsonValueNode(string value) : JsonNode(), value(value) {};
  string to_string(int indentation) override { return value; }
};

class JsonArrayNode : public JsonNode {
public:
  vector<shared_ptr<JsonNode>> nodeList;
  JsonArrayNode() {};
  JsonArrayNode(shared_ptr<JsonNode> node) { addNode(node); };
  JsonArrayNode addNode(shared_ptr<JsonNode> node) {
    nodeList.push_back(node);
    return *this;
  };
  string to_string(int indentation) override {
    if (nodeList.empty())
      return "[]";
    string ret = "[\n";
    for (auto idx = 0; idx < nodeList.size(); idx++) {
      ret += inden(indentation + 4) + nodeList[idx]->to_string(indentation + 4);
      if (idx != nodeList.size() - 1)
        ret += ",";
      ret += "\n";
    }
    ret += inden(indentation) + "]";
    return ret;
  }
};

class JsonObjectNode : public JsonNode {
public:
  JsonObjectNode() : JsonNode() {};
  JsonObjectNode(string key, string value) : JsonNode() {
    addNode(key, value);
  };
  JsonObjectNode(vector<pair<string, string>> contexts) : JsonNode() {
    for (auto ctx : contexts)
      addNode(ctx.first, ctx.second);
  };
  vector<pair<string, shared_ptr<JsonNode>>> nodeList;
  JsonObjectNode &addNode(string key, string value) {
    nodeList.push_back({key, std::make_shared<JsonValueNode>(value)});
    return *this;
  };
  JsonObjectNode &addNode(string key, shared_ptr<JsonNode> node) {
    nodeList.push_back({key, node});
    return *this;
  };
  string to_string(int indentation) override {
    string ret = "{\n";
    for (auto idx = 0; idx < nodeList.size(); idx++) {
      auto pair = nodeList[idx];
      ret += inden(indentation + 4) + "\"" + pair.first + "\": ";
      ret += pair.second.get()->to_string(indentation + 4);
      if (idx != nodeList.size() - 1)
        ret += ",";
      ret += "\n";
    }
    ret += inden(indentation) + "}";
    return ret;
  }
};
} // namespace formatter
} // namespace codegen

#endif