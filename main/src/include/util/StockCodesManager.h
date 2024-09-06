#pragma once

#include <vector>
#include <string>

class StockCodesManager {
public:
    static std::vector<std::string>& getStockCodes();
    static void init(); // 초기화 메서드
private:
    static void initializeFromFile(const std::string& filename);
    static std::vector<std::string> stockCodes;
};