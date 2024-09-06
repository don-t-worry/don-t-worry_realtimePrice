#pragma once 
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_

#include "util/DataCollection.h"
#include <string>
#include <map>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <asio.hpp>
#include <thread>
#include <vector>

namespace common{

    template<typename T>
    class Singleton{
    public:
        static T& getInstance(){
            static T instance;
            return instance;
        }
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

        virtual ~Singleton() = default;
    protected:
        Singleton() = default;
    };


    class HttpClient{
    public:
        HttpClient();
        virtual ~HttpClient() = default;

        nlohmann::json doPost(const std::string& url,  const std::map<std::string, std::string>& postData, const std::map<std::string, std::string>& headers = {});
        nlohmann::json doGet(const std::string& baseUrl, const std::map<std::string, std::string>& queryParams = {}, const std::map<std::string, std::string>& headers = {});

    private:
        CURL* curl;

        static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
        std::string performRequest();
    };

    typedef websocketpp::client<websocketpp::config::asio_client> client;
    class WebSocket : public Singleton<WebSocket>{
    public:
        ~WebSocket();

        void onMessage(websocketpp::connection_hdl hdl, client::message_ptr msg);
        void send(const nlohmann::json& message);
        std::vector<std::string> split(const std::string &s, char delimiter);

    private:
        WebSocket();
        void connect();
        friend class Singleton<WebSocket>;
        client m_client;
        websocketpp::connection_hdl m_hdl;

        asio::io_context m_io_context;
        asio::io_context::strand m_strand;
        std::vector<std::thread> m_thread_pool;
        PriceData priceData;  
    };


    class Asset{
    public:
        Asset();
        virtual ~Asset() = default;
        static void setToken(const std::string& token);
        static void setAppkey(const std::string& appkey);
        static void setAppsecret(const std::string& secretkey);
        static void setWebSocketkey(const std::string& webSocketkey);
    protected:
        static std::string token;
        static std::string appkey;
        static std::string appsecret;
        static std::string webSocketkey;
    };


    class Address{
    public:
        static std::string test_domain;
        static std::string real_domain;
        static std::string present_domain;
        static std::string test_websocket;
        static std::string real_websocket;
        static std::string present_websocket;
    };


    
}