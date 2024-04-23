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
  DEBUG,    // Detailed information, typically of interest only when diagnosing
            // problems.
  INFO,     // Program Information.
  WARNING,  // Used in User Programs for if something unexpected but recoverable
            // happened
  ERROR,  // Useful for if the user program cannot recover, such as a sensor not
          // being found
};

class RobotLOG {
 private:
  // pros::Mutex queueMutex;
  // std::queue<std::string> logQueue;
  // std::string logFILE;
  // std::atomic<bool> running{true};
  // pros::Task worker;
  // std::atomic<bool> logstofile{false};
  // std::atomic<Level> logLevel{INFO};

  std::atomic<bool> isTakingNewLogs{
      true};  // replaces running. I still want to finish flusing out the logs
              // before stopping
  std::atomic<bool> logstofile{false};
  std::string logFilePath;
  std::ofstream logFile;
  std::queue<std::string> logQueue;  // queue of logs to be written to file
  pros::Mutex logMutex;              // mutex to protect the queue
  pros::Task worker;                 // task to write logs to file
  /*
   * Logging Level is as Follows:
   * DEBUG: Detailed information, typically of interest only when diagnosing
   * problems. Level 0 INFO: Program Information. Level 1 WARN: Used in User
   * Programs for if something unexpected but recoverable happened ERROR: Useful
   * for if the user program cannot recover, such as a sensor not being found
   */
  std::atomic<Level> loggingLevel{INFO};  // minimum level to log

  /**
   * @brief Entry point for the background task that writes logs to the file
   * @param param Pointer to the RobotLOG object
  */
  static void taskEntry(void *param) {
    RobotLOG *logger = static_cast<RobotLOG *>(param);
    logger->processLogs();
  }

  /**
   * @brief Processes the logs in the queue and writes them to the file
  */
  void processLogs() {
    
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
  void log(Level level, const T &message) {}

  // Log method
  template <typename T>
  void log(const T &message) {}

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
