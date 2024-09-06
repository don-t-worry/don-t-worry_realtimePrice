#include <api/UnderlyingAsset.h>
#include <util/Common.h>

namespace underlying_asset{
    UnderAsset::UnderAsset(): common::Asset() {}

    UnderAsset::~UnderAsset(){}

    void UnderAsset::getPrice(){
        std::map<std::string, std::string> headers;
        headers["approval_key"] = webSocketkey;
        headers["custtype"] = "P";
        headers["tr_type"] = "1";
        headers["content-type"] = "utf-8";

        nlohmann::json bodyJson;
        bodyJson["input"]["tr_id"] = "H0IFASP0";
        bodyJson["input"]["tr_key"] = "101S12";

        // 전체 메시지 구성
        nlohmann::json message;
        message["header"] = headers;
        message["body"] = bodyJson;
  
        common::WebSocket::getInstance().send(message);
    }
}