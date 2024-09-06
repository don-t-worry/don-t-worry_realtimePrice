#pragma once

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <map>

class HttpClient{
public:
    HttpClient();
    virtual ~HttpClient();

    nlohmann::json doPost(const std::string& url,  const std::map<std::string, std::string>& postData, const std::map<std::string, std::string>& headers = {});
    nlohmann::json doGet(const std::string& baseUrl, const std::map<std::string, std::string>& queryParams = {}, const std::map<std::string, std::string>& headers = {});

    static void init();
    static void destroy();
private:
    CURL* curl;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
    std::string performRequest();
};