/*

 *      Copyright (c) 1984, 1985 AT&T
 *      All Rights Reserved

 *      THIS IS UNPUBLISHED PROPRIETARY SOURCE 
 *      CODE OF AT&T.
 *      The copyright notice above does not 
 *      evidence any actual or intended
 *      publication of such source code.

 */
int	sqtable[26] = { 0, 1, 1, 1, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4 };

sqrt (value)
int	value;
{
	long	guess;
	int	iterations;
	
	if (value < 0)
		return (-1);
	if (value < 25)
		return (sqtable[value]);

	guess = value >> 1;
	iterations = 8;

	while (iterations--) {
		guess = (value/guess + guess) >> 1;
	}
	return ((int) guess);
}

