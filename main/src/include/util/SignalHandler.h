#pragma once

#include <csignal>
#include <atomic>

class SignalHandler {
public:
    SignalHandler();
    static void handleSignal(int signal);
    static bool isRunning();

private:
    static std::atomic<bool> running;
};