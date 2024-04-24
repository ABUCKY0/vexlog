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

using namespace std;

namespace LOG {


// Enum representing log levels
enum Level {
  DEBUG,    // Detailed information, typically of interest only when diagnosing
            // problems.
  INFO,     // Program Information.
  WARNING,  // Used in User Programs for if something unexpected but recoverable
            // happened.
  ERROR,  // Useful for if the user program cannot recover, such as a sensor not
          // being found.
};

// Struct representing a log entry
struct logEntry {
  string file;  // Name of the file where the log is generated (__FILE__)
  uint64_t microseconds;  // Timestamp in microseconds (Converted to Seconds in
                          // getSeconds())
  Level level;            // Log level (DEBUG, INFO, WARNING, ERROR)
  string message;         // Log message
  string func;            // Function where the log is generated (__FUNCTION__)
  int line;               // Line number where the log is generated

  /**
   * Constructor for log entry
   * @param file File where the log is generated
   * @param level Log level
   * @param message Log message
   * @param func Function where the log is generated
   * @param line Line number where the log is generated
   * @example logEntry entry(__FILE__, Level::INFO, "Hello, World!", __FUNCTION__,
   * __LINE__);
   * @note It's reccomented to use @see log() macro to generate log entries
  */
  logEntry(const std::string& file, Level level, const std::string& message,
           const std::string& func, int line) {
    this->file = file;
    this->microseconds = pros::micros();
    this->level = level;
    this->message = message;
    this->func = func;
    this->line = line;
  }

  /**
   * Get seconds from microseconds
   * @return Seconds from the timestamp
  */
  int getSeconds() { return this->microseconds / 1000000; }

  /**
   * Get time in the format of MM:SS.mmm
   * @return Time string in the format of MM:SS.mmm
  */
  string getTime() {
    int total_seconds = this->microseconds / 1000000;
    int minutes = total_seconds / 60;
    int seconds = total_seconds % 60;
    int milliseconds = (this->microseconds / 1000) % 1000;

    string time = (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
                  (seconds < 10 ? "0" : "") + std::to_string(seconds) + "." +
                  (milliseconds < 100 ? "0" : "") +
                  (milliseconds < 10 ? "0" : "") + std::to_string(milliseconds);

    return time;
  }

  /**
   * Colorize the log level
   * @param level Log level
   * @return Colorized log level
  */
  std::string colorizeLevel(Level level) {
    switch (level) {
      case DEBUG:
        return Colors::GREEN + "[DEBUG]" + Colors::RESET;
      case INFO:
        return Colors::WHITE + "[INFO]" + Colors::RESET;
      case WARNING:
        return Colors::YELLOW + "[WARN]" + Colors::RESET;
      case ERROR:
        return Colors::RED + "[ERR]" + Colors::RESET;
      default:
        return "UNKNOWN";
    }
  }
};

/// @brief Class for logging
class RobotLOG {
 private:
  std::atomic<bool> isTakingNewLogs{
      true};  // Flag indicating whether the logger is taking new logs
  std::atomic<bool> logstofile{
      false};  // Flag indicating whether logs should be written to file
  std::string logFilePath;        // Path to the log file
  std::ofstream logFile;          // Output stream for writing logs to file
  std::queue<logEntry> logQueue;  // Queue of logs to be written to file
  pros::Mutex logMutex;           // Mutex to protect the log queue
  pros::Task worker;              // Task to write logs to file asynchronously
  std::string format = "%t - %l (%y-%b): %m";  // Log message format

  /**
   * Static method to be called by the task scheduler to write logs to file
   * @param param Pointer to the logger object
   * @note This method is called by the task scheduler to write logs to file
  */
  static void taskEntry(void* param) {
    RobotLOG* logger = static_cast<RobotLOG*>(param);
    logger->processLogs();  // Process logs in the queue and write them to the
                            // file
  }
  
