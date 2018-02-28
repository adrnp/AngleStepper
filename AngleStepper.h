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
	enum class StepMode : uint8_t {
		FULL_STEP = 0,
		HALF_STEP,
		QUARTER_STEP,
		EIGTH_STEP
	};

	/**
	 * @brief      possible directions of travel for the motor (may not be of use!!!)
	 */
	enum class Direction : int8_t {
		CW = 1,
		CCW = -1
	};

	enum class AngleMode : uint8_t {
		END_360 = 0,
		END_180
	};

	AngleStepper(StepMode stepMode, AngleMode angleMode, uint8_t pinStep, uint8_t pinDir, int stepsPerRevolution, float gearRatio);
	AngleStepper(StepMode stepMode, uint8_t pinStep, uint8_t pinDir, int stepsPerRevolution);
	AngleStepper(StepMode stepMode, uint8_t pinStep, uint8_t pinDir);


	void setCurrentAngle(float angle) { _currentMicroAngle = (int32_t) (angle*1e6f); };

	void setCurrentStep(int step) { _currentStep = step; };

	void resetAngleSwept() { _microAngleSwept = 0; };

	void setMaxSpeed(int anglePerSecond) { _maxSpeed = anglePerSecond; };

	void setNextStepSize(unsigned int numSteps) { _numSteps = numSteps; };

	void setMoveToNextDirection(Direction dir) { _nextDirection = dir; };

	float getCurrentAngle() { return _currentMicroAngle/1e6f; };

	int32_t getCurrentMicroAngle() { return _currentMicroAngle; };

	int getCurrentStep() { return _currentStep; };

	float getAngleSwept() { return _microAngleSwept/1e6f; };

	int32_t getMicroAngleSwept() { return _microAngleSwept; };

	void reset();


	void moveTo(float angle);
	void moveTo(int32_t angle);

	void moveBy(float deltaAngle);
	void moveBy(int32_t deltaMilliAngle);

	/**
	 * @brief      move to the next angle along the routine.
	 *             for now this assumes that we will want to stop at every FULL step
	 *
	 * @return     the angle to which we have moved
	 */
	int32_t moveToNext();

	// make this public for now, so the adjustment command can work -> potentially want to find better way
	void move(int steps);

private:

	StepMode _stepMode;
	const uint8_t _pinDir;
	const uint8_t _pinStep;

	const int _stepsPerRevolution;
	const float _gearRatio;

	float _anglePerFullStep;
	float _anglePerStep;

	int32_t _microAnglePerStep;


	int32_t _currentMicroAngle;
	float _currentStep;
	Direction _currentDirection;
	int32_t _microAngleSwept;

	float _targetAngle;


	int _maxSpeed;
	unsigned int _numSteps;
	Direction _nextDirection;

	long _stepDelayMicroseconds;

	int32_t _minAllowedAngle;
	int32_t _maxAllowedAngle;


	void calculateStepDelay();

	
	void step();




};



#endif /* AngleStepper_h */