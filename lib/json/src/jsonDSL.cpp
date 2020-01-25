#include<map>
#include<string>
#include "jsonDSL.h"

bool JsonDSL::isCommandInLanguage(std::string strCommand){
    return enumToStringMap.count(strCommand) == 1;
}
