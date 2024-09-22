#pragma once

#include <string>
#include <map>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "api/HttpClient.h"
#include "util/RedisConfig.h"

#include "util/Resources.h"

class Request{
public:

    Request(int num);
    ~Request();

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

    RedisConfig redisConfig;

    static const std::vector<std::string>& stockCodes;
    static int size;

};