#include <util/SignalHandler.h>
#include <iostream>

std::atomic<bool> SignalHandler::running(true);

SignalHandler::SignalHandler(){
    signal(SIGTERM, SignalHandler::handleSignal);
}

void SignalHandler::handleSignal(int signal) {
    running = false; // 종료 플래그를 false로 설정
    std::cout << "signal detection..." << std::endl;
}

bool SignalHandler::isRunning() {
    return running;
}