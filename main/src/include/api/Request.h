#pragma once

#include <string>
#include <map>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "HttpClient.h"

class Request{
public:

    ~Request();
    Request(int num);

    void getToken();
    void getStockPrice(int startIndex, int endIndex);
private:
    std::string appkey;
    std::string secretkey;
    std::string token;
    std::string domain;

    std::map<std::string, std::string> stockPriceHeader;
    std::map<std::string, std::string> tokenBody;
    
    HttpClient httpClient;
};