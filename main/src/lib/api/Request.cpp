#include "api/Request.h"
#include "util/StockCodesManager.h"
#include <iostream>
// #include <yaml-cpp/yaml.h>
// #include <util/Resources.h>

#include <thread>

const std::vector<std::string>& Request::stockCodes = StockCodesManager::getStockCodes();
int Request::size = 0; 

Request::Request(int num){

    appkey = Resources::authorization[num].appkey;
    secretkey = Resources::authorization[num].secretkey;
    
    token = "";
    domain = "https://openapi.koreainvestment.com:9443";

    stockPriceHeader["appkey"] = appkey;
    stockPriceHeader["appsecret"] = secretkey;
    stockPriceHeader["tr_id"] = "FHKST11300006";
    stockPriceHeader["custtype"] = "P";
    stockPriceHeader["authorization"] = token;
    
    tokenBody["grant_type"] = "client_credentials";
    tokenBody["appkey"] = appkey;
    tokenBody["appsecret"] = secretkey;

    size = stockCodes.size();
    
}

Request::~Request(){
    redisConfig.stop();
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
    
    int index = startIndex-1;
    for(int i=0; i<30; i++){
        index = (index+1) % size;

        params["FID_COND_MRKT_DIV_CODE_"+ std::to_string(count)] = "J";
        params["FID_INPUT_ISCD_"+std::to_string(count)] = stockCodes[index];
        count++;
    }

    std::map<std::string,std::string> insertedCodes;


    nlohmann::json response = httpClient.doGet(domain+url,params, stockPriceHeader);

    if(response.contains("output")){
        for(const auto& item : response["output"]){
            std::string code = item["inter_shrn_iscd"];
            std::string price = item["inter2_prpr"];

            insertedCodes[code] = price;
        }
        redisConfig.insert(insertedCodes);
    }else{
        std::cout<<response<<std::endl;
    }


}