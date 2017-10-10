#pragma config(Sensor, in1,    liftPot,        sensorPotentiometer)
#pragma config(Sensor, in2,    clawPot,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  lDriveEnc,      sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rDriveEnc,      sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  chainEnc,       sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  goalBtn,        sensorDigitalIn)
#pragma config(Motor,  port1,           brDrive,       tmotorVex393HighSpeed_HBridge, openLoop, reversed, driveLeft)
#pragma config(Motor,  port2,           trDrive,       tmotorVex393HighSpeed_MC29, openLoop, reversed, driveLeft)
#pragma config(Motor,  port3,           lGoalMtr,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           clawMtr,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           lLiftMtr,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           rLiftMtr,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           chainMtr,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           rGoalMtr,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           tlDrive,       tmotorVex393HighSpeed_MC29, openLoop, driveRight)
#pragma config(Motor,  port10,          blDrive,       tmotorVex393HighSpeed_HBridge, openLoop, driveRight)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*

Motor Configuration

Port 1 - Cone Intake Motor (Claw)
Port 2- Left 4-Bar Lift Motor
Port 3- Left Mobile Goal Lift Motor (Power Expander)
Port 4- Bottom Left Drive Motor
Port 5- Top Left Drive Motor (Power Expander)
Port 6- Top Right Drive Motor (Power Expander)
Port 7- Bottom Right Drive Motor
Port 8- Right Mobile Goal Lift Motor
Port 9- Right 4-Bar Lift Motor
Port 10- Chain Bar Lift Motor

*/

/*

Digital Sensor Configuration


Ports 1 & 2 - Left Drive Encoder

Ports 3 & 4 - Right Drive Encoder

Ports 5 & 6 - Chain Bar Encoder

Port 7 - Mobile Goal Intake Limit Switch

*/

/*

Analog Sensor Configuration


Port 1 - 4-Bar Lift Potentiometer

Port 2 - Claw Potentiometer

*/

//#includes

//PID Library
#include "pid.h"

//Macro Definitions


//Setting Left Drive Motor

#define setDriveL(pwr) motor[blDrive] = \
	motor[tlDrive] = \
	pwr


//Setting Right Drive Power

#define setDriveR(pwr) motor[brDrive] = \
	motor[trDrive] = \
	pwr


//Setting Mobile Goal Lift Power

#define setGoal(pwr) motor[lGoalMtr] = \
	motor[rGoalMtr] = \
	pwr


//Set Cone Claw Power

#define setClaw(pwr) motor[clawMtr] = pwr


//Set Chain Bar Power

#define setChain(pwr) motor[chainMtr] = pwr


//Set 4-Bar Lift Power

#define setLift(pwr) motor[lLiftMtr] = \
	motor[rLiftMtr] = \
	pwr


//Function Definitions

int arcadeL(short* ins) {
	return ins[2] + ins[3];
}

int arcadeR(short* ins) {
	return ins[2] - ins[3];
}

/*
void arcade(short* ins) { //Takes pointer to short array of input values
	setDriveL(arcadeL(ins)); //Left power is equal to sum of x and y axes
	setDriveR(arcadeR(ins)); //Right power is equal to difference of y and x axes
}

void arcade(short x, short y) { //Takes 2 short arguments
	setDriveL(y + x); //Left power is equal to sum of x and y axes
	setDriveR(y - x); //Right power is equal to difference of y and x axes
}
*/


//Constant Definitions


//Threshold for analog joysticks to prevent motor whine
//Range of dangerous lift values
//Maximum change in drive power
const int STICK_THRESH = 7,
	CLAW_OPEN_POT = 160,
	DRIVE_MAX_SLEW = 31,
	CLAW_PWR_HOLD = 15;
const float pidConsts[4][3] = {
	{ .1, .001, .01 },
	{ .1, .001, .01 },
	{ .1, .001, .01 },
	{ .1, .001, .01 }
};
const float liftFac = 4;

//Struct declarations
Pid rDrivePid,
	lDrivePid,
	liftPid,
	chainPid;
Pid* pids[4] = { &rDrivePid, &lDrivePid, &liftPid, &chainPid };

//Global variable declarations
int liftPotVal = 0,
	lDrivePwr,
	lDrivePwrLast = 0,
	rDrivePwr,
	rDrivePwrLast = 0,
	liftPwr,
	chainPwr,
	clawPwr,
	goalPwr;

