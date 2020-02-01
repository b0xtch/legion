#ifndef ENGINE_H
#define ENGINE_H
#include <string>
#include <json.hpp>
#include <iostream>  
using namespace std;

template <typename T> 
class Engine { 
    public:
        Engine (T json);

        bool validGameConfig(T json);

    private:
        nlohmann::json json;
        nlohmann::json configuration;
        nlohmann::json constants;
        nlohmann::json variables;
        nlohmann::json perPlayer;
        nlohmann::json perAudience;
        nlohmann::json rules;
        std::unordered_map<std::string, void(*)()> gameConfig;

        void initalizeEngine();
        void buildGame();
        void mapKeyToValue();

        // only set methods, the build game method has access to private variables
        void setConfiguration(nlohmann::json configuration);
        void setConstants(nlohmann::json constants);
        void setVariables(nlohmann::json variables);
        void setPerPlayer(nlohmann::json perPlayer);
        void setPerAudience(nlohmann::json perAudience);
        void setRules(nlohmann::json rules);

        // Control Structure Methods
        void findAndExecute(/* find a specific function and execute dynamically*/);

};

#endif