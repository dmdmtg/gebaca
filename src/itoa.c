/*

 *      Copyright (c) 1984, 1985 AT&T
 *      All Rights Reserved

 *      THIS IS UNPUBLISHED PROPRIETARY SOURCE 
 *      CODE OF AT&T.
 *      The copyright notice above does not 
 *      evidence any actual or intended
 *      publication of such source code.

 */
/*
 * 'itoa' puts an integer value into string form without invoking the
 *	overhead of sprintf.
 */
itoa(str, num, digits)
char	*str;
int	num, digits;
{
	register int	dcnt = digits;
			
	str[dcnt--] = '\0';
	while (dcnt >= 0) {
		str[dcnt] = ((num == 0) ? ' ' : (num % 10) + '0');
		num /= 10;
		dcnt--;
	}
	return;
}
