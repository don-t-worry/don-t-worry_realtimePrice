#include "api/Derivative.h"
#include "util/Common.h"

namespace derivative{

    Future::Future() : common::Asset(){}
    Future::~Future(){}
    void Future::getFuturePrice(std::string ticker){
        std::map<std::string, std::string> headers;
        headers["approval_key"] = webSocketkey;
        headers["custtype"] = "P";
        headers["tr_type"] = "1";
        headers["content-type"] = "utf-8";

        nlohmann::json bodyJson;
        bodyJson["input"]["tr_id"] = "H0ZFASP0";
        bodyJson["input"]["tr_key"] = ticker;
        // bodyJson["input"]["tr_key"] = "111V06";

        // 전체 메시지 구성
        nlohmann::json message;
        message["header"] = headers;
        message["body"] = bodyJson;


        common::WebSocket::getInstance().send(message);
    }


    Option::Option() : common::Asset(){}
    Option::~Option(){}
    void Option::getOptionPrice(std::string ticker){
        std::map<std::string, std::string> headers;
        headers["approval_key"] = webSocketkey;
        headers["custtype"] = "P";
        headers["tr_type"] = "1";
        headers["content-type"] = "utf-8";

        nlohmann::json bodyJson;
        bodyJson["input"]["tr_id"] = "H0ZOASP0";
        bodyJson["input"]["tr_key"] = ticker;
        // bodyJson["input"]["tr_key"] = "211V05059";

        // 전체 메시지 구성
        nlohmann::json message;
        message["header"] = headers;
        message["body"] = bodyJson;
  
        common::WebSocket::getInstance().send(message);
    }

    FutureUnderAssetPrice::FutureUnderAssetPrice() : common::HttpClient(), common::Asset(){}
    FutureUnderAssetPrice::~FutureUnderAssetPrice(){}
    void FutureUnderAssetPrice::getFutureUnderAssetPrice(std::string ticker){
        std::map<std::string, std::string> headers;
        headers["authorization"] = token;
        headers["appkey"] = appkey;
        headers["appsecret"] = appsecret;
        headers["tr_id"] = "FHPIF05030000";
        headers["custtype"] = "P";

        std::map<std::string, std::string> params;
        params["FID_COND_MRKT_DIV_CODE"] = "F";
        // params["FID_INPUT_ISCD"] = "150V8000";    
        params["FID_INPUT_ISCD"] = ticker;    
        params["FID_COND_MRKT_DIV_CODE1"] = "";
        params["FID_COND_SCR_DIV_CODE"] = "";    
        params["FID_MTRT_CNT"] = "";
        params["FID_COND_MRKT_CLS_CODE"] = "";    

        nlohmann::json response =  
        doGet(common::Address::present_domain + "/uapi/domestic-futureoption/v1/quotations/display-board-top", params, headers);

        std::cout << response << std::endl;
    }


}
