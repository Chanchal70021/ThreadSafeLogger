// Logger.cpp
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <cstdio>

// ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {}

void Logger::init(const std::string& filename, LogLevel minLevel) {
    std::lock_guard<std::mutex> lock(mtx);
    logFileName = filename;
    currentLogLevel = minLevel;
    outFile.open(logFileName, std::ios::app);

    if (!outFile.is_open()) {
        std::cerr << "Failed to open log file: " << logFileName << std::endl;
        std::exit(EXIT_FAILURE);  // fail fast
    }

    worker = std::thread(&Logger::workerThread, this);
}
void Logger::shutdown() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (isShutdown) return;
        exitFlag = true;
        isShutdown = true;
    }
    cv.notify_one();
    if (worker.joinable()) worker.join();
    outFile.close();
}

Logger::~Logger() {
    shutdown();  // safe backup
}



void Logger::log(const std::string& message, LogLevel level) {
    if (isShutdown || level < currentLogLevel) return;

    std::string levelStr;
    switch (level) {
        case LogLevel::INFO: levelStr = "INFO"; break;
        case LogLevel::WARNING: levelStr = "WARNING"; break;
        case LogLevel::ERROR: levelStr = "ERROR"; break;
    }

    std::string formattedMessage = "[" + timestamp() + "] [" + levelStr + "] " + message;

    {
        std::lock_guard<std::mutex> lock(mtx);
        logQueue.push(formattedMessage);
    }
    cv.notify_one();
}

void Logger::workerThread() {
    std::unique_lock<std::mutex> lock(mtx);
    while (!exitFlag || !logQueue.empty()) {
        cv.wait(lock, [this] { return exitFlag || !logQueue.empty(); });
        while (!logQueue.empty()) {
            if (getCurrentFileSize() > MAX_FILE_SIZE) {
                rotateLogFile();
            }

            std::string msg = logQueue.front();
            logQueue.pop();

            // Write to file
            outFile << msg << std::endl;

            // Write to console with color
            if (msg.find("[ERROR]") != std::string::npos) {
                std::cout << RED << msg << RESET << std::endl;
            } else if (msg.find("[WARNING]") != std::string::npos) {
                std::cout << YELLOW << msg << RESET << std::endl;
            } else {
                std::cout << WHITE << msg << RESET << std::endl;
            }
        }
    }
}

std::size_t Logger::getCurrentFileSize() {
    outFile.flush();
    std::ifstream in(logFileName, std::ios::ate | std::ios::binary);
    return static_cast<std::size_t>(in.tellg());
}

void Logger::rotateLogFile() {
    outFile.close();
    std::string rotatedName = "logs_" + getTimestampForFilename() + ".txt";

    if (std::rename(logFileName.c_str(), rotatedName.c_str()) != 0) {
        std::cerr << "Failed to rename log file\n";
    }

    outFile.open(logFileName, std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Failed to reopen log file after rotation\n";
    }
}

std::string Logger::timestamp() {
    std::time_t now = std::time(nullptr);
    std::tm buf;
    localtime_s(&buf, &now);
    std::ostringstream ss;
    ss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Logger::getTimestampForFilename() {
    std::time_t now = std::time(nullptr);
    std::tm buf;
    localtime_s(&buf, &now);
    std::ostringstream ss;
    ss << std::put_time(&buf, "%Y-%m-%d_%H-%M-%S");
    return ss.str();
}
