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
_currentMilliAngle(0),
_currentStep(0),
_currentDirection(Direction::CW),
_milliAngleSwept(0),
_targetAngle(0.0f),
_maxSpeed(10),
_numSteps(8)
{
	// based on the motor properties, set the angle per step that is capable
	_anglePerFullStep = 360.0f/_stepsPerRevolution;
	_anglePerStep = 360.0f/(_stepsPerRevolution << _stepMode);

	_milliAnglePerStep = (int) (_anglePerStep*1000.0f);

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
	int32_t milliAngle = (int32_t) (angle*1000.0f);
	int32_t milliAngleDifference = milliAngle - _currentMilliAngle;

	// TODO: handle angle wrapping and such properly!!
	

	// convert angle requirement to step requirement
	// TODO: handle the errors.... (the remainder on the conversion to an int)
	//int stepDifference = (int) round(angleDifference/_anglePerStep);
	int stepDifference = (int) (milliAngleDifference/_milliAnglePerStep);
	move(stepDifference);

}

void AngleStepper::moveBy(float deltaAngle) {

	// make sure we have the proper step delay
	calculateStepDelay();

	// convert angle requirement to step requirement
	// TODO: handle the errors.... (the remainder on the conversion to an int)
	int stepDifference = (int) round(deltaAngle/_anglePerStep);
	move(stepDifference);

}


int32_t AngleStepper::moveToNext() {

	// make sure we have the proper step delay (speed)
	calculateStepDelay();

	// move the number of steps defined by the num steps parameter
	move(_numSteps);

	return _currentMilliAngle;
}


void AngleStepper::calculateStepDelay() {
	// need to calculate the step delay to use
	float stepDelaySeconds = _anglePerStep/_maxSpeed;

	// step delay does need to be a int... so we need to do some rounding
	_stepDelayMicroseconds = (long) (stepDelaySeconds * 1e6);

	// TODO: handle the rounding errors....
}


void AngleStepper::move(int steps) {

	// set the direction based on the sign of the step count
	if (steps < 0) {
		digitalWrite(_pinDir, HIGH);
		_currentDirection = Direction::CCW;
	} else {
		digitalWrite(_pinDir, LOW);
		_currentDirection = Direction::CW;
	}

	// now just take the magnitude of the steps
	steps = abs(steps);

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
    // note: sign depends on current direction
    if (_currentDirection == Direction::CW) {
    	_currentStep++;
    	_currentMilliAngle += _milliAnglePerStep;
    } else {
    	_currentStep--;
    	_currentMilliAngle -= _milliAnglePerStep;
    }

    // update the angle swept -> this is simply cumulative angle, so direction doesn't matter
    _milliAngleSwept += _milliAnglePerStep;

    // make sure angle states between 0 and 360
    // TODO: adjust this based on angle definition mode to be introduced
    if (_currentMilliAngle >= 360000) {
    	_currentMilliAngle -= 360000;
    }
}





