#pragma once

#include <string>

// Authorization 구조체 선언
struct Authorization {
    std::string appkey;
    std::string secretkey;
};

class Resources{
public:
    static const int accountCount=3;
    static Authorization authorization[accountCount];
    static int redisPort;
    static std::string redisHost;
    static std::string redisPassword;

    static void initialize();
};

