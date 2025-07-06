// main.cpp
#include "Logger.h"
#include <thread>
#include <chrono>

   int main() {
    Logger& logger = Logger::getInstance();
    logger.init("my_app_logs.txt", LogLevel::INFO);     // ← allows all levels

    logger.log("Main thread: Application started", LogLevel::INFO);
    logger.log("Disk space low", LogLevel::WARNING);
    logger.log("Fatal error occurred", LogLevel::ERROR);

    std::thread t1([&logger] {
        logger.log("Thread 1: Startup", LogLevel::INFO);
        logger.log("Thread 1: Something went wrong", LogLevel::ERROR);
    });

    std::thread t2([&logger] {
        logger.log("Thread 2: Caution message", LogLevel::WARNING);
        logger.log("Thread 2: Done", LogLevel::INFO);
    });

    t1.join();
    t2.join();

    logger.log("Main thread: Done", LogLevel::INFO);

    logger.shutdown();  // ✅ call this before main exits

    return 0;
}
