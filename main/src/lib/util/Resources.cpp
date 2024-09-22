#include "util/Resources.h"
#include <yaml-cpp/yaml.h>
#include <iostream>


Authorization Resources::authorization[Resources::accountCount] = {};
int Resources::redisPort = 0;
std::string Resources::redisHost = "";
std::string Resources::redisPassword = "";

void Resources::initialize(){
    const char* configPath = CONFIG_YAML_PATH;
        
    YAML::Node config = YAML::LoadFile(configPath);

    for(int i=0; i<accountCount; i++){
        std::string appkey = config["authorization"]["account"][i]["appkey"].as<std::string>(); 
        std::string secretkey = config["authorization"]["account"][i]["secretkey"].as<std::string>();

        authorization[i] = {appkey,secretkey};
    }

    redisPort = config["redis"]["port"].as<int>();
    redisHost = config["redis"]["host"].as<std::string>();
    redisPassword = config["redis"]["password"].as<std::string>();

}
