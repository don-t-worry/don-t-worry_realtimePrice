#include "util/Run.h"
#include <iostream>
#include <thread>
#include <functional>
#include "util/StockCodesManager.h"
#include "util/SignalHandler.h"

Runner::Runner()
    : stockCodes(StockCodesManager::getStockCodes()), timer(ioContext) {  // 참조 초기화

    Resources::initialize();
    RedisConfig::initialize();

    // Request 배열 초기화
    re.reserve(numRequests);  // 메모리 예약
    for (int i = 0; i < numRequests; ++i) {
        re.emplace_back(i);
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    size = stockCodes.size();

    SignalHandler SignalHandler;

}


void Runner::run(){
    processTokens();
    allProcessStocks();
}

void Runner::processStocks(Request& req){

    const std::chrono::milliseconds interval(1000 / 20);
    while(SignalHandler::isRunning()){
        try{

            int localIndex;

            {
                std::lock_guard<std::mutex> lock(indexMutex);

                if (size == 0) {
                    std::cerr << "Error: Size is zero, cannot perform modulus operation." << std::endl;
                    return; // or throw an appropriate exception
                }

                index = (index + 30) % size;
                localIndex = index;

            }

            int start = (localIndex - 30 + size) % size;
            int end = localIndex;
        
            auto startTime = std::chrono::steady_clock::now();
            req.getStockPrice(start,end);
            auto endTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            if (elapsedTime < interval) {
                std::this_thread::sleep_for(interval - elapsedTime);
            }
        
        } catch (const std::exception& e) {
            std::cout << "Exception in processStocks: " << e.what() << std::endl;
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(50));
}


void Runner::allProcessStocks(){

    std::vector<std::thread> threads;

    // Request 인스턴스별로 스레드를 생성하여 처리
    try {
        for (int i = 0; i < numRequests; ++i) {
            threads.emplace_back(&Runner::processStocks, this, std::ref(re[i]));
        }

        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Exception in allProcessStocks: " << e.what() << std::endl;
    }
}


void Runner::processTokens(){
    for(int i=0; i<3; i++){
        re[i].getToken();
    }
}
