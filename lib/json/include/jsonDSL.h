#include<map>

class JsonDSL{
public:
    JsonDSL();
    enum LanguageCommands{placeholder};
    bool isCommandInLanguage(std::string);
private:
    std::map<LanguageCommands, std::string> enumToStringMap;

};