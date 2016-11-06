#include "AST.h"
#include "CPPAST.h"
#include "CodeGen.h"
#include "Translate.h"
#include <iostream>
#include <utility>

BlockNode* factorial() {
    return new BlockNode( \
        std::list<StatementNode*>({ \
        new FunDeclNode( \
            new IdentifierNode("fact"), \
            new IdentifierNode("n"), \
            new IfNode( \
                new BinOpNode( \
                    new IdentifierNode("_lt_"), \
                    new IdentifierNode("n"), \
                    new IntNode(2) \
                    ), \
                new IntNode(1), \
                new BinOpNode( \
                    new IdentifierNode("_mul_"), \
                    new IdentifierNode("n"), \
                    new InvokeNode( \
                        new IdentifierNode("fact"), \
                        new BinOpNode( \
                            new IdentifierNode("_sub_"), \
                            new IdentifierNode("n"), \
                            new IntNode(1) \
                            ) \
                        ) \
                    ) \
                ) \
            ), \
        new FunDeclNode( \
            new IdentifierNode("enter"), \
            new IdentifierNode("_"), \
            new InvokeNode( \
                new IdentifierNode("fact"), \
                new IntNode(5) \
                )
            )
    })
        );
};

int main() {

    // std::list<CPPFunction*> funList;
    // std::list<CPPStatement*> body;
    //
    // body.push_back(new CPPReturn(StringNode("hi").translate(funList)));
    //
    // funList.push_back(new CPPFunction("hello","_",body));

    std::cout << generateCode(pgrmTranslate(factorial()));
    return 0;
}
