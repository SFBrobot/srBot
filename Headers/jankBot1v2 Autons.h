
//Macro Definitions

#ifndef MACRO_DEF //Include guard; if macros are already defined, don't define them again

#define MACRO_DEF

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

#endif

//Does nothing currently, routine to be written soon
void lAuton() {
	displayLCDCenteredString(0, "Left");
	bLCDBacklight = true;
	wait1Msec(5000);
}

//Does nothing currently, routine to be written soon
void rAuton() {
	bLCDBacklight = true;
	displayLCDCenteredString(0, "Right");
	wait1Msec(5000);
}

void defaultAuton() {
	bLCDBacklight = true;
	displayLCDCenteredString(0, "Default");
	while(SensorValue[lDriveEnc] < 360 && SensorValue[rDriveEnc] < 360) {
		setDriveL(127);
		setDriveR(127);
		wait1Msec(20);
	}
	setDriveL(-63);
	setDriveR(-63);
	wait1Msec(20);
	setDriveR(0);
	setDriveL(0);
	wait1Msec(5000);
}
