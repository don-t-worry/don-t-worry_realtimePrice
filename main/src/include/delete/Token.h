#pragma once
#include <string>
#include "util/Common.h"

namespace token {

    class Token : public common::HttpClient , public common::Singleton<Token>{
    public:

        void updateAssetToken();
        void updateAssetWebSocketkey();

        void fetchToken();
        void fetchWebSocketkey();
        
        void initialize();

        ~Token();

    private:
        static std::string grant_type;
        static std::string appkey;
        static std::string secretkey;
        static std::string token;
        static std::string webSocketkey;
        
        bool initial = false;

        friend class Singleton<Token>;

        Token();
    };
}