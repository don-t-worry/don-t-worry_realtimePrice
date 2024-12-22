#include <hiredis.h>
#include <map>
#include <string>
#include "util/Resources.h"


class RedisConfig{
public:
    RedisConfig();
    ~RedisConfig();

    static void initialize();

    void appConnect();

    void insert(const std::map<std::string, std::string>& data);

    static void appOnConnect(const redisAsyncContext *c, int status);
    static void appOnDisconnect(const redisAsyncContext *c, int status);

    void stop(); 

    void startEventLoop();

    static void redisCallback(redisAsyncContext *c, void *reply, void *privdata);
    struct event_base *base;

private:
    redisAsyncContext *c;

    static int redisPort;
    static std::string redisHost;
    static std::string redisPassword;
    
    bool responseReceived = false;
    struct event *timeout_event = nullptr;
    static int index;
};