#ifndef ROBOTLOG_H
#define ROBOTLOG_H

#include "colors.h"
#include "main.h"
#include "pros/rtos.hpp"
#include <atomic>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <mutex>
#include <ostream>
#include <queue>
#include <regex>
#include <sstream>
#include <string>

#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

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
  std::optional<int> line;
  std::optional<Level> level;
  std::optional<std::string> message;

public:
  LogMessage(Level level, std::string message, std::string file, int line) {
    this->level = std::make_optional(level);
    this->message = std::make_optional(message);
    this->file = std::make_optional(file);
    this->line = std::make_optional(line);
  }

  std::string getFile() { return this->file.value_or(""); }
  std::string getLine() {
    std::ostringstream oss;
    oss << this->line.value_or(-1);
    return oss.str();
  }
  Level getLevel() { return this->level.value_or(ROBOTLOG::Level::INFO); }
  std::string getMessage() { return this->message.value_or(""); }
  std::string getLevelAsString() {
    switch (this->level.value()) {
    case DEBUG:
      return "DEBUG";
    case INFO:
      return "INFO";
    case WARN:
      return "WARN";
    case ERR:
      return "ERR";
    case DATA:
      return "";
    default:
      return std::to_string(this->level.value());
    }

    return "";
  }

  std::string getLevelAsStringBrackets() {
    switch (this->level.value()) {
    case DEBUG:
      return "[DEBUG]";
    case INFO:
      return "[INFO]";
    case WARN:
      return "[WARN]";
    case ERR:
      return "[ERR]";
    case DATA:
      return "";
    default:
      return "";
    }
    return "[" + std::to_string(this->level.value()) + "]";
  }

  std::string getLevelAsStringWithColors(
      std::string COLOR_ERR = ROBOTLOG::Colors::RED,
      std::string COLOR_WARN = ROBOTLOG::Colors::YELLOW,
      std::string COLOR_INFO = ROBOTLOG::Colors::GREEN,
      std::string COLOR_DEBUG = ROBOTLOG::Colors::MAGENTA) {
    switch (this->level.value()) {
    case DEBUG:
      return COLOR_DEBUG + "DEBUG" + Colors::RESET;
      break;
    case INFO:
      return COLOR_INFO + "INFO" + Colors::RESET;
      break;
    case WARN:
      return COLOR_WARN + "WARN" + Colors::RESET;
      break;
    case ERR:
      return COLOR_ERR + "ERR" + Colors::RESET;
      break;
    case DATA:
      return "";
      break;
    default:
      return Colors::WHITE + "[" + std::to_string(this->level.value()) + "]" +
             Colors::RESET;
    }
    return "";
  }

  std::string
  getLevelAsStringFull(std::string COLOR_ERR = ROBOTLOG::Colors::RED,
                       std::string COLOR_WARN = ROBOTLOG::Colors::YELLOW,
                       std::string COLOR_INFO = ROBOTLOG::Colors::HI_GREEN,
                       std::string COLOR_DEBUG = ROBOTLOG::Colors::MAGENTA) {
    switch (this->level.value()) {
    case DEBUG:
      return COLOR_DEBUG + "[DEBUG]" + Colors::RESET;
      break;
    case INFO:
      return COLOR_INFO + "[INFO]" + Colors::RESET;
      break;
    case WARN:
      return COLOR_WARN + "[WARN]" + Colors::RESET;
      break;
    case ERR:
      return COLOR_ERR + "[ERR]" + Colors::RESET;
      break;
    case DATA:
      return "";
      break;
    default:
      return Colors::WHITE + "[" + std::to_string(this->level.value()) + "]" +
             Colors::RESET;
    }

    return "";
  }

  std::string format(std::string formatString,
                     std::string COLOR_ERR = ROBOTLOG::Colors::RED,
                     std::string COLOR_WARN = ROBOTLOG::Colors::YELLOW,
                     std::string COLOR_INFO = ROBOTLOG::Colors::WHITE,
                     std::string COLOR_DEBUG = ROBOTLOG::Colors::MAGENTA) {
    /*
    ! Characteristics of a Format String
    * <LEVEL> - Log Level
    ? <FILE> - File the logger was (Called From or Made In, Haven't Decided Yet)
    TODO: Decide <FILE> Characteristics

    * <FUNC> - Function .log was called from
    ? <LINE> - Line .log was called from
    * <MESSAGE> - Message to Include with Log

    ! COLORS
    ! Colors can be set for the LEVEL with the following
    ? <LEVEL> - No Color
    ? <CLEVEL> - Colorizes only the level text, doesn't add []
    ? <BLEVEL> - Adds Brackets, doesn't add color
    ? <CBLEVEL> - Colorizes both level text, and adds brackets which are colored
    the same way
    */
    if (this->getLevel() == data ||
        this->getLevel() == DATA) { // If it's a DATA log, you don't want any
                                    // extra formatting to parse.
      return this->getMessage();
    }

    formatString =
        std::regex_replace(formatString, std::regex("<LEVEL>"), // Level Only
                           this->getLevelAsString());
    formatString = std::regex_replace(
        formatString, std::regex("<BLEVEL>"), // Brackets no Color
        this->getLevelAsStringBrackets());
    formatString = std::regex_replace(
        formatString, std::regex("<CLEVEL>"), // Color and Level
        this->getLevelAsStringWithColors(COLOR_ERR, COLOR_WARN, COLOR_INFO,
                                         COLOR_DEBUG));
    formatString = std::regex_replace(
        formatString, std::regex("<CBLEVEL>"), // Color and Brackets
        this->getLevelAsStringFull(COLOR_ERR, COLOR_WARN, COLOR_INFO,
                                   COLOR_DEBUG));

    formatString = std::regex_replace(formatString, std::regex("<FILE>"),
                                      this->getFile()); // File
    formatString = std::regex_replace(formatString, std::regex("<LINE>"),
                                      this->getLine()); // Line
    formatString =
        std::regex_replace(formatString, std::regex("<MESSAGE>"), // Message
                           this->getMessage());

    return formatString;
  }
};
class LOGGER {
private:
  pros::Task worker;
  std::queue<ROBOTLOG::LogMessage> logs;
  pros::Mutex updateLogFormat; // If the worker task is currently using a
                               // format, this will prevent updating that. This
                               // blocks user code, but so be it
  std::optional<std::string> logFormat =
      "<CBLEVEL> <FILE>:<LINE> - <MESSAGE>"; // replace <LEVEL> with
                                             // <CLEVEL> to colorize the
                                             // level, <BLEVEL> to put
                                             // brackets around the
                                             // level, and <CBLEVEL> to
                                             // place and colorize the
                                             // brackets around the
                                             // level. use normal
                                             // brackets surrounding
                                             // <CLEVEL> or <LEVEL> to
                                             // not colorize the brackets
  ROBOTLOG::Level consoleLogLevel = ROBOTLOG::Level::INFO;
  ROBOTLOG::Level fileLogLevel = ROBOTLOG::Level::DEBUG;
  std::optional<std::string> filePath;
  std::string COLOR_ERR = ROBOTLOG::Colors::RED;
  std::string COLOR_WARN = ROBOTLOG::Colors::YELLOW;
  std::string COLOR_INFO = ROBOTLOG::Colors::GREEN;
  std::string COLOR_DEBUG = ROBOTLOG::Colors::MAGENTA;
  pros::Mutex logmutex;

