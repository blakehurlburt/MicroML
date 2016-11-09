#include "CPPAST.h"
#include <list>

std::list < CPPFunction * > pgrmTranslate(BlockNode * pgrm) {

    std::list < CPPFunction * > functions;

    std::list < CPPStatement * > statements;
    for(StatementNode * s : pgrm->statements) {
        statements.push_back(s->translate(functions));
    }
    statements.push_back(new CPPReturn(new CPPInvokeExpression(new CPPLookupExpression("enter"),new CPPRecordExpression({}))));

    functions.push_back(new CPPFunction("_main_", "_", statements));

    return functions;

}
