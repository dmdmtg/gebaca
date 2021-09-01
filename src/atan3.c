/*

 *      Copyright (c) 1984, 1985 AT&T
 *      All Rights Reserved

 *      THIS IS UNPUBLISHED PROPRIETARY SOURCE 
 *      CODE OF AT&T.
 *      The copyright notice above does not 
 *      evidence any actual or intended
 *      publication of such source code.

 */

long	tanvals[45] = {
	0,	17,	35,	53,	71,
	89,	107,	125,	143,	162,
	180,	199,	217,	236,	255,
	274,	293,	313,	332,	352,
	372,	393,	413,	434,	455,
	477,	499,	521,	544,	567,
	591,	615,	639,	664,	690,
	717,	743,	771,	800,	829,
	859,	890,	922,	954,	988
};

atan3(x, y) /* does y/x consistent with jerq atan2*/
int	x, y;
{
	long	ratio;
	int	lowlim, uplim, guess,
		flipit = 0,
		negate = 0;
		
	if (x == 0) {
		if (y < 0)
			return (270);
		else
			return (90);
	}
	if (y == 0) {
		if (x < 0)
			return (180);
		else
			return (0);
	}
	ratio = (long) 1024 * y / x;
	if ((ratio < -1024) || (ratio > 1024)) {
		ratio = (long) 1024 * x / y;
		flipit = 1;
	}

	if (ratio < 0) {
		ratio = -ratio;
		negate = 1;
	}
	lowlim = 0;
	uplim = 44;
	guess = 22;
	while (uplim - lowlim > 1) {
		if (tanvals[guess] > ratio)
			uplim = guess;
		else
			lowlim = guess;
		guess = (uplim + lowlim) / 2;
	}
	if (ratio < (tanvals[lowlim] + tanvals[uplim]) / 2)
		guess = lowlim;
	else
		guess = uplim;
	
	if (flipit) {
		if (negate)
			guess += 90;
		else
			guess = 90 - guess;
	}
	else if (negate)
		guess = 180 - guess;

	if (y < 0)
		guess += 180;

	return (guess);
}