  static void taskEntry(void *param) {
    ROBOTLOG::LOGGER *logger = static_cast<ROBOTLOG::LOGGER *>(param);
    logger->workerTask();
  }

  void workerTask() {
    while (true) {
      if (this->logs.empty()) {
        pros::delay(10);
      }

      // std::ofstream file(this->filePath.value_or(""));
      constexpr static char maxlogwrites = 10;
      long availableLogs = this->logs.size();
      short loopindices = 0;
      if (maxlogwrites >= availableLogs) {
        loopindices = availableLogs;
      } else {
        loopindices = maxlogwrites;
      }

      for (int i = 0; i < loopindices; i++) {
        LogMessage msg = this->logs.front();
        std::string logmsg = msg.format(
            this->logFormat.value_or("<CBLEVEL> <FILE>:<LINE> - <MESSAGE>"));
        std::cout << logmsg << "\n";
        this->logs.pop();
      }
      pros::delay(5);
    }
  }

public:
  /**
   * @brief Construct a new LOGGER object
   *
   */
  LOGGER() : worker(&taskEntry, this, "(VexLog) LogProcessor") {
    this->addlog(Level::debug, "Initalized VexLog @ " +
                                   std::to_string(pros::millis()) + "ms");
  }

  /**
   * @brief Add a log message to the queue
   *
   * Converts the input T message to a string, then
   * Constructs a new LogMessage object with the level, message, file, and line.
   * Then it pushes the LogMessage object to the queue.
   *
   * @tparam T any type that can be converted to a string
   * @param level Log level, can be DEBUG, INFO, WARNING, ERROR, DATA
   * @param message The message to log
   * @param file a string name of the file the log was called from
   * @param line the line number the log was called from
   */
  template <typename T>
  void addlog(Level level, const T &message, std::string file = __FILENAME__,
              int line = __LINE__) {
    std::ostringstream messageAsString;
    messageAsString << message;
    LogMessage lmsg(level, messageAsString.str(), file, line);
    this->logs.push(lmsg);
  }

