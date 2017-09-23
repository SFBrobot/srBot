
#ifndef PID //If PID is not defined (pid.h not included)

#define PID //Define PID

typedef struct {
	float targ,
		val,
		err,
		errLast,
		dErr,
		integ,
		out,
		ks[3]; //Lets me use a for loop to initialize the Pid struct
	/*
	ks[0] = kP
	ks[1] = kI
	ks[2] = kD
	*/
	int	mtrPwr;
} Pid;

Pid* initPid(Pid* pid, float* consts) {
	for(int i = 0; i < 3; i++)
		pid->ks[i] = consts[i];
	pid->out =
		pid->targ =
		pid->val =
		pid->errLast =
		pid->err =
		pid->dErr =
		pid->integ =
		0;
	pid->mtrPwr = 0;
	return pid;
}

float setTarg(Pid* pid, float target) {
	pid->targ = target;
	pid->errLast =
		pid->err =
		pid->dErr =
		pid->integ =
		0;
	pid->mtrPwr = 0;
	return pid->targ;
}

void setVal(Pid* pid, float value) {
	pid->val = value;
	pid->errLast = pid->err;
	pid->err = pid->targ - pid->val;
	pid->integ += pid->err * pid->ks[1];

	//Prevent integral windup by limiting integral to + or - 127
	if(fabs(pid->integ) > 127)
		pid->integ = 127 * sgn(pid->integ);

	pid->dErr = pid->err - pid->errLast;
	return;
}

int calc(Pid* pid) {
	pid->out =
		(pid->err * pid->ks[0])
		+ pid->integ
		+ (pid->dErr * pid->ks[2]);

	if(fabs(pid->out) > 127)
		pid->out = 127 * sgn(pid->out);

	pid->mtrPwr = round(pid->out);
	return pid->mtrPwr;
}

//Update PID value and calculation
int upPid(Pid* pid, float val) {
	setVal(pid, val);
	return calc(pid);
}

#endif
