#include "util/RedisConfig.h"
#include <iostream>
#include <sstream>
#include <thread>

#include <async.h>
#include <adapters/libevent.h>
#include <poll.h>

int RedisConfig::redisPort=0;
std::string RedisConfig::redisHost = "";
std::string RedisConfig::redisPassword = "";

void RedisConfig::initialize(){
    redisPort = Resources::redisPort;
    redisHost = Resources::redisHost;
    redisPassword = Resources::redisPassword;
}

RedisConfig::RedisConfig(){
    appConnect();
    startEventLoop();
}

RedisConfig::~RedisConfig(){
    if (c) {
        redisAsyncDisconnect(c);
        redisAsyncFree(c);
        c = nullptr; // 포인터를 nullptr로 초기화
    }
    if (base) {
        event_base_free(base);
        base = nullptr; // 포인터를 nullptr로 초기화
    }
}

void RedisConfig::appConnect(){

    c = redisAsyncConnect(redisHost.c_str(), redisPort);
    
    if (c->err) {
        std::cout << "Error : " << c->errstr << std::endl;
        redisAsyncFree(c);
        c = NULL;
    }
    else{
        base = event_base_new();

        redisLibeventAttach(c,base);

        redisAsyncSetConnectCallback(c, appOnConnect);
        redisAsyncSetDisconnectCallback(c, appOnDisconnect);


        redisAsyncCommand(c,NULL, NULL, ("AUTH " + redisPassword).c_str());
    }
}

void RedisConfig::appOnConnect(const redisAsyncContext *c, int status)
{

    if (status == REDIS_OK) {
        std::cout<<"연결 connect성공"<<std::endl;
    } else {
        std::cout<<"연결 connect실패"<<std::endl;
    }
}

void RedisConfig::appOnDisconnect(const redisAsyncContext *c, int status)
{

    if (status == REDIS_OK) {
        std::cout<<"연결 dis connect성공"<<std::endl;
    } else {
        std::cout<<"연결 dis connect실패"<<std::endl;
    }
}

void RedisConfig::insert(const std::map<std::string, std::string>& data){

    std::ostringstream commandStream;

    commandStream << "MSET";
    for (const auto& pair : data) {
        commandStream << " " << pair.first << " " << pair.second;
    }

    std::string command = commandStream.str();
    
    // Redis에 명령어 전송
    redisAsyncCommand(c, NULL, NULL, command.c_str());
}

void RedisConfig::stop(){
    event_base_loopexit(base,nullptr);
}

void RedisConfig::startEventLoop() {
    std::thread([this]() {
        std::cout << "thread start" << std::endl;
        event_base_dispatch(base);
    }).detach();
}