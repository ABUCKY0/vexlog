#ifndef ROBOTLOG_H
#define ROBOTLOG_H

#include "main.h"
#include "pros/rtos.hpp"
#include <atomic>
#include <fstream>
#include <fstream>
#include <queue>
#include <string>
#include <sstream>
namespace ROBOTLOG {
enum Level {
  DEBUG = 0,
  debug = 0,
  INFO = 1,
  info = 1,
  WARN = 2,
  WARNING = 2,
  warn = 2,
  warning = 2,
  ERR = 3,
  ERROR = 3,
  err = 3,
  error = 3,

  // data is used to override the string format, and instead writes the direct
  // data,
  DATA = 4,
  data = 4,
};

struct LogMessage {
  /*
  A Message Can Include the following:

  file the file where a log entry was passed from
  func the function a log entry was passed from
  line the line a log was called from
  level the level a log was set to, debug, info, warning, error, data
  message the message of a log
  */

private:
  std::optional<std::string> file;
  std::optional<std::string> func;
  std::optional<int> line;
  std::optional<Level> level;
  std::optional<std::string> message;

public:
  LogMessage(Level level, std::string message, std::string file,
             std::string func, int line) {
    this->level = std::make_optional(level);
    this->message = std::make_optional(message);
    this->file = std::make_optional(file);
    this->func = std::make_optional(func);
    this->line = std::make_optional(line);
  }

  std::string getFile() {
    return this->file.value_or("");
  }
  std::string getFunc() {
    return this->func.value_or("");
  }
  std::string getLine() {
    std::ostringstream oss;
    oss << this->line.value_or(-1);
    return oss.str();
  }
  Level getLevel() {
    return this->level.value_or(ROBOTLOG::Level::INFO);
  }
  std::string getMessage() {
    return this->message.value_or("");
  }
};
class LOGGER {
private:
  // thread safe queue
  pros::Task worker;
  std::queue<ROBOTLOG::LogMessage> logs;
  pros::Mutex updateLogFormat; // If the worker task is currently using a
                               // format, this will prevent updating that. This
                               // blocks user code, but so be it
  std::optional<std::string> logFormat = "[<LEVEL>] <FILE>:<FUNC>:<LINE> - <MESSAGE>";
  ROBOTLOG::Level consoleLogLevel = ROBOTLOG::Level::INFO;
  ROBOTLOG::Level fileLogLevel = ROBOTLOG::Level::DEBUG;
  
  std::optional<std::string> filePath;


  static void taskEntry(void *param) {
    ROBOTLOG::LOGGER *logger = static_cast<ROBOTLOG::LOGGER *>(param);
    logger->workerTask(); 
  }

  void workerTask() {
    while (true) {
      if (this->logs.empty()) {
        pros::delay(2);
        continue;
      }

      LogMessage msg = this->logs.front();

      std::optional<std::fstream> file;
      std::atomic<bool> fileIsWritable;
      if (this->filePath.has_value()) {
        // Instantiate File
        file = std::fstream(this->filePath.value());
      }



      // Close File
      file.value().close();
    }
  }

public:
  LOGGER() : worker(&taskEntry, this) {

  }
};
} // namespace ROBOTLOG

#endif