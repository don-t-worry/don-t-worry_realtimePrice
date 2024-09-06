#include "util/DataCollection.h"
#include <typeinfo>
#include <iostream>
#include <deque>
#include <optional>
#include <mutex>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <memory>
#include <mutex>

// 옵션 호가 출력
double stockhokaStockoptn(const std::string& data, const char call_or_put) {
    // 데이터를 '^' 기준으로 나누기 위해 스트링스트림과 벡터 사용
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> recvvalue;
    while (std::getline(ss, token, '^')) {
        recvvalue.push_back(token);
    }



    try{ 
        int size = recvvalue.size();
        if  (size == 68){
            // 결과 출력
            std::cout << "주식옵션  [" << recvvalue[0] << "]" << std::endl;
            std::cout << "영업시간  [" << recvvalue[1] << "]" << std::endl;
            std::cout << "====================================" << std::endl;
            std::cout << "옵션매도호가1  [" << recvvalue[2] << "],    매도호가건수1  [" << recvvalue[12] << "],    매도호가잔량1  [" << recvvalue[22] << "]" << std::endl;
            std::cout << "옵션매도호가2  [" << recvvalue[3] << "],    매도호가건수2  [" << recvvalue[13] << "],    매도호가잔량2  [" << recvvalue[23] << "]" << std::endl;
            std::cout << "옵션매도호가3  [" << recvvalue[4] << "],    매도호가건수3  [" << recvvalue[14] << "],    매도호가잔량3  [" << recvvalue[24] << "]" << std::endl;
            std::cout << "옵션매도호가4  [" << recvvalue[5] << "],    매도호가건수4  [" << recvvalue[15] << "],    매도호가잔량4  [" << recvvalue[25] << "]" << std::endl;
            std::cout << "옵션매도호가5  [" << recvvalue[6] << "],    매도호가건수5  [" << recvvalue[16] << "],    매도호가잔량5  [" << recvvalue[26] << "]" << std::endl;
            std::cout << "옵션매수호가1  [" << recvvalue[7] << "],    매수호가건수1  [" << recvvalue[17] << "],    매수호가잔량1  [" << recvvalue[27] << "]" << std::endl;
            std::cout << "옵션매수호가2  [" << recvvalue[8] << "],    매수호가건수2  [" << recvvalue[18] << "],    매수호가잔량2  [" << recvvalue[28] << "]" << std::endl;
            std::cout << "옵션매수호가3  [" << recvvalue[9] << "],    매수호가건수3  [" << recvvalue[19] << "],    매수호가잔량3  [" << recvvalue[29] << "]" << std::endl;
            std::cout << "옵션매수호가4  [" << recvvalue[10] << "],   매수호가건수4  [" << recvvalue[20] << "],    매수호가잔량4  [" << recvvalue[30] << "]" << std::endl;
            std::cout << "옵션매수호가5  [" << recvvalue[11] << "],    매수호가건수5  [" << recvvalue[21] << "],    매수호가잔량5  [" << recvvalue[31] << "]" << std::endl;
            std::cout << "====================================" << std::endl;
            std::cout << "총매도호가건수  [" << recvvalue[32] << "],    총매도호가잔량  [" << recvvalue[34] << "],    총매도호가잔량증감  [" << recvvalue[36] << "]" << std::endl;
            std::cout << "총매수호가건수  [" << recvvalue[33] << "],    총매수호가잔량  [" << recvvalue[35] << "],    총매수호가잔량증감  [" << recvvalue[37] << "]" << std::endl;
           
            if (call_or_put == '2') {
                return std::stod(recvvalue[2]);
            } else if (call_or_put == '3'){
                return std::stod(recvvalue[7]);
            }
        }
    } catch(...){
        std::cout << "Not Working" << std::endl;
    }

    return 0;
}

// 선물 호가 출력 
double stockhokaStockfuts(const std::string& data) {
    // Split the data string by '^'
    std::vector<std::string> recvvalue;
    std::stringstream ss(data);
    std::string token;

    while (std::getline(ss, token, '^')) {
        recvvalue.push_back(token);
    }

    
    try{
        int size = recvvalue.size();
        if  (size == 68){
            // Print data
            std::cout << "주식선물  [" << recvvalue[0] << "]\n";
            std::cout << "====================================\n";
            std::cout << "선물매도호가1  [" << recvvalue[2] << "]" << ",    매도호가건수1  [" << recvvalue[12] << "]" << ",    매도호가잔량1  [" << recvvalue[22] << "]\n";
            std::cout << "선물매도호가2  [" << recvvalue[3] << "]" << ",    매도호가건수2  [" << recvvalue[13] << "]" << ",    매도호가잔량2  [" << recvvalue[23] << "]\n";
            std::cout << "선물매도호가3  [" << recvvalue[4] << "]" << ",    매도호가건수3  [" << recvvalue[14] << "]" << ",    매도호가잔량3  [" << recvvalue[24] << "]\n";
            std::cout << "선물매도호가4  [" << recvvalue[5] << "]" << ",    매도호가건수4  [" << recvvalue[15] << "]" << ",    매도호가잔량4  [" << recvvalue[25] << "]\n";
            std::cout << "선물매도호가5  [" << recvvalue[6] << "]" << ",    매도호가건수5  [" << recvvalue[16] << "]" << ",    매도호가잔량5  [" << recvvalue[26] << "]\n";
            std::cout << "선물매수호가1  [" << recvvalue[7] << "]" << ",    매수호가건수1  [" << recvvalue[17] << "]" << ",    매수호가잔량1  [" << recvvalue[27] << "]\n";
            std::cout << "선물매수호가2  [" << recvvalue[8] << "]" << ",    매수호가건수2  [" << recvvalue[18] << "]" << ",    매수호가잔량2  [" << recvvalue[28] << "]\n";
            std::cout << "선물매수호가3  [" << recvvalue[9] << "]" << ",    매수호가건수3  [" << recvvalue[19] << "]" << ",    매수호가잔량3  [" << recvvalue[29] << "]\n";
            std::cout << "선물매수호가4  [" << recvvalue[10] << "]" << ",   매수호가건수4  [" << recvvalue[20] << "]" << ",    매수호가잔량4  [" << recvvalue[30] << "]\n";
            std::cout << "선물매수호가5  [" << recvvalue[11] << "]" << ",    매수호가건수5  [" << recvvalue[21] << "]" << ",    매수호가잔량5  [" << recvvalue[31] << "]\n";
            std::cout << "====================================\n";
            std::cout << "총매도호가건수  [" << recvvalue[32] << "]" << ",    총매도호가잔량  [" << recvvalue[34] << "]" << ",    총매도호가잔량증감  [" << recvvalue[36] << "]\n";
            std::cout << "총매수호가건수  [" << recvvalue[33] << "]" << ",    총매수호가잔량  [" << recvvalue[35] << "]" << ",    총매수호가잔량증감  [" << recvvalue[37] << "]\n";
            return std::stod(recvvalue[7]); // 매수호가1  반환
        }
    }catch(...)
    {
        std::cout << "Not working";
    }
    return 0;
}



