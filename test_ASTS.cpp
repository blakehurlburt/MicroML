
#include "AST.h"
#include <list>
#include<iostream>

BlockNode factorial() {
  return BlockNode( \
    std::list<StatementNode*>(1, new FunDeclNode( \
      new IdentifierNode("fact"), \
      new IdentifierNode("n"), \
      new IfNode( \
        new BinOpNode( \
          new IdentifierNode("<"), \
          new IdentifierNode("n"), \
          new IntNode(2) \
        ), \
        new IntNode(1), \
        new BinOpNode( \
          new IdentifierNode("*"), \
          new IdentifierNode("n"), \
          new InvokeNode( \
            new IdentifierNode("fact"), \
            new BinOpNode( \
              new IdentifierNode("-"), \
              new IdentifierNode("n"), \
              new IntNode(1) \
            ) \
          ) \
        ) \
      ) \
    ) \
  ));
};

int main() {
  std::cout << factorial().toString();
}
