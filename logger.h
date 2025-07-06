// Logger.h
#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static Logger& getInstance();

    void init(const std::string& filename = "logs.txt", LogLevel minLevel = LogLevel::INFO);
    void log(const std::string& message, LogLevel level = LogLevel::INFO);
    void shutdown();

    ~Logger();

private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void workerThread();
    void rotateLogFile();
    std::string timestamp();
    std::string getTimestampForFilename();
    std::size_t getCurrentFileSize();
    std::string colorForLevel(LogLevel level);

    static constexpr std::size_t MAX_FILE_SIZE = 5 * 1024 * 1024; // 5MB

    std::mutex mtx;
    std::condition_variable cv;
    std::queue<std::string> logQueue;
    std::ofstream outFile;
    std::thread worker;
    bool exitFlag = false;
    bool isShutdown = false;

    LogLevel currentLogLevel = LogLevel::INFO;
    std::string logFileName = "logs.txt";
};
