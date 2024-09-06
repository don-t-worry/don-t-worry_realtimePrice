#pragma once
#include <util/Common.h>

namespace derivative{
    class Future : public common::Asset, public common::Singleton<Future>{
        public:
            void getFuturePrice(std::string ticker);
            ~Future();

        private:
            Future();
            friend class Singleton<Future>;
    };

    
    class Option : public common::Asset, public common::Singleton<Option>{
        public:
            void getOptionPrice(std::string ticker);
            ~Option();

        private:
            Option();
            friend class Singleton<Option>;
    };
    class FutureUnderAssetPrice : public common::HttpClient, public common::Asset, public common::Singleton<FutureUnderAssetPrice>{
        public:
            void getFutureUnderAssetPrice(std::string ticker);
            ~FutureUnderAssetPrice();

        private:
            FutureUnderAssetPrice();
            friend class Singleton<FutureUnderAssetPrice>;
    };
}