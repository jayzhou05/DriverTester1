#include "main.h"

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
	pros::Motor right_mtr(2,true);
	pros::Motor clamp(3);
	pros::Motor conveyor(4);
	okapi::MotorGroup leftWheels({5, 6});
  okapi::MotorGroup rightWheels({-7, -8});

	clamp.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
using namespace okapi;
	std::shared_ptr<okapi::ChassisController> drive =
		okapi::ChassisControllerBuilder()
				.withMotors(leftWheels, rightWheels)
				// Green gearset, 4 in wheel diam, 11.5 in wheel track
				.withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR}) //change wheelbase 11.5 pls
				.build();


	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);


	 // DRIVE TRAIN
	 drive->getModel()->arcade(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), //leftY
	 	                              master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)); //rightx

    // LIFT
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
			left_mtr.move_velocity(135);
			right_mtr.move_velocity(135);
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			left_mtr.move_velocity(-95);
			right_mtr.move_velocity(-95);
		}
		else{
			left_mtr.move_velocity(0);
			right_mtr.move_velocity(0);
		}

		//CLAMP
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) clamp.move_velocity(25);
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) clamp.move_velocity(-25);
		else clamp.move_velocity(0);

		//CONVEYOR
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) conveyor.move_velocity(100);
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) conveyor.move_velocity(-100);
		else conveyor.move_velocity(0);

		pros::delay(20);
	}
}
