#pragma config(Motor,  port1,           clawMtr,       tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           lLiftMtr,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           lGoalMtr,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           blDrive,       tmotorVex393HighSpeed_MC29, openLoop, driveLeft)
#pragma config(Motor,  port5,           tlDrive,       tmotorVex393HighSpeed_MC29, openLoop, reversed, driveLeft)
#pragma config(Motor,  port6,           trDrive,       tmotorVex393HighSpeed_MC29, openLoop, driveRight)
#pragma config(Motor,  port7,           brDrive,       tmotorVex393HighSpeed_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port8,           rGoalMtr,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           rLiftMtr,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          chainMtr,      tmotorVex393HighSpeed_HBridge, openLoop)
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

Port 5 - Mobile Goal Intake Limit Switch

*/

/*

Analog Sensor Configuration


Port 1 - 4-Bar Lift Potentiometer

Port 2 - Chain Bar Potentiometer

Port 3 - Claw Potentiometer

*/



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



//Arcade Drive Control Scheme

void arcade(short* ins) { //Takes pointer to short array of input values
	setDriveL(ins[2] + ins[0]); //Left power is equal to sum of x and y axes
	setDriveR(ins[2] - ins[0]); //Right power is equal to difference of y and x axes
}


//Arcade Drive Control Scheme

void arcade(short x, short y) { //Takes 2 short arguments
	setDriveL(y + x); //Left power is equal to sum of x and y axes
	setDriveR(y - x); //Right power is equal to difference of y and x axes
}



//Constant Definitions


//Threshold for analog joysticks to prevent motor whine

#define STICK_THRESH 7


//Task Main Definition

task main()
{
	static short sticks[4]; //Stores values of joystick axes after threshold adjustment implemented below
	while(true) {
		//Compare joystick values against an arbitrary threshold to prevent motor whine and assign to sticks array
		for(byte i = 0; i < 4; i++) //Iterate over all 4 joystick axes
			sticks[i] = (fabs(vexRT[i]) >= STICK_THRESH) //If joystick axis value is greater than threshold
				? vexRT[i] //If true, store joystick axis value in member of sticks array
				: 0; //Otherwise, assign value of zero to member sticks array

		arcade(sticks); //Arcade control implementation (execute arcade function defined on lines 113-116) with threshold-adjusted values

		//Assign 4-bar lift power
		setLift((vexRT[Btn5U] ^ vexRT[Btn5D]) //If exactly one of button 5 up or button 5 down is pressed
			? (vexRT[Btn5U]) //If button 5 up is pressed
				? 127 //Assign power 127
				: -127 //If button 5 down is pressed (5 up is not, but one of the buttons is), assign power -127
			: 0); //If neither button is pressed, assign power 0

		//Assign chain bar power
		setChain((vexRT[Btn8U] ^ vexRT[Btn8D]) //If exactly one of button 8 up or button 8 down is pressed
			? (vexRT[Btn8U]) //If button 8 up is pressed
				? 127 //Assign power 127
				: -127 //If button 8 down is pressed (8 up is not, but one of the buttons is), assign power -127
			: 0); //If neither button is pressed, assign power 0

		//Assign claw power
		setClaw((vexRT[Btn6U] ^ vexRT[Btn6D]) //If exactly one of button 6 up or button 6 down is pressed
			? (vexRT[Btn6U]) //If button 6 up is pressed
				? 127 //Assign power 127
				: -127 //If button 6 down is pressed (6 up is not, but one of the buttons is), assign power -127
			: 0); //If neither button is pressed, assign power 0

		//Assign mobile goal intake power
		setGoal((vexRT[Btn7U] ^ vexRT[Btn7D]) //If exactly one of button 7 up or button 7 down is pressed
			? (vexRT[Btn7U]) //If button 7 up is pressed
				? 127 //Assign power 127
				: -127 //If button 7 down is pressed (7 up is not, but one of the buttons is), assign power -127
			: 0); //If neither button is pressed, assign power 0
			
			
	}

}
