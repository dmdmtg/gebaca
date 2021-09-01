/*

 *      Copyright (c) 1984, 1985 AT&T
 *      All Rights Reserved

 *      THIS IS UNPUBLISHED PROPRIETARY SOURCE 
 *      CODE OF AT&T.
 *      The copyright notice above does not 
 *      evidence any actual or intended
 *      publication of such source code.

 */
#include "playdefs.h"

struct	object	digit[6];

/* 'disp_score' displays the value of 'score'.				*/

disp_score ()
{
	register	int	i;
	long	tempscr, prevtemp;
	
	if (score < 0)
		score = 0;
	tempscr = score;
	prevtemp = prevscore;

	/* show score */
	if (prevtemp == -1) {
		for (i = 5; i >= 0; i--) {
			digit[i].looks = &(N[tempscr % 10]);
			put_object (digit[i]);
			tempscr /= 10;
		}
	}
	else {
		for (i = 5; i >= 0; i--) {
			if ((tempscr % 10) != (prevtemp % 10)) {
				put_object (digit[i]);
				digit[i].looks = &(N[tempscr % 10]);
				put_object (digit[i]);
			}
			prevtemp /= 10;
			tempscr /= 10;
		}
	}
	if (score >= newmanat) {
		newmanat += NEWMANSTEP;
		add_extra ();
		/* make sounds */
		beep;
		nap(10);
		beep;
		nap(10);
		beep;
		nap(10);
	}
	prevscore = score;
}

startdigits()
{
	int	i;
	
	for (i = 0; i < DIGMAX; i++) {
		SET_ATT(digit[i], EXISTS);
		digit[i].dxdt = 0;
		digit[i].dydt = 0;
		digit[i].value = urnd (1, 2);
	}
}

digitchoose(objp)
struct	object	*objp;
{
	if ((objp->dydt == 0) && (objp->rect.corner.y == Drect.corner.y))
		return (FALSE);
	objp->dydt += objp->value;
	objp->next = raddp (objp->rect, Pt (objp->dxdt, objp->dydt));
	
	if (!(isallin(objp->next, playerrect))) {
		if (objp->dydt > 0)
			objp->dydt = -(objp->dydt - min(urnd(2, 4),
							objp->dydt));
		else
			objp->dydt = -objp->dydt;
		objp->next = raddp (objp->rect, Pt (objp->dxdt, objp->dydt));
	}
	return (TRUE);
}
