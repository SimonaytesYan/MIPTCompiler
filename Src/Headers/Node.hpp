#pragma once

#include "Tokens.hpp"

#include <string>

enum class NodeType {
    NUM,
    VAR,
    OPER,
    KEYWORD,
    FICT
};

struct Node {
    Node(const Node& other) = default;
    Node(Node&& other) = default;

    Node& operator=(Node& other) = default;
    Node& operator=(Node&& other) = default;

    Node(Node* left = nullptr, Node* right = nullptr) :
      left_(left),
      right_(right) { }

    virtual NodeType getNodeType() const { return NodeType::FICT; }

  private:
    Node* left_;
    Node* right_;
};

struct OperatorNode : Node {
    OperatorNode(OperatorType oper, Node* left = nullptr, Node* right = nullptr) :
      Node(left, right),
      oper_(oper) { }

    NodeType getNodeType() const override { return NodeType::OPER; }

  private:
    OperatorType oper_;
};

struct KeywordNode : Node {
    KeywordNode(KeywordType oper, Node* left = nullptr, Node* right = nullptr);

  private:
    KeywordType keyword_;
};

struct VarNode : Node {
    VarNode(std::string&& name) :
      Node(nullptr, nullptr),
      name_(name) { }

  private:
    std::string name_;
};

struct NumNode : Node {
    NumNode(int num) :
      Node(nullptr, nullptr),
      num_(num) { }

  private:
    int num_;
};