  /**
   * Processes Queued Logs
   * @note This method processes logs in the queue and writes them to the file
  */
  void processLogs() {
    cout << ("Logger started\n");  // Log a message indicating that the logger
                                   // has started
    while (true) {
      if (this->logQueue.empty()) {
        pros::delay(30);  // Delay for 5 milliseconds if the queue is empty
        continue;
      }
      logMutex.take(0);  // Take the mutex to access the log queue
      logEntry message =
          logQueue.front();  // Get the frontmost log entry from the queue
      logQueue.pop();        // Remove the frontmost log entry from the queue

      // Format the log message
      std::string formattedMessage = format;
      std::string time = message.getTime();
      std::string level = message.colorizeLevel(message.level);
      std::string file = message.file;
      std::string func = message.func;
      std::string log = message.message;
      std::string line = std::to_string(message.line);

      // Replace placeholders in the log message with actual values
      std::size_t pos;
      pos = formattedMessage.find("%t");
      if (pos != std::string::npos) {
        formattedMessage.replace(pos, 2, time);
      }
      pos = formattedMessage.find("%l");
      if (pos != std::string::npos) {
        formattedMessage.replace(pos, 2, level);
      }
      pos = formattedMessage.find("%y");
      if (pos != std::string::npos) {
        formattedMessage.replace(pos, 2, file);
      }
      pos = formattedMessage.find("%b");
      if (pos != std::string::npos) {
        formattedMessage.replace(pos, 2, func);
      }
      pos = formattedMessage.find("%m");
      if (pos != std::string::npos) {
        formattedMessage.replace(pos, 2, log);
      }
      pos = formattedMessage.find("%r");
      if (pos != std::string::npos) {
        formattedMessage.replace(pos, 2, line);
      }
      // if (logstofile && logFile.is_open()) {
      //   logFile << formattedMessage << std::endl;
      // }
      cout << formattedMessage
           << std::endl;  // Print the log message to the console
      logMutex.give();
      pros::delay(15);  // Delay for 15 milliseconds
    }
  }

 public:
  /**
   * Constructor for the logger
  */
  RobotLOG() : worker(&RobotLOG::taskEntry, this) { logstofile = false; }

 /**
  * Constructor for the logger
  * @param logFileLoc Path to the log file
 */
  RobotLOG(const std::string& logFileLoc) : worker(&RobotLOG::taskEntry, this) {
    logstofile = true;
    this->logFilePath = "/usd/" + logFileLoc;
    this->logQueue = std::queue<logEntry>();
  }

  /**
   * Destructor for the logger
  */
  ~RobotLOG() {
    shutdown();  // Ensure proper shutdown when object is destroyed
  }

  /**
   * Disable file logging
  */
  void disableFileLogging() { logstofile = false; }

  
  /**
   * Enable file logging
  */
  void enableFileLogging() { logstofile = true; }

  /**
   * Add a log entry to the queue
   * @param level Log level
   * @tparam message Log message
   * @param file File where the log is generated
   * @param function Function where the log is generated
   * @param line Line number where the log is generated
   * @example addlog(LOG::INFO, "Hello, World!", __FILE__, __FUNCTION__, __LINE__);
   */
  template <typename T>
  void addlog(LOG::Level level, const T& message, const std::string& file,
              const std::string& function, int line) {
    std::ostringstream msgStream;
    msgStream << message;               // Convert any type T to string
    std::string msg = msgStream.str();  // Get the string from the stream

    this->logQueue.push(logEntry(file, level, msg, function, line));
    cout << "Added log to queue" << endl;
  }

  /**
   * Shutdown the logger
  */
  void shutdown() {
    if (worker.get_state() == pros::E_TASK_STATE_RUNNING) {
      worker.remove();  // Remove the task from the task scheduler
    }
  }
};

};  // namespace LOG

/**
 * Macro to generate log entries
 * @param level Log level
 * @param message Log message
*/
#define log(level, message) \
  RobotLOG::addlog(level, message, __FILE__, __FUNCTION__, __LINE__)

/**
 * Macro to generate log entries with log level DEBUG
 * @param message Log message
 * @example debug("This is a debug message");
 */
#define info(message) \
  RobotLOG::addlog(LOG::INFO, message, __FILE__, __FUNCTION__, __LINE__)

/**
 * Macro to generate log entries with log level INFO
 * @param message Log message
 * @example info("This is an info message");
 */
#define debug(message) \
  RobotLOG::addlog(LOG::DEBUG, message, __FILE__, __FUNCTION__, __LINE__)

/**
 * Macro to generate log entries with log level WARNING
 * @param message Log message
 * @example warning("This is a warning message");
 */
#define warning(message) \
  RobotLOG::addlog(LOG::WARNING, message, __FILE__, __FUNCTION__, __LINE__)

/**
 * Macro to generate log entries with log level ERROR
 * @param message Log message
 * @example error("This is an error message");
 */
#define error(message) \
  RobotLOG::addlog(LOG::ERROR, message, __FILE__, __FUNCTION__, __LINE__)

#endif
