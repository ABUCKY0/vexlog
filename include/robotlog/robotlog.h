#ifndef ROBOTLOG_H
#define ROBOTLOG_H

#include <atomic>   // For std::atomic
#include <fstream>  // For std::ofstream
#include <mutex>    // For std::mutex
#include <queue>    // For std::queue
#include <sstream>  // For std::ostringstream
#include <string>   // For std::string

#include "api.h"          // Include PROS API header
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
  std::ofstream logStream;
  std::atomic<bool> running{true};  // Atomic flag for thread-safe access
  pros::Task worker;

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
        logStream << toLog << std::endl;
        logStream.flush();  // Flush the stream to ensure that the message is written to the file
        // Print the message to console
        std::cout << toLog << std::endl;
      }
      pros::delay(10);  // Add a small delay to avoid tight looping
    }
  }

  static std::string levelToString(Level level) {
    switch (level) {
      case DEBUG:
        return "DEBUG";
      case INFO:
        return "INFO";
      case WARNING:
        return "WARN";
      case ERROR:
        return "ERR";
      default:
        return "UNKNOWN";
    }
  }

 public:
  // Constructor
  RobotLOG(const std::string &logFile)
      : logStream(logFile, std::ios::app), worker(&RobotLOG::taskEntry, this) {}

  // Destructor
  ~RobotLOG() {
    shutdown();  // Ensure proper shutdown when object is destroyed
  }

  // Log method
  template <typename T>
  void log(Level level, const T &message) {
    std::lock_guard<pros::Mutex> lock(queueMutex);
    std::ostringstream oss;
    oss << "[" << levelToString(level) << "] " << message;
    logQueue.push(oss.str());
  }

  // Shutdown method
  void shutdown() {
    running = false;  // Signal the background thread to stop
    if (worker.get_state() == pros::E_TASK_STATE_RUNNING) {
      worker.remove();  // Remove the task from the task scheduler
    }
    logStream.close();  // Close the log file
  }
};

};  // namespace LOG

#endif
