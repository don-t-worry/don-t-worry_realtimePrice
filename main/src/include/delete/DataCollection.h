#ifndef DATA_COLLECTION_H
#define DATA_COLLECTION_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <optional>
#include <mutex>
#include <deque>
#include <memory>


// 특정 종목의 행사가 추출 
std::string extractNumericPart(const std::string& str);
int extractCleanedPrice(const std::string& numericPart);
int getPriceForTicker(const std::string& tickerSymbol, const std::string& filePath);


// 가격 정보 파싱  
double stockhokaStockfuts(const std::string& data);
double stockhokaStockoptn(const std::string& data, const char call_or_put);



// 가격 정보를 저장하는 클래스
class PriceData {
public:
    PriceData(const std::string& timestamp);
    PriceData() = default;

    // Getters
    double getCallPrice() const;    
    double getPutPrice() const;
    double getFuturesPrice() const;
    double getStrikePrice() const;
    std::string getTimestamp() const;

    // Setters
    void setCallPrice(double callPrice);
    void setPutPrice(double putPrice);
    void setFuturesPrice(double futuresPrice);
    void setStrikePrice(double strikePrice);

    // 모든 가격을 불러왔는지 확인
    bool allPricesSet() const;

    bool calculateArbitrage(const std::string& timestamp);
    void setPricesToNull(const std::string& timestamp);

private:
    double call_price;
    double put_price;
    double futures_price;
    double strike_price;
    std::string timestamp;
};


// pricedata 40개 있는 pool
class PriceDataPool {
public:
    PriceDataPool(size_t size);

    std::shared_ptr<PriceData> getPriceDataInstance();

private:
    std::vector<std::shared_ptr<PriceData>> pool;
    size_t currentIndex;
    std::mutex mtx;
};


#endif // DATA_COLLECTION_H
