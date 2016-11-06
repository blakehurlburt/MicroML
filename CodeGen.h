#include <string>
#include "CPPAST.h"

std::string generateCode(std::list < CPPFunction* > functions) {

    std::string str = "//Autogenerated C++ Source Code DO NOT EDIT\n";
    str += "#include \"runtime.h\"\n";
    str += "#include <utility>\n";
    str += "#include <iostream>\n";
    str += "\n\n\n";

    for(CPPFunction* f : functions) {
        str += f->genCode();
    }

    str += "int main() {\n";
    str += "\tstd::cout << unwrapInt(_main_(makeRecord({}), new Environment())) << std::endl;\n";
    str += "\treturn 0;\n";
    str += "}\n";

    return str;
}