#ifndef ROBOTLOG_H
#define ROBOTLOG_H

#include <atomic>   // For std::atomic
#include <fstream>  // For std::ofstream
#include <mutex>    // For std::mutex
#include <queue>    // For std::queue
#include <sstream>  // For std::ostringstream
#include <string>   // For std::string

#include "api.h"  // Include PROS API header
#include "colors.h"
#include "pros/rtos.hpp"  // Include PROS RTOS header for Mutex and Task
namespace LOG {

enum Level {
  DEBUG,
  INFO,
  WARNING,
  ERROR,
};

class RobotLOG {
 private:
  pros::Mutex queueMutex;
  std::queue<std::string> logQueue;
  std::string logFILE;
  std::atomic<bool> running{true};
  pros::Task worker;
  std::atomic<bool> logstofile{false};
  std::atomic<Level> logLevel{INFO};

  // Background task function for processing log messages
  static void taskEntry(void *param) {
    RobotLOG *logger = static_cast<RobotLOG *>(param);
    logger->processLogs();
  }

  // Actual task function to process log messages
  void processLogs() {
    while (running) {
      std::string toLog;
      {
        std::lock_guard<pros::Mutex> lock(queueMutex);
        if (!logQueue.empty()) {
          toLog = logQueue.front();
          logQueue.pop();
        }
      }
      if (!toLog.empty()) {
        if (logstofile) {
          FILE* outlogFILE = fopen((this->logFILE).c_str(), "a");
          if (outlogFILE != NULL) {
            fprintf(outlogFILE, "%s\n", toLog.c_str());
            fflush(outlogFILE);
            fclose(outlogFILE);
          } else {
            std::cerr << "Failed to open log file" << std::endl;
          }
        }
        // Print the message to console
        std::cout << toLog << std::endl;
      }
      pros::delay(10);  // Add a small delay to avoid tight looping
    }
  }

  static std::string levelToString(Level level) {
    switch (level) {
      case DEBUG:
        return Colors::GREEN + "DEBUG" + Colors::RESET;
      case INFO:
        return Colors::WHITE + "INFO" + Colors::RESET;
      case WARNING:
        return Colors::YELLOW + "WARN" + Colors::RESET;
      case ERROR:
        return Colors::RED + "ERR" + Colors::RESET;
      default:
        return "UNKNOWN";
    }
  }

 public:
  // No-arg constructor
  RobotLOG() : worker(&RobotLOG::taskEntry, this) { logstofile = false; }

  RobotLOG(const std::string &logFileLoc) : worker(&RobotLOG::taskEntry, this) {
    logstofile = true;
    this->logFILE = "/usd/" + logFileLoc;
  }

  // Destructor
  ~RobotLOG() {
    shutdown();  // Ensure proper shutdown when object is destroyed
  }

  void disableFileLogging() { logstofile = false; }

  void enableFileLogging() { logstofile = true; }

  // Log method
  template <typename T>
  void log(Level level, const T &message) {
    std::lock_guard<pros::Mutex> lock(queueMutex);
    std::ostringstream oss;
    oss << pros::micros() << "μs - [" << levelToString(level) << "] "
        << message;
    logQueue.push(oss.str());
  }

  // Log method
  template <typename T>
  void log(const T &message) {
    std::lock_guard<pros::Mutex> lock(queueMutex);
    std::ostringstream oss;
    oss << pros::micros() << "μs - [" << levelToString(INFO) << "] " << message;
    logQueue.push(oss.str());
  }

  // Shutdown method
  void shutdown() {
    running = false;  // Signal the background thread to stop
    if (worker.get_state() == pros::E_TASK_STATE_RUNNING) {
      worker.remove();  // Remove the task from the task scheduler
    }
  }
};

};  // namespace LOG

#endif
