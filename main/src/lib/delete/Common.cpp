#include "util/Common.h"
#include "util/Token.h"
#include "util/DataCollection.h"
#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include "arbitrage/putcall.h"


namespace common{
    HttpClient::HttpClient(){
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
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
        }

        return response;
    }


    WebSocket::WebSocket() : 
    m_strand(m_io_context),
    priceData("2024-08-04 12:34:56"){
        m_client.init_asio(&m_io_context);
        m_client.set_message_handler(bind(&WebSocket::onMessage, this, std::placeholders::_1, std::placeholders::_2));
        m_client.set_access_channels(websocketpp::log::alevel::none); // 모든 접근 로그 비활성화
        connect();

        int max_thread = 10;
        for(int i=0; i<max_thread; i++){
            m_thread_pool.emplace_back([this](){ m_io_context.run(); });
        }
    }


    WebSocket::~WebSocket(){
        m_io_context.stop();
        for(std::thread& t : m_thread_pool){
            if(t.joinable()){
                t.join();
            }
        }
    }
    
    std::vector<std::string> WebSocket::split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::stringstream ss(s);
        
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
    
    // Initialize the PriceDataPool with 40 PriceData instances
    PriceDataPool priceDataPool(40);
    
    void WebSocket::onMessage(websocketpp::connection_hdl hdl, client::message_ptr msg){
        
        m_strand.post([this,msg](){
        std::string message =  "";
        double futures_price, call_price, put_price; 

        std::string futures_code, call_code, put_code, template_code;

        put_call::PutCall& putcall = put_call::PutCall::getInstance();

            try {
                message = msg->get_payload();

                std::cout << "Received: " << message << std::endl;

                auto priceData = priceDataPool.getPriceDataInstance();

                    // 선물과 옵션 가격 동시에 저장이 안됨
                    // 선물가격만 가져오거나, 옵션 가격만 가져옴

                if (message[0] == '0' || message[1] == '1') {
                    std::vector<std::string> recvstr = split(message, '|');
                    std::string trid0 = recvstr[1];  // 두 번째 항목 (인덱스 1)을 추출
                    char call_or_put = recvstr[3][0];

                    if (trid0 == "H0ZFASP0"){
                        std::cout << "#### 주식선물 호가 ####"  << std::endl;
                        futures_price = stockhokaStockfuts(message);
                        priceData->setFuturesPrice(futures_price);
                    }
                    
                    if (trid0 == "H0ZOASP0"){
                        std::cout << "#### 주식옵션 호가 ####"  << std::endl;
                        std::string template_code = recvstr[3].substr(0,9);
                        

                        // template_code = 
                        // Put과 Call 구분하는 로직 추가하기
                        if (call_or_put == '2')
                        {   
                            call_price = stockhokaStockoptn(message, call_or_put); // 매도가
                            priceData->setCallPrice(call_price);
                        } else if (call_or_put == '3')
                        {
                            put_price = stockhokaStockoptn(message, call_or_put);  // 매수가
                            priceData->setPutPrice(put_price);
                        }
                    }                    

                    // 행사가 
                    std::string filePath = "/home/a123/algorithm/main/src/strike_prices.txt";
                    std::string tickerSymbol = "319V09060"; // Change code to recv
                    int strike_price = getPriceForTicker(tickerSymbol, filePath);
                    priceData->setStrikePrice(strike_price); // 행사가 설정

                
                    // // futures_code =
                    // // put_code =
                    // // call_code = 
                    
                    std::cout << "Futures:" << priceData->getFuturesPrice() << std::endl; // 선물호가 받았는지 확인
                    std::cout << "Call:" << priceData->getCallPrice() << std::endl; // 옵션호가 받았는지 확인
                    std::cout << "Put:" << priceData->getPutPrice() << std::endl; // 옵션호가 받았는지 확인
                    std::cout << "Strike:" << priceData->getStrikePrice() << std::endl; // 옵션호가 받았는지 확인

                    // 모든 가격이 설정되었는지 확인
                    if (priceData->allPricesSet()) {

                        std::cout << "All prices have been set." << std::endl;
                        

                        // 시세차익 포착됬는지 확인
                        if (priceData->calculateArbitrage("2024-08-04 12:34:56")){
                            
                            // 시세차익 매매하기

                            // 선물 매도
                            putcall.createLimitSellOrder("111V09", futures_price, 1);

                            // 콜 매수
                            putcall.createLimitBuyOrder("211V09060", call_price, 1);
                            std::this_thread::sleep_for(std::chrono::milliseconds(500));

                            // 풋 매도
                            putcall.createLimitSellOrder("311V09060", put_price, 1);

                            std::exit(EXIT_SUCCESS);

                        };

                        // 모든 가격 초기화
                        priceData->setPricesToNull("2024-08-04 12:34:56");

                    }



                } else {
                    nlohmann::json json = nlohmann::json::parse(message);
                    std::string trid = json["header"]["tr_id"];

                    if (trid != "PINGPONG") {
                        std::string rt_cd = json["body"]["rt_cd"];

                        if (rt_cd == "1") { // 예외처리
                            std::cout << "### ERROR RETURN CODE [ " << rt_cd << " ] MSG [ " << json["body"]["msg1"] << " ]" << std::endl;
                        } else if (rt_cd == "0") {
                            std::cout << "### RETURN CODE [ " << rt_cd << " ] MSG [ " << json["body"]["msg1"] << " ]" << std::endl;

                            //체결통보나 등등 복호화가 필요한 api에서 사용될 key값등을 처리해야함
                        }
                    } else if (trid == "PINGPONG") {
                        websocketpp::lib::error_code ec;
                        auto con = m_client.get_con_from_hdl(m_hdl);
                        con->pong(message);
                    }
                }


            } catch (const std::exception& e) {
                std::cerr << "Exception in thread: " << e.what() << std::endl;
            }
        });
    }

    void WebSocket::send(const nlohmann::json& message){
        m_strand.post([this, message]() {
            websocketpp::lib::error_code ec;
            m_client.send(m_hdl, message.dump(4), websocketpp::frame::opcode::text, ec);
            if (ec) {
                std::cout << "Send failed because: " << ec.message() << std::endl;
            }
        });
    }
    
    void WebSocket::connect(){
        websocketpp::lib::error_code ec;
        std::string uri = Address::present_websocket;
        client::connection_ptr con = m_client.get_connection(uri, ec);

        // 새 연결 핸들 설정
        m_hdl = con->get_handle();

        // 연결 시도
        m_client.connect(con);

        // 이벤트 루프를 별도 쓰레드에서 실행
        std::thread([this]() {
            m_client.run();
        }).detach();

        // // 연결이 설정될 때까지 대기
        while (con->get_state() != websocketpp::session::state::open) {
            m_client.run_one();
        }
    }
    

    std::string Address::test_domain = "https://openapivts.koreainvestment.com:29443";
    std::string Address::real_domain = "https://openapi.koreainvestment.com:9443";
    std::string Address::present_domain = Address::real_domain;
    std::string Address::test_websocket = "ws://ops.koreainvestment.com:31000";
    std::string Address::real_websocket = "ws://ops.koreainvestment.com:21000";
    std::string Address::present_websocket = real_websocket;

    std::string Asset::token = "";
    std::string Asset::appkey = "";
    std::string Asset::appsecret = "";
    std::string Asset::webSocketkey = "";
    Asset::Asset() {}
    void Asset::setToken(const std::string& newToken){
        token = newToken;
    }
    void Asset::setWebSocketkey(const std::string& newKey){
        webSocketkey = newKey;
    }
    void Asset::setAppkey(const std::string& newAppkey){
        appkey = newAppkey;
    }
    void Asset::setAppsecret(const std::string& newSecret){
        appsecret = newSecret;
    }


}