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
void autonomous() {
	/*
	okapi::MotorGroup leftWheels({5, 6});
  okapi::MotorGroup rightWheels({-7, -8});

	using namespace okapi;
		std::shared_ptr<OdomChassisController> chassis =
		  ChassisControllerBuilder()
		    .withMotors(leftWheels, rightWheels)
		    // green gearset, 4 inch wheel diameter, 11.5 inch wheel track
		    .withDimensions(AbstractMotor::gearset::green, {{4_in, 11.125_in}, imev5GreenTPR})
		    .withOdometry()
		    .buildOdometry();
		std::shared_ptr<AsyncPositionController<double, double>> back =
		  AsyncPosControllerBuilder()
				.withMotor(3)
				.withGains({0, 0, 0})
				.build();
		std::shared_ptr<AsyncPositionController<double, double>> lift =
		  AsyncPosControllerBuilder()
				.withMotor({-1,2})
				.withGains({0, 0, 0})
				.build();

		chassis->setState({0_in, 0_in, 0_deg});
		chassis->driveToPoint({1_ft, 0_ft});
		back->setTarget(-180);
		lift->waitUntilSettled();
		chassis->driveToPoint({0_ft, 0_ft});
		*/



}

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

	//PORTS
	pros::Controller master(pros::E_CONTROLLER_MASTER);


	pros::Motor liftL(14, true);
	pros::Motor liftR(18, false);
	pros::Motor back(15, false);
	pros::Motor hook(4, false);

	/*
	okapi::MotorGroup leftWheels({5, 14});
  okapi::MotorGroup rightWheels({-19, -16});
 */

	pros::Motor left_front (15);
	pros::Motor left_back (19);
	pros::Motor right_front (16,true);
  pros::Motor right_back (17, true);

	//BRAKES
	back.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	liftL.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	liftR.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	hook.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

	left_front.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	left_back.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	right_front.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	right_back.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

	//CONTROLS
	pros::controller_digital_e_t buttonLiftUp = pros::E_CONTROLLER_DIGITAL_L1;
	pros::controller_digital_e_t buttonLiftDown = pros::E_CONTROLLER_DIGITAL_L2;
	pros::controller_digital_e_t buttonBackUp = pros::E_CONTROLLER_DIGITAL_R1;
	pros::controller_digital_e_t buttonBackDown = pros::E_CONTROLLER_DIGITAL_R2;
	pros::controller_digital_e_t buttonHookOut = pros::E_CONTROLLER_DIGITAL_UP;
	pros::controller_digital_e_t buttonHookIn = pros::E_CONTROLLER_DIGITAL_DOWN;



/*
	std::shared_ptr<okapi::ChassisController> drive =
		okapi::ChassisControllerBuilder()
				.withMotors(leftWheels, rightWheels)
				// Green gearset, 4 in wheel diam, 11.5 in wheel track
				.withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR}) //change wheelbase 11.5 pls
				.build();

*/
	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

	 /*
	 // DRIVE TRAIN
	 drive->getModel()->arcade(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), //leftY
	 	                              master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)); //rightx
		*/


		float LEFTY= master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		float RIGHTX= master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		//Linear multiply so forward/backward maxes at 100
		LEFTY = LEFTY * 1.27;
		if(LEFTY>127){
			LEFTY = 127;
		}

		//Linear multiply for turns (no longer used)
		//RIGHTX = RIGHTX * 5/6;

		//Deadzones
		if( (LEFTY < 5) && (LEFTY > -5) ){
			LEFTY=0;
		}
		if( (RIGHTX < 5) && (RIGHTX > -5) ){
			RIGHTX=0;
		}

		double turnConst = 1.5;       // lower = linear, higher = cubic; cannot be 0
		double turnInput = master.get_analog(ANALOG_RIGHT_X) * (double)105.0 / 127.0;
		double turnSpeed = turnConst * (pow(turnInput, 3) / 10000 + turnInput / turnConst) / (turnConst + 1);

		float frontLeftMod = (LEFTY + turnSpeed); //front left
		float frontRightMod = (LEFTY - turnSpeed); //front right
		float backLeftMod = (LEFTY + turnSpeed); //back left
		float backRightMod = (LEFTY - turnSpeed); //back right

		left_front.move_velocity(frontLeftMod*(200/127));
    right_front.move_velocity(frontRightMod*(200/127));
		left_back.move_velocity(backLeftMod*(200/127));
    right_back.move_velocity(backRightMod*(200/127));





    // LIFT
		if(master.get_digital(buttonLiftUp)){
			liftL.move_velocity(100);
			liftR.move_velocity(100);
		}
		else if(master.get_digital(buttonLiftDown)) {
			liftL.move_velocity(-100);
			liftR.move_velocity(-100);
		}
		else{
			liftL.move_velocity(0);
			liftR.move_velocity(0);
		}

		//BACK
		if(master.get_digital(buttonBackUp)) back.move_velocity(100);
		else if(master.get_digital(buttonBackDown)) back.move_velocity(-100);
		else back.move_velocity(0);

		//HOOK
		if(master.get_digital(buttonHookOut)) hook.move_velocity(200);
		else if(master.get_digital(buttonHookIn)) hook.move_velocity(-200);
		else hook.move_velocity(0);

		pros::delay(10);
	}
}