PriceData::PriceData(const std::string& timestamp)
    : call_price(0), put_price(0), futures_price(0), strike_price(0), timestamp(timestamp) {}

double PriceData::getCallPrice() const { return call_price; }
double PriceData::getPutPrice() const { return put_price; }
double PriceData::getFuturesPrice() const { return futures_price; }
double PriceData::getStrikePrice() const { return strike_price; }
std::string PriceData::getTimestamp() const { return timestamp; }

void PriceData::setCallPrice(double callPrice) { call_price = callPrice; }
void PriceData::setPutPrice(double putPrice) { put_price = putPrice; }
void PriceData::setFuturesPrice(double futuresPrice) { futures_price = futuresPrice; }
void PriceData::setStrikePrice(double strikePrice) { strike_price = strikePrice; }

bool PriceData::allPricesSet() const {
    return call_price != 0 && put_price != 0 && futures_price != 0;
}


bool PriceData::calculateArbitrage(const std::string& timestamp){
    double arbitrage = futures_price - call_price + put_price - strike_price;

    if (arbitrage > 0){
        std::cout << "Arbitrage for " << timestamp << ": " << arbitrage << std::endl;
        return true;
    }
    
    return false;
}
void PriceData::setPricesToNull(const std::string& time){
    futures_price = 0;
    call_price = 0;
    put_price = 0;
    strike_price = 0;
}


PriceDataPool::PriceDataPool(size_t size) : currentIndex(0) {
    for (size_t i = 0; i < size; ++i) {
        pool.push_back(std::make_shared<PriceData>());
    }
}

std::shared_ptr<PriceData> PriceDataPool::getPriceDataInstance() {
    std::lock_guard<std::mutex> lock(mtx);
    while (pool[currentIndex]->allPricesSet()) {
        if (++currentIndex >= pool.size()) {
            currentIndex = 0;
        }
    }
    return pool[currentIndex];
}


// Function to extract numeric part from a string
std::string extractNumericPart(const std::string& str) {
    std::string result;
    for (char ch : str) {
        if (std::isdigit(ch)) {
            result += ch;
        }
    }
    return result;
}

// Function to clean and extract price from the given numeric part
int extractCleanedPrice(const std::string& numericPart) {
    // Ensure the numeric part is long enough
    if (numericPart.length() < 8) {
        std::cerr << "Numeric part is too short to contain a valid price" << std::endl;
        return -1;
    }
    
    // Remove the first 6 digits and the last 2 digits
    std::string cleanedPrice = numericPart.substr(6, numericPart.length() - 8);
    
    // Convert the cleaned price string to an integer
    try {
        int price = std::stoi(cleanedPrice);
        return price;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument for stoi: " << e.what() << std::endl;
        return -1;
    }
}

// Function to get the price for a given ticker symbol from a file
int getPriceForTicker(const std::string& tickerSymbol, const std::string& filePath) {
    std::ifstream myfile(filePath);
    if (!myfile.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return -1; // Indicate error
    }

    std::string line;
    while (std::getline(myfile, line)) {
        std::stringstream ss(line);
        std::string token;
        int fieldIndex = 0;

        while (std::getline(ss, token, '|')) {
            if (fieldIndex == 1 && token == tickerSymbol) {
                // Skip to the price field
                std::getline(ss, token, '|'); // Skip the next field
                std::getline(ss, token, '|'); // This should be the price field

                // // Debugging: Print the extracted price field
                // std::cout << "Extracted Price Field: '" << token << "'" << std::endl;

                // Extract only the numeric part from the price field
                std::string numericPrice = extractNumericPart(token);

                // Clean and extract the price from the numeric part
                int price = extractCleanedPrice(numericPrice);

                if (price == -1) {
                    myfile.close();
                    return -1; // Indicate error
                }

                myfile.close();
                return price;
            }
            ++fieldIndex;
        }
    }

    myfile.close();
    std::cerr << "Ticker not found" << std::endl;
    return -1; // Indicate that the ticker was not found
}
