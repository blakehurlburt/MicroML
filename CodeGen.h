#include <string>
#include "CPPAST.h"

std::string generateCode(std::list < CPPFunction* > functions) {

    std::string str = "//Autogenerated C++ Source Code DO NOT EDIT\n";
    str += "#include <Arduino.h>\n";
    str += "#include <runtime.h>\n";
    //str += "#include \"arduino_bindings.h\"\n";
    str += "\n";

    for(CPPFunction* f : functions) {
        str += f->genCode();
    }

    str += "void setup() {\n";
    str += "\tSerial.begin(9600);\n";
    str += "\twhile(!Serial);\n";
    str += "\ttopEnv = new Environment();\n";
    str += "\tSerial.println(unwrapReal(_main_(makeRecord({}, topEnv), topEnv)));\n";
    str += "}\n";

    str += "void loop() {\n";
    str += "}\n";

    return str;
}
