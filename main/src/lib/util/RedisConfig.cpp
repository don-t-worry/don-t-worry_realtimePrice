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
int RedisConfig::index=1;

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

    responseReceived = false;
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    // 타이머 콜백 설정
    timeout_event = event_new(base, -1, EV_TIMEOUT,
        [](evutil_socket_t fd, short event, void *arg) {

            RedisConfig* self = static_cast<RedisConfig*>(arg);
            if(self->responseReceived){
                return;
            }
            self->responseReceived = true;
            // std::cerr << "Timeout reached! Command " << self->index << " not completed within the time limit." << std::endl;
        },
        this
    );

    if (!event_pending(timeout_event, EV_TIMEOUT, NULL)) {
        event_add(timeout_event, &timeout);
    }  // 타이머 시작

    
    // Redis에 명령어 전송
    redisAsyncCommand(c, RedisConfig::redisCallback, this, command.c_str());
    index++;
}

void RedisConfig::redisCallback(redisAsyncContext *c, void *reply, void *privdata) {
    RedisConfig* self = static_cast<RedisConfig*>(privdata); // privdata로 RedisConfig 객체에 접근
    if(self->responseReceived){
        return;
    }
    self->responseReceived = true;
    
    // if (reply == nullptr) {
    //     std::cerr << "Command " << self->index <<" failed!" << std::endl;
    // } else {
    //     std::cout << "Command " << self->index <<" executed successfully!" << std::endl;
    // }
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