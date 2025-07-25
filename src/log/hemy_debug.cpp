//
// Created by zhaoj on 25-7-13.
//

#include "hemy_debug.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

Logger::Logger() : currentLevel(LogLevel::INFO), outputToConsole(true) {}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::initialize(const std::string& filename,
                       LogLevel level,
                       bool consoleOutput) {
    std::lock_guard<std::mutex> lock(logMutex);

    // 设置日志级别和输出选项
    currentLevel = level;
    outputToConsole = consoleOutput;

    // 关闭当前文件流（如果已打开）
    if (logFile.is_open()) {
        logFile.close();
    }

    // 确定日志文件名
    if (filename.empty()) {
        // 生成默认文件名: logs/YYYY-MM-DD.log
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << "logs/" << std::put_time(&tm, "%Y-%m-%d") << ".log";
        filePath = oss.str();
    } else {
        filePath = filename;
    }

    // 创建日志目录（如果需要）
    auto dir = fs::path(filePath).parent_path();
    if (!dir.empty() && !fs::exists(dir)) {
        fs::create_directories(dir);
    }

    // 打开文件（追加模式）
    logFile.open(filePath, std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("无法打开日志文件: " + filePath);
    }
}

void Logger::log(LogLevel level,
                const std::string& file,
                const std::string& func,
                int line,
                const std::string& message) {
    if (level < currentLevel) return;

    std::lock_guard<std::mutex> lock(logMutex);
    std::string logEntry = formatLogEntry(level, file, func, line, message);

    // 输出到文件
    if (logFile.is_open()) {
        logFile << logEntry << std::endl;
        logFile.flush();  // 确保立即写入
    }

    // 输出到控制台
    if (outputToConsole) {
        std::cout << logEntry << std::endl;
    }
}

std::string Logger::formatLogEntry(LogLevel level,
                                  const std::string& file,
                                  const std::string& func,
                                  int line,
                                  const std::string& message) {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);

    std::string filename = file;
    size_t pos = filename.find_last_of("/\\");
    if (pos != std::string::npos) {
        filename = filename.substr(pos + 1);
    }

    std::ostringstream oss;
    oss << "[" << levelToString(level) << "] "
        << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] "
        << "[" << filename << ": " << line << ": " << func << "] "
        << message;

    return oss.str();
}

std::string Logger::levelToString(LogLevel level) {
    switch(level) {
    case LogLevel::DEBUG:    return "DEBUG";
    case LogLevel::INFO:     return "INFO";
    case LogLevel::WARNING:  return "WARNING";
    case LogLevel::ERROR:    return "ERROR";
    case LogLevel::CRITICAL: return "CRITICAL";
    default:                 return "UNKNOWN";
    }
}