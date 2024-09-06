#include <iostream>
#include <yaml-cpp/yaml.h>
#include "util/Token.h"

namespace token {

    std::string Token::grant_type = "client_credentials";
    std::string Token::appkey = "";
    std::string Token::secretkey = "";
    std::string Token::token = "";
    std::string Token::webSocketkey = "";

    Token::Token() : common::HttpClient() {}
 
    void Token::initialize(){
        if(!initial){
            initial = true;
            const char* configPath = CONFIG_YAML_PATH;
            
            YAML::Node config = YAML::LoadFile(configPath);
            appkey = config["authorization"]["real"]["appkey"].as<std::string>(); // 모의투자 계좌로 매매하기 위해 real -> test 로 변경
            secretkey = config["authorization"]["real"]["secretkey"].as<std::string>();

            common::Asset::setAppkey(appkey);
            common::Asset::setAppsecret(secretkey);

            fetchToken();
            fetchWebSocketkey();
        }
    }

    void Token::fetchToken(){
        std::map<std::string, std::string> postData;
        postData["grant_type"] = grant_type;
        postData["appkey"] = appkey;
        postData["appsecret"] = secretkey;

        nlohmann::json response = this->doPost(common::Address::present_domain + "/oauth2/tokenP", postData);
        
        token = "Bearer " + response["access_token"].get<std::string>();
        updateAssetToken();
    }

    void Token::fetchWebSocketkey(){
        std::map<std::string, std::string> postData;
        postData["grant_type"] = grant_type;
        postData["appkey"] = appkey;
        postData["secretkey"] = secretkey;

        nlohmann::json response = this->doPost(common::Address::present_domain + "/oauth2/Approval", postData);
        
        webSocketkey = response["approval_key"].get<std::string>();
        updateAssetWebSocketkey();
    }

    void Token::updateAssetToken(){
        common::Asset::setToken(Token::token);
    }
    void Token::updateAssetWebSocketkey(){
        common::Asset::setWebSocketkey(Token::webSocketkey);
    }

    Token::~Token(){}
}