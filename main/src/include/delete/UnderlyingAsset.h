#pragma once
#include <util/Common.h>

namespace underlying_asset{
    class UnderAsset : public common::Asset, public common::Singleton<UnderAsset>{
        public:
            void getPrice();
            ~UnderAsset();

        private:
            UnderAsset();
            friend class Singleton<UnderAsset>;
    };
}