signed char btns[6] = { 0, 0, 0, 0, 0, 0 },
	btnLast[6] = { 0, 0, 0, 0, 0, 0 };
	
bool liftPrecision = false,
	driveFlip = false;


//Task Main Definition

task main()
{
	static short sticks[4]; //Stores values of joystick axes after threshold adjustment implemented below
	int swap = 0;

	for(char i = 0; i < (sizeof(pids) / sizeof(Pid)); i++)
		initPid(pids[i], &pidConsts[i][0]);
	
	while(true) {
		//Compare joystick values against an arbitrary threshold to prevent motor whine and assign to sticks array
		for(byte i = 0; i < 4; i++) //Iterate over all 4 joystick axes
			sticks[i] = (fabs(vexRT[i]) >= STICK_THRESH) //If joystick axis value is greater than threshold
				? vexRT[i] //If true, store joystick axis value in member of sticks array
				: 0; //Otherwise, assign value of zero to member sticks array

		btns[0] = (vexRT[Btn5U] ^ vexRT[Btn5D])
			? (vexRT[Btn5U])
				? 1
				: -1
			: 0;

		btns[1] = (vexRT[Btn6U] ^ vexRT[Btn6D])
			? (vexRT[Btn6U])
				? 1
				: -1
			: 0;

		btns[2] = (vexRT[Btn8U] ^ vexRT[Btn8D])
			? (vexRT[Btn8U])
				? 1
				: -1
			: 0;

		btns[3] = (vexRT[Btn8L] ^ vexRT[Btn8R])
			? (vexRT[Btn8L])
				? 1
				: -1
			: 0;
			
		btns[4] = (vexRT[Btn7D])
			? 1
			: 0;
		
		btns[5] = (vexRT[Btn7L])
			? 1
			: 0;

		driveFlip = (btns[5] && !btnLast[5])
			? !driveFlip
			: driveFlip;
			
		liftPrecision = (btns[4] && !btnLast[4])
			? !liftPrecision
			: liftPrecision;
			
		lDrivePwr = arcadeL(sticks);
		rDrivePwr = arcadeR(sticks);

		if(!btns[0] && btnLast[0]) //Button Pair 5
			setTarg(pids[2], SensorValue[liftPot]); //Set target value for 4-bar PID

		if(!btns[1] && btnLast[1]) //Button Pair 5
			setTarg(pids[3], SensorValue[chainEnc]); //Set target value for Chain Bar PID

		chainPwr = ((btns[1]) //Button Pair 6
			? (btns[1] > 0)
				? 127
				: -127
			: upPid(pids[3], SensorValue[chainEnc])); //Update Chain Bar PID

		liftPwr = ((btns[0]) //Button Pair 5
			? (btns[0] > 0)
				? 127
				: -127
			: upPid(pids[2], SensorValue[liftPot])); //Update 4-bar PID
			
		clawPwr = (btns[3]) //Button Pair 8L/R
			? (btns[3] > 0)
				? 127
				: -127
			: (CLAW_PWR_HOLD * sgn(SensorValue[clawPot] - CLAW_OPEN_POT));
			
		goalPwr = (btns[2]) //Button Pair 8U/D
			? (btns[2] > 0)
				? 127
				: -127
			: 0;
			
		lDrivePwr = ((fabs(lDrivePwr - lDrivePwrLast) > DRIVE_MAX_SLEW)
			? lDrivePwrLast + (DRIVE_MAX_SLEW * sgn(lDrivePwr - lDrivePwrLast))
			: lDrivePwr);

		rDrivePwr = ((fabs(rDrivePwr - rDrivePwrLast) > DRIVE_MAX_SLEW)
			? rDrivePwrLast + (DRIVE_MAX_SLEW * sgn(rDrivePwr - rDrivePwrLast))
			: rDrivePwr);
			
		if(driveFlip) {
			swap = lDrivePwr;
			lDrivePwr = -rDrivePwr;
			rDrivePwr = -swap;
		}
			
		if(liftPrecision)
			liftPwr = round(((liftPwr + 1) / liftFac) - 1);

		lDrivePwrLast = lDrivePwr;
		rDrivePwrLast = rDrivePwr;
		for(char i = 0; i < sizeof(btns) / sizeof(char); i++)
			btnLast[i] = btns[i];
	
		setDriveL(lDrivePwr);
		setDriveR(rDrivePwr);
		setClaw(clawPwr);
		setLift(liftPwr);
		setChain(chainPwr);
		setGoal(goalPwr);

		wait1Msec(20);

	}

}
