#ifndef COLORS_H
#define COLORS_H

#include <string>
namespace ROBOTLOG {
class Colors {
 public:
  // Regular Colors
  static const std::string BLACK;
  static const std::string RED;
  static const std::string GREEN;
  static const std::string YELLOW;
  static const std::string BLUE;
  static const std::string MAGENTA;
  static const std::string CYAN;
  static const std::string WHITE;
  // Bold Colors
  static const std::string BOLD_BLACK;
  static const std::string BOLD_RED;
  static const std::string BOLD_GREEN;
  static const std::string BOLD_YELLOW;
  static const std::string BOLD_BLUE;
  static const std::string BOLD_MAGENTA;
  static const std::string BOLD_CYAN;
  static const std::string BOLD_WHITE;
  // Underline
  static const std::string UBLACK;
  static const std::string URED;
  static const std::string UGREEN;
  static const std::string UYELLOW;
  static const std::string UBLUE;
  static const std::string UMAGENTA;
  static const std::string UCYAN;
  static const std::string UWHITE;
  // Background
  static const std::string BG_BLACK;
  static const std::string BG_RED;
  static const std::string BG_GREEN;
  static const std::string BG_YELLOW;
  static const std::string BG_BLUE;
  static const std::string BG_MAGENTA;
  static const std::string BG_CYAN;
  static const std::string BG_WHITE;
  // High Intensity
  static const std::string HI_BLACK;
  static const std::string HI_RED;
  static const std::string HI_GREEN;
  static const std::string HI_YELLOW;
  static const std::string HI_BLUE;
  static const std::string HI_MAGENTA;
  static const std::string HI_CYAN;
  static const std::string HI_WHITE;
  // Bold High Intensity
  static const std::string BOLD_HI_BLACK;
  static const std::string BOLD_HI_RED;
  static const std::string BOLD_HI_GREEN;
  static const std::string BOLD_HI_YELLOW;
  static const std::string BOLD_HI_BLUE;
  static const std::string BOLD_HI_MAGENTA;
  static const std::string BOLD_HI_CYAN;
  static const std::string BOLD_HI_WHITE;
  // High Intensity backgrounds
  static const std::string HI_BG_BLACK;
  static const std::string HI_BG_RED;
  static const std::string HI_BG_GREEN;
  static const std::string HI_BG_YELLOW;
  static const std::string HI_BG_BLUE;
  static const std::string HI_BG_MAGENTA;
  static const std::string HI_BG_CYAN;
  static const std::string HI_BG_WHITE;
  // Reset
  static const std::string RESET;

  static std::string colorize(std::string color, std::string text) {
    return color + text + RESET;
  }
  static std::string CUSTOMRGB(short red, short green, short blue) {
    return "\033[38;2;" + std::to_string(red) + ";" + std::to_string(green) +
           ";" + std::to_string(blue) + "m";
  }
};

const std::string ROBOTLOG::Colors::RESET = "\033[0;0m";
const std::string ROBOTLOG::Colors::BLACK = "\033[0;30m";
const std::string ROBOTLOG::Colors::RED = "\033[0;31m";
const std::string ROBOTLOG::Colors::GREEN = "\033[0;32m";
const std::string ROBOTLOG::Colors::YELLOW = "\033[0;33m";
const std::string ROBOTLOG::Colors::BLUE = "\033[0;34m";
const std::string ROBOTLOG::Colors::MAGENTA = "\033[0;35m";
const std::string ROBOTLOG::Colors::CYAN = "\033[0;36m";
const std::string ROBOTLOG::Colors::WHITE = "\033[0;37m";
const std::string ROBOTLOG::Colors::BOLD_BLACK = "\033[1m\033[30m";
const std::string ROBOTLOG::Colors::BOLD_RED = "\033[1m\033[31m";
const std::string ROBOTLOG::Colors::BOLD_GREEN = "\033[1m\033[32m";
const std::string ROBOTLOG::Colors::BOLD_YELLOW = "\033[1m\033[33m";
const std::string ROBOTLOG::Colors::BOLD_BLUE = "\033[1m\033[34m";
const std::string ROBOTLOG::Colors::BOLD_MAGENTA = "\033[1m\033[35m";
const std::string ROBOTLOG::Colors::BOLD_CYAN = "\033[1m\033[36m";
const std::string ROBOTLOG::Colors::BOLD_WHITE = "\033[1m\033[37m";
const std::string ROBOTLOG::Colors::UBLACK = "\033[4m\033[30m";
const std::string ROBOTLOG::Colors::URED = "\033[4m\033[31m";
const std::string ROBOTLOG::Colors::UGREEN = "\033[4m\033[32m";
const std::string ROBOTLOG::Colors::UYELLOW = "\033[4m\033[33m";
const std::string ROBOTLOG::Colors::UBLUE = "\033[4m\033[34m";
const std::string ROBOTLOG::Colors::UMAGENTA = "\033[4m\033[35m";
const std::string ROBOTLOG::Colors::UCYAN = "\033[4m\033[36m";
const std::string ROBOTLOG::Colors::UWHITE = "\033[4m\033[37m";
const std::string ROBOTLOG::Colors::BG_BLACK = "\033[40m";
const std::string ROBOTLOG::Colors::BG_RED = "\033[41m";
const std::string ROBOTLOG::Colors::BG_GREEN = "\033[42m";
const std::string ROBOTLOG::Colors::BG_YELLOW = "\033[43m";
const std::string ROBOTLOG::Colors::BG_BLUE = "\033[44m";
const std::string ROBOTLOG::Colors::BG_MAGENTA = "\033[45m";
const std::string ROBOTLOG::Colors::BG_CYAN = "\033[46m";
const std::string ROBOTLOG::Colors::BG_WHITE = "\033[47m";
const std::string ROBOTLOG::Colors::HI_BLACK = "\033[0;90m";
const std::string ROBOTLOG::Colors::HI_RED = "\033[0;91m";
const std::string ROBOTLOG::Colors::HI_GREEN = "\033[0;92m";
const std::string ROBOTLOG::Colors::HI_YELLOW = "\033[0;93m";
const std::string ROBOTLOG::Colors::HI_BLUE = "\033[0;94m";
const std::string ROBOTLOG::Colors::HI_MAGENTA = "\033[0;95m";
const std::string ROBOTLOG::Colors::HI_CYAN = "\033[0;96m";
const std::string ROBOTLOG::Colors::HI_WHITE = "\033[0;97m";
const std::string ROBOTLOG::Colors::BOLD_HI_BLACK = "\033[1;90m";
const std::string ROBOTLOG::Colors::BOLD_HI_RED = "\033[1;91m";
const std::string ROBOTLOG::Colors::BOLD_HI_GREEN = "\033[1;92m";
const std::string ROBOTLOG::Colors::BOLD_HI_YELLOW = "\033[1;93m";
const std::string ROBOTLOG::Colors::BOLD_HI_BLUE = "\033[1;94m";
const std::string ROBOTLOG::Colors::BOLD_HI_MAGENTA = "\033[1;95m";
const std::string ROBOTLOG::Colors::BOLD_HI_CYAN = "\033[1;96m";
const std::string ROBOTLOG::Colors::BOLD_HI_WHITE = "\033[1;97m";
const std::string ROBOTLOG::Colors::HI_BG_BLACK = "\033[0;100m";
const std::string ROBOTLOG::Colors::HI_BG_RED = "\033[0;101m";
const std::string ROBOTLOG::Colors::HI_BG_GREEN = "\033[0;102m";
const std::string ROBOTLOG::Colors::HI_BG_YELLOW = "\033[0;103m";
const std::string ROBOTLOG::Colors::HI_BG_BLUE = "\033[0;104m";
const std::string ROBOTLOG::Colors::HI_BG_MAGENTA = "\033[0;105m";
const std::string ROBOTLOG::Colors::HI_BG_CYAN = "\033[0;106m";
const std::string ROBOTLOG::Colors::HI_BG_WHITE = "\033[0;107m";

}  // namespace LOG
#endif