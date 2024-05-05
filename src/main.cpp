#include "main.h"

#include <chrono>

#include "robotlog/robotlog.h"

using namespace std;

ROBOTLOG::LOGGER logger;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {}
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
  //     long int l= 0;
  //     while (true) {
  //         log.info(l);
  //         l++;
  //         pros::delay(1);
  //     }
  double total = 0;
  for (int i = 0; i < 1000; i++) {
    auto start = chrono::high_resolution_clock::now();
    logger.info(i);
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    total += elapsed.count();
    pros::delay(10);
  }
  logger.debug("For each time a log function is called, it took " + std::to_string(total*1000) + " microseconds on average before finishing");
  logger.log(ROBOTLOG::Level(7), "This is a log message with a custom level of 7");
while (true) {
    pros::delay(10000);
}
}