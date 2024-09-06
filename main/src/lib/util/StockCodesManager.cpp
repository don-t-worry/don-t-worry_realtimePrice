#include "StockCodes.h"
#include <fstream>
#include <iostream>

// 전역 주식 코드 벡터 정의
std::vector<std::string> StockCodesManager::stockCodes;

std::vector<std::string>& StockCodesManager::getStockCodes() {
    return stockCodes;
}

void StockCodesManager::initializeFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
        return;
    }

    stockCodes.clear();  // 기존 데이터 제거

    std::string line;
    while (std::getline(file, line)) {
        stockCodes.push_back(line);
    }

    file.close();
}

void StockCodesManager::init() {
    initializeFromFile("../../../../stock_codes.txt");
}
