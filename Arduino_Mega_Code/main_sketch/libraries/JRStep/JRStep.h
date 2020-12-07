
//      ******************************************************************
//      *                                                                *
//      *                 Header file for JRStep.c                       *
//      *                                                                *
//      *            Johannes Röring                23/11/2020           *
//      *               Copyright (c) J. Röring, 2020                    *
//      *                                                                *
//      ******************************************************************
//	This library is based on JRStep

// MIT License
// 
// Copyright (c) 2020 Johannes Röring
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#ifndef JRStep_h
#define JRStep_h

#include <Arduino.h>
#include <stdlib.h>


//
// the JRStep class
//
class JRStep
{
  public:
    //
    // public functions
    //
    JRStep();
    void connectToPins(byte stepPinNumber, byte directionPinNumber, bool positiveDirectionState);

    void setStepsPerMillimeter(float motorStepPerMillimeter);
    float getCurrentPositionInMillimeters();
    void setCurrentPositionInMillimeters(float currentPositionInMillimeters);
    void setCurrentPositionInMillimeter(float currentPositionInMillimeter);
    void setSpeedInMillimetersPerSecond(float speedInMillimetersPerSecond);
    void setAccelerationInMillimetersPerSecondPerSecond(float accelerationInMillimetersPerSecondPerSecond);
    bool moveToHomeInMillimeters(long directionTowardHome, float speedInMillimetersPerSecond, long maxDistanceToMoveInMillimeters, int homeLimitSwitchPin);
    void moveRelativeInMillimeters(float distanceToMoveInMillimeters);
    void setTargetPositionRelativeInMillimeters(float distanceToMoveInMillimeters);
    void moveToPositionInMillimeters(float absolutePositionToMoveToInMillimeters);
    void setTargetPositionInMillimeters(float absolutePositionToMoveToInMillimeters);
    float getCurrentVelocityInMillimetersPerSecond();
   
    void setStepsPerRevolution(float motorStepPerRevolution);
    void setCurrentPositionInRevolutions(float currentPositionInRevolutions);
    float getCurrentPositionInRevolutions();
    void setSpeedInRevolutionsPerSecond(float speedInRevolutionsPerSecond);
    void setAccelerationInRevolutionsPerSecondPerSecond(float accelerationInRevolutionsPerSecondPerSecond);
    bool moveToHomeInRevolutions(long directionTowardHome, float speedInRevolutionsPerSecond, long maxDistanceToMoveInRevolutions, int homeLimitSwitchPin);
    void moveRelativeInRevolutions(float distanceToMoveInRevolutions);
    void setTargetPositionRelativeInRevolutions(float distanceToMoveInRevolutions);
    void moveToPositionInRevolutions(float absolutePositionToMoveToInRevolutions);
    void setTargetPositionInRevolutions(float absolutePositionToMoveToInRevolutions);
    float getCurrentVelocityInRevolutionsPerSecond();

    void setCurrentPositionInSteps(long currentPositionInSteps);
    long getCurrentPositionInSteps();
    void setSpeedInStepsPerSecond(float speedInStepsPerSecond);
    void setAccelerationInStepsPerSecondPerSecond(float accelerationInStepsPerSecondPerSecond);
    bool moveToHomeInSteps(long directionTowardHome, float speedInStepsPerSecond, long maxDistanceToMoveInSteps, int homeSwitchPin);
    void moveRelativeInSteps(long distanceToMoveInSteps);
    void setTargetPositionRelativeInSteps(long distanceToMoveInSteps);
    void moveToPositionInSteps(long absolutePositionToMoveToInSteps);
    void setTargetPositionInSteps(long absolutePositionToMoveToInSteps);
    void setTargetPositionToStop();
    bool motionComplete();
    float getCurrentVelocityInStepsPerSecond(); 
    bool processMovement(void);


  private:
    //
    // private functions
    //
    void DeterminePeriodOfNextStep();

 
    //
    // private member variables
    //
    byte stepPin;
    byte directionPin;
    float stepsPerMillimeter;
    float stepsPerRevolution;
    int directionOfMotion;
    long currentPosition_InSteps;
    long targetPosition_InSteps;
    float desiredSpeed_InStepsPerSecond;
    float desiredPeriod_InUSPerStep;
    float acceleration_InStepsPerSecondPerSecond;
    float acceleration_InStepsPerUSPerUS;
    float periodOfSlowestStep_InUS;
    float minimumPeriodForAStoppedMotion;
    float nextStepPeriod_InUS;
    unsigned long lastStepTime_InUS;
    float currentStepPeriod_InUS;
};

// ------------------------------------ End ---------------------------------
#endif

