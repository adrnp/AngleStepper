/**
 * AngleStepper.cpp
 * 
 * author     Adrien Perkins <adrienp@stanford.edu>
 * copyright  Stanford University 2017
 */
#include "AngleStepper.h"


AngleStepper::AngleStepper(uint8_t stepMode, uint8_t pinStep, uint8_t pinDir, int stepsPerRevolution, float gearRatio) : 
_stepMode(stepMode),
_pinStep(pinStep),
_pinDir(pinDir),
_stepsPerRevolution(stepsPerRevolution),
_gearRatio(gearRatio),
_currentAngle(0),
_currentStep(0),
_targetAngle(0),
_maxSpeed(10)
{
	// based on the motor properties, set the angle per step that is capable
	_anglePerFullStep = 360.0f/_stepsPerRevolution;
	_anglePerStep = 360.0f/(_stepsPerRevolution << _stepMode);

	calculateStepDelay();

	// configure the pin modes
	pinMode(pinDir, OUTPUT);
	pinMode(pinStep, OUTPUT);

	// make sure step pin is low
	digitalWrite(pinStep, LOW);
}

AngleStepper::AngleStepper(uint8_t stepMode, uint8_t pinStep, uint8_t pinDir, int stepsPerRevolution) : AngleStepper(stepMode, pinStep, pinDir, stepsPerRevolution, 1) {}
AngleStepper::AngleStepper(uint8_t stepMode, uint8_t pinStep, uint8_t pinDir) : AngleStepper(stepMode, pinStep, pinDir, 200, 1) {}


void AngleStepper::moveTo(float angle) {

	// make sure we have the proper step delay
	calculateStepDelay();

	// find how how much we need to move
	float angleDifference = angle - _currentAngle;

	// TODO: handle angle wrapping and such properly!!
	

	// convert angle requirement to step requirement
	// TODO: handle the errors.... (the remainder on the conversion to an int)
	int stepDifference = (int) (angleDifference/_anglePerStep);
	move(stepDifference);

}

void AngleStepper::moveBy(float deltaAngle) {

	// make sure we have the proper step delay
	calculateStepDelay();

	// convert angle requirement to step requirement
	// TODO: handle the errors.... (the remainder on the conversion to an int)
	int stepDifference = (int) (deltaAngle/_anglePerStep);
	move(stepDifference);

}


float AngleStepper::moveToNext() {

	// make sure we have the proper step delay (speed)
	calculateStepDelay();

	// going to move a full step, so need to determine if that is 1, 2, 4 or 8 steps based on the mode
	int stepDifference = (1 << _stepMode);
	move(stepDifference);

	return _currentAngle;
}


void AngleStepper::calculateStepDelay() {
	// need to calculate the step delay to use
	float stepDelaySeconds = _anglePerStep/_maxSpeed;

	// step delay does need to be a int... so we need to do some rounding
	_stepDelayMicroseconds = (int) (stepDelaySeconds * 1e6);

	// TODO: handle the rounding errors....
}


void AngleStepper::move(int steps) {

	// TODO: set the direction based on the sign of the steps

	// loop through and move the desired number of steps
	while (steps > 0) {
		step();
		steps--;
	}
}

void AngleStepper::step() {

	// TODO: this is where acceleration, etc should be handled
	
	// send a pulse to the controller to move by a step
	digitalWrite(_pinStep, HIGH);
    delayMicroseconds(_stepDelayMicroseconds);
    digitalWrite(_pinStep, LOW);

    // update the current state
    _currentStep++;
    _currentAngle += _anglePerStep;
}





