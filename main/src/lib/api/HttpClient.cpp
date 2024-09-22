#include "api/HttpClient.h"
#include <iostream>


void HttpClient::init(){
    curl_global_init(CURL_GLOBAL_ALL);
}

void HttpClient::destroy(){
    curl_global_cleanup();
}

HttpClient::HttpClient(){
    curl = curl_easy_init();  // 인스턴스마다 curl 초기화
}

HttpClient::~HttpClient() {
    if (curl) {
        curl_easy_cleanup(curl);  // 인스턴스 파괴 시 curl 해제
    }
}

nlohmann::json HttpClient::doPost(const std::string& url,  const std::map<std::string, std::string>& postData, const std::map<std::string, std::string>& headers){
    if (curl) {
            
        nlohmann::json jsonData(postData);
        std::string postDataStr = jsonData.dump();

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postDataStr.c_str());

        struct curl_slist* chunk = NULL;
        chunk = curl_slist_append(chunk, "Content-Type: application/json; charset=utf-8");
        for (const auto& header : headers) {
            std::string headerString = header.first + ": " + header.second;
            chunk = curl_slist_append(chunk, headerString.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);


        std::string response = performRequest();
        curl_slist_free_all(chunk);

        return nlohmann::json::parse(response);
    }
    return nlohmann::json();
}


nlohmann::json HttpClient::doGet(const std::string& baseUrl, const std::map<std::string, std::string>& queryParams, const std::map<std::string, std::string>& headers) {
    if (curl) {
        // 쿼리 파라미터를 URL에 추가
        std::string url = baseUrl;
        if (!queryParams.empty()) {
            url += "?";
            bool first = true;
            for (const auto& [key, value] : queryParams) {
                if (!first) url += "&";
                char* escapedKey = curl_easy_escape(curl, key.c_str(), key.length());
                char* escapedValue = curl_easy_escape(curl, value.c_str(), value.length());
                    
                if (!escapedKey || !escapedValue) {
                    if (escapedKey) curl_free(escapedKey);
                    if (escapedValue) curl_free(escapedValue);
                    throw std::runtime_error("Failed to escape URL parameters");
                }

                url += escapedKey;
                url += "=";
                url += escapedValue;
                first = false;

                curl_free(escapedKey);
                curl_free(escapedValue);
            }
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

        struct curl_slist* chunk = NULL;
        chunk = curl_slist_append(chunk, "Content-Type: application/json; charset=utf-8");
        for (const auto& header : headers) {
            std::string headerString = header.first + ": " + header.second;
            chunk = curl_slist_append(chunk, headerString.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        std::string response = performRequest();
        curl_slist_free_all(chunk);

        return nlohmann::json::parse(response);
    }
    return nlohmann::json();
}


size_t HttpClient::writeCallback(void* contents, size_t size, size_t nmemb, void* userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string HttpClient::performRequest(){
    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return {};
    }

    return response;
}