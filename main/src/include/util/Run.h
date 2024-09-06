#pragma once

#include "Request.h"
#include <thread>

class Runner{
public:
    virtual ~Runner();
    Runner();

    run();

private:
    Request[] request;
}