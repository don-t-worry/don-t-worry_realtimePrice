#pragma once

#include "api/Request.h"
#include <asio.hpp>
#include <atomic>
#include <mutex>

class Runner{
public:
    ~Runner() = default;
    Runner();

    void run();

    void processStocks(Request& req);
    void allProcessStocks();

    void processTokens();

private:
    std::vector<Request> re;
    const int numRequests = 3;
    const std::vector<std::string>& stockCodes;
    asio::io_context ioContext;
    asio::steady_timer timer{ioContext};

    std::atomic<int> index=0;
    int size;
    std::mutex indexMutex;
    
};