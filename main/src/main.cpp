#include <iostream>
#include <vector>
#include "HttpClient.h"
#include "Request.h"
#include "StockCodesManager.h"

using namespace std;

int main(){

    StockCodesManager.init();

    std::list<std::string>& list = StockCodesManager::getStockCodes();

    int count = 0;
    for (const std::string& code : list) {
        if (count >= 10) break; // 10개 항목만 출력
        std::cout << code << std::endl;
        ++count;
    }
    
    return 0;
}