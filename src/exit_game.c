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

/* 'exit_game' updates score file if necessary and exits or starts
 *	a new game if the player desires.
 */
exit_game ()
{
	Point	poynt;

	poynt.x = ((Drect.corner.x - Drect.origin.x) - 300) / 2 + Drect.origin.x;
	poynt.x = max (poynt.x, Drect.origin.x);
	poynt.y = (Drect.origin.y + Drect.corner.y) / 2;
	string (&defont, "                                ",
		&display, poynt, F_STORE);
	string (&defont, "<space> to go again, 'q' to quit",
		&display, poynt, F_STORE);

	cursswitch ((Texture16*) 0);
	while (TRUE) {
		if ((bttn1() && (!ptinrect (mouse.xy, Drect))) ||
		    bttn3()) {
			serve_mouse ();
			cursswitch ((Texture16*) 0);
		}
		wait(CPU);
		switch (kbdchar()) {
			case 'q':
			case 'Q':
				doublecheck ();
			case ' ':
			case '\n':
				start_over = TRUE;
				return (FALSE);
		}
# ifdef MPX
		if (P->state & RESHAPED) {
			start_over = TRUE;
			return (FALSE);
		}
# endif MPX
	}
}

/* 'doublecheck' assures that the player really wants to exit the
 *	layer.
 */
doublecheck ()
{
	int	c;
	
	texture16 (&display, Drect, (own()&MOUSE? &plain : &fancy), F_STORE);
	string (&defont, "Really exit Gebaca? (y or n)",
		&display, div(add(Drect.origin, Drect.corner), 2), F_XOR);

	while (TRUE) {
		if ((bttn1() && (!ptinrect (mouse.xy, Drect))) ||
		    bttn3()) {
			serve_mouse ();
			cursswitch((Texture16*)0);
		}
		if (((c = kbdchar()) == 'q') || (c == 'Q') ||
		    (c == 'y') || (c == 'Y')) {
			exit (0);
		}
		else if (c != -1)
			return;
		wait (CPU);
# ifdef MPX
		if (P->state & RESHAPED) {
			return (FALSE);
		}
# endif MPX
	}
}
