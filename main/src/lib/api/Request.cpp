#include "Request.h"
#include "StockCodes.h"
#include <iostream>
#include <yaml-cpp/yaml.h>

Request::Request(int num){
    const char* configPath = CONFIG_YAML_PATH;
            
    YAML::Node config = YAML::LoadFile(configPath);

    std::string account = std::to_string(num);

    appkey = config["authorization"]["account"][account]["appkey"].as<std::string>(); 
    secretkey = config["authorization"]["account"][account]["secretkey"].as<std::string>();
    token = "";
    domain = "https://openapi.koreainvestment.com:9443";

    stockPriceHeader["appkey"] = appkey;
    stockPriceHeader["appsecret"] = secretkey;
    stockPriceHeader["tr_id"] = "FHKST11300006";
    stockPriceHeader["custtype"] = "P";
    stockPriceHeader["authorization"] = token;
    
    tokenBody["grant_type"] = "client_credentials";
    tokenBody["appkey"] = appkey;
    tokenBody["appsecret"] = appsecret;
    
}

void Request::getToken(){

    std::string url = "/oauth2/tokenP";

    nlohmann::json response = httpClient.doPost(domain+url,tokenBody);

    token = "Bearer " + response["access_token"].get<std::string>();

    stockPriceHeader["authorization"] = token;
}

void Request::getStockPrice(int startIndex, int endIndex){

    std::string url = "/uapi/domestic-stock/v1/quotations/intstock-multprice";

    std::map<std::string, std::string> params;

    int count=1;
    const std::vector<std::string>& stockCodes = StockCodesManager::getStockCodes();
    for(int i=startIndex; i<=endIndex; i++){
        params["FID_COND_MRKT_DIV_CODE_"+ std::to_string(count)] = "J";
        params["FID_INPUT_ISCD_"+std::to_string(count)] = stock_codes[i];
        count++;
    }

    nlohmann::json response = httpClient.doGet(domain+url,params, stockPriceHeader);
}