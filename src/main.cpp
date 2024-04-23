#include "main.h"

#include <chrono>

#include "robotlog/robotlog.h"

using namespace LOG;
using namespace std;
RobotLOG logger("logger.txt");
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
  static bool pressed = false;
  pressed = !pressed;
  if (pressed) {
    pros::lcd::set_text(2, "I was pressed!");
  } else {
    pros::lcd::clear_line(2);
  }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::lcd::initialize();
  pros::lcd::set_text(1, "Hello PROS User!");

  pros::lcd::register_btn1_cb(on_center_button);
  logger.log(INFO, std::string("Hello from " + std::string(__FILE__) +
                               " initalize()"));
  pros::delay(10);
  logger.log(INFO, std::string("Hello from " + std::string(__FILE__) +
                               " initalize() after 10s delay"));

  // test the logger
  logger.log(DEBUG, "Debug message");
  logger.log(INFO, "Info message");
  logger.log(WARNING, "Warning message");
  logger.log(ERROR, "Error message");

  // Passing in an int
  logger.log(INFO, 42);

  // Passing in a double
  logger.log(INFO, 3.14159);

  // Passing in a char
  logger.log(INFO, 'A');

  // Passing in a string
  logger.log(INFO, "Hello, World!");

  // Passing in a float
  logger.log(INFO, 3.14f);

  // Passing in a bool
  logger.log(INFO, true);

  // Passing in a long
  logger.log(INFO, 1234567890);

  // Passing in a long long
  logger.log(INFO, 1234567890123456789);

  // Passing in a long double
  logger.log(
      INFO,
      3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337L);

  // Passing in a std::string
  logger.log(INFO, std::string("Hello, World!"));

  // Testing no-level log
  logger.log(std::string("Hello from " + std::string(__FILE__) +
                         " initalize() but without a default level"));

  // Timing Test
  // Timing COUT
  for (int i = 0; i < 1000; i++) {
    auto start = chrono::high_resolution_clock::now();
    cout << "Hello, World!" << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    logger.log(INFO,
               "COUT Time: " + to_string(duration.count()) + " microseconds");
  }

  // Timing Logger
  for (int i = 0; i < 1000; i++) {
	auto start = chrono::high_resolution_clock::now();
	logger.log(INFO, "Hello, World!");
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	logger.log(INFO,
			   "Logger Time: " + to_string(duration.count()) + " microseconds");
  }
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  pros::Controller master(pros::E_CONTROLLER_MASTER);
  pros::Motor left_mtr(1);
  pros::Motor right_mtr(2);

  while (true) {
    pros::lcd::print(0, "%d %d %d",
                     (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
                     (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
                     (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
    int left = master.get_analog(ANALOG_LEFT_Y);
    int right = master.get_analog(ANALOG_RIGHT_Y);

    left_mtr = left;
    right_mtr = right;

    pros::delay(20);
  }
}
