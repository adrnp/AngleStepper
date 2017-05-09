/**
 * AngleStepper.h
 * 
 * Class to control a stepper motor using the easy driver setup.
 * Handles the control of the stepper motor using angles instead of steps that are commonly used.
 * 
 * author     Adrien Perkins <adrienp@stanford.edu>
 * copyright  Stanford University 2017
 */

#ifndef AngleStepper_h
#define AngleStepper_h


#include <Arduino.h>

class AngleStepper {


public:

	/**
	 * @brief      the possible options for the step mode the controller is in
	 */
	enum StepMode : uint8_t {
		FULL_STEP = 0,
		HALF_STEP,
		QUARTER_STEP,
		EIGTH_STEP
	};

	/**
	 * @brief      possible directions of travel for the motor (may not be of use!!!)
	 */
	enum Direction : int8_t {
		CW = 1,
		CCW = -1
	};

	AngleStepper(uint8_t stepMode, uint8_t pinStep, uint8_t pinDir, int stepsPerRevolution, float gearRatio);
	AngleStepper(uint8_t stepMode, uint8_t pinStep, uint8_t pinDir, int stepsPerRevolution);
	AngleStepper(uint8_t stepMode, uint8_t pinStep, uint8_t pinDir);


	void setCurrentAngle(float angle) { _currentAngle = angle; };

	void setCurrentStep(int step) { _currentStep = step; };

	void resetAngleSwept() { _angleSwept = 0.0f; };

	void setMaxSpeed(int anglePerSecond) { _maxSpeed = anglePerSecond; };

	void setNextStepSize(int numSteps) { _numSteps = numSteps; };

	float getCurrentAngle() { return _currentAngle; };

	int getCurrentStep() { return _currentStep; };

	float getAngleSwept() { return _angleSwept; };


	void moveTo(float angle);

	void moveBy(float deltaAngle);

	/**
	 * @brief      move to the next angle along the routine.
	 *             for now this assumes that we will want to stop at every FULL step
	 *
	 * @return     the angle to which we have moved
	 */
	float moveToNext();

private:

	uint8_t _stepMode;
	const uint8_t _pinDir;
	const uint8_t _pinStep;

	const int _stepsPerRevolution;
	const float _gearRatio;

	float _anglePerFullStep;
	float _anglePerStep;


	float _currentAngle;
	float _currentStep;
	Direction _currentDirection;
	float _angleSwept;

	float _targetAngle;


	int _maxSpeed;
	int _numSteps;

	int _stepDelayMicroseconds;


	void calculateStepDelay();

	void move(int steps);
	void step();




};



#endif /* AngleStepper_h */