#include <string>
#include "CPPAST.h"

std::string generateCode(std::list < CPPFunction* > functions) {

    std::string str = "//Autogenerated C++ Source Code DO NOT EDIT\n";
    str += "#include <Arduino.h>\n";
    str += "#include <runtime.h>\n";
    str += "\n";

    for(CPPFunction* f : functions) {
        str += f->genCode();
    }

    str += "void setup() {\n";
    str += "\tSerial.begin(9600);\n";
    str += "\twhile(!Serial);\n";
    str += "\tSerial.println(unwrapInt(_main_(makeRecord({}), new Environment())));\n";
    str += "}\n";

    str += "void loop() {\n";
    str += "}\n";

    return str;
}
