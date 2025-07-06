# 🧵 ThreadSafeLogger

A modern, lightweight, **thread-safe logging library** in C++17 with:

- ✅ Log level filtering (`INFO`, `WARNING`, `ERROR`)
- ✅ Colored console output
- ✅ Log file rotation based on file size
- ✅ Customizable log file name
- ✅ Graceful shutdown with background worker thread
- ✅ Designed for multithreaded C++ applications

---

## 📸 Demo Output

[2025-07-06 12:45:29] [INFO] Thread 1: Startup (white)
[2025-07-06 12:45:29] [WARNING] Disk space low (yellow)
[2025-07-06 12:45:29] [ERROR] Thread 1: Failure (red)

---

## 🚀 Features

| Feature                         | Description                                      |
|--------------------------------|--------------------------------------------------|
| 🧵 Thread-safe logging          | Safe from race conditions using `mutex + CV`     |
| 🔥 Colored console output       | Uses ANSI escape codes for `INFO`, `WARN`, `ERR` |
| 🪵 Log level filtering          | Set a minimum level to control verbosity         |
| 💾 File logging                 | All logs saved to a file (`logs.txt`)            |
| 📁 Log file rotation            | Rotates logs automatically if size > 5MB         |
| 🔧 Custom output file name      | Configure via `logger.init()`                    |
| ✅ Graceful shutdown            | Prevents logging after destruction               |

---

## 🛠️ Build & Run

### ✅ Prerequisites
- Windows with `g++` (MinGW or MSYS2)
- C++17 or higher

### 🔧 Compile
```bash
g++ -std=c++17 -o logger_app main.cpp src/Logger.cpp

▶️ Run
.\logger_app.exe

📂 Project Structure
ThreadSafeLogger/
├── src/
│   ├── Logger.cpp
│   └── Logger.h
├── main.cpp
├── .gitignore
├── README.md

📌 Example Usage

Logger& logger = Logger::getInstance();
logger.init("app_logs.txt", LogLevel::INFO);

logger.log("App started", LogLevel::INFO);
logger.log("Low disk warning", LogLevel::WARNING);
logger.log("Fatal crash", LogLevel::ERROR);

logger.shutdown();

👤 Author
Darelene
github.com/Chanchal70021a