  /**
   * @brief Change the Format String
   *
   * Changes the format string used to format log messages. The format string
   * can include <LEVEL>, <CLEVEL>, <BLEVEL>, <CBLEVEL>, <FILE>, <LINE>, and
   * <MESSAGE>. See the README for more information.
   * @param formatString the new format string
   * @return true if the format string was successfully updated, false otherwise
   */
  bool setFormatString(std::string formatString) {
    if (this->updateLogFormat.take(5000) == true) {

      this->logFormat = std::make_optional(formatString);
      this->updateLogFormat.give();
      return true;
    }
    return false;
  }
};
} // namespace ROBOTLOG

/**
 * Macro to generate log entries
 *
 * Used to generate log entries with a specified log level. Automatically
 * include the filename and line number in the log message.
 *
 * @param level Log level
 * @param message Log message
 */
#define log(level, message)                                                    \
  LOGGER::addlog(level, message, __FILENAME__, __LINE__)

/**
 * @brief Macro to generate log entries with log level DEBUG
 *
 * Used when a debug message is to be logged. Automatically include the filename
 * and line number in the debug message.
 *
 * @param message Log message
 * @example debug("This is a debug message");
 */
#define info(message)                                                          \
  LOGGER::addlog(ROBOTLOG::Level::INFO, message, __FILENAME__, __LINE__)

/**
 * @brief Macro to generate log entries with log level INFO
 *
 * Used when an info message is to be logged. Automatically include the filename
 * and line number in the info message.
 *
 * @param message Log message
 * @example info("This is an info message");
 */
#define debug(message)                                                         \
  LOGGER::addlog(ROBOTLOG::Level::DEBUG, message, __FILENAME__, __LINE__)

/**
 * @brief Macro to generate log entries with log level WARNING
 *
 * Used when a warning is encountered. Automatically include the filename and
 * line number in the warning message.
 *
 * @param message Log message
 * @example warning("This is a warning message");
 */
#define warning(message)                                                       \
  LOGGER::addlog(ROBOTLOG::Level::WARNING, message, __FILENAME__, __LINE__)

/**
 * @brief Macro to generate log entries with log level ERROR
 *
 * Used when an error is encountered. Automatically include the filename and
 * line number in the error message.
 * @param message Log message
 * @example error("This is an error message");
 */
#define error(message)                                                         \
  LOGGER::addlog(ROBOTLOG::Level::ERROR, message, __FILENAME__, __LINE__)

/**
 * @brief Macro to generate log entries with log level DATA. Only prints the
 * message, no formatting.
 *
 * Used when raw data should be printed/saved. Does not add any extra formatting
 * to the message on the console or in the file.
 * @param message Log message
 * @example data("This is a data-only message");
 */
#define data(message)                                                          \
  LOGGER::addlog(ROBOTLOG::Level::DATA, message, __FILENAME__, __LINE__)

#endif