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

/* 'init_scr' checks to make sure there's enugh room to display the game
 *	and then initializes that display.
 */
init_scr ()
{
	int	c;

TOP:	request (KBD|MOUSE);
			
# ifdef MPX
	while (TRUE) {
		P->state &= ~RESHAPED;

		texture16 (&display, Drect, (own()&MOUSE? &plain : &fancy),
			 F_STORE);

		if (((Drect.corner.x - Drect.origin.x) < MINWIDTH) ||
		    ((Drect.corner.y - Drect.origin.y) < MINHEIGHT)) {
			beep;
			string (&defont, "Too small,",
				&display, Drect.origin, F_XOR);
			string (&defont, "reshape.",
				&display, add (Drect.origin, Pt(0, 12)),
				F_XOR);
			cursswitch ((Texture16*)0);
		}
		else
			break;
		while (!(P->state & RESHAPED)) {
			if (((c = kbdchar()) == 'q') || (c == 'Q')) {
				exit (0);
			}
			if ((bttn1() && (!ptinrect (mouse.xy, Drect))) ||
			    bttn3()) {
				serve_mouse ();
				cursswitch ((Texture16*) 0);
			}
			wait (CPU);
		}
	}
# else
	cursswitch ((Texture16*) 0);
	texture16 (&display, Drect, (own()&MOUSE? &plain : &fancy),
		   F_STORE);
# endif
	string (&defont, "G  E  B  A  C  A",
		&display, sub(div(add(Drect.origin, Drect.corner), 2),
			      Pt(0, 30)), F_XOR);

	string (&defont, "Hit any key to start",
		&display, div(add(Drect.origin, Drect.corner), 2), F_XOR);

	while ((c = kbdchar()) == -1) {
		if ((bttn1() && (!ptinrect (mouse.xy, Drect))) ||
		    bttn3()) {
			serve_mouse ();
			cursswitch ((Texture16*) 0);
		}
		wait (CPU);
# ifdef MPX
		if (P->state & RESHAPED) {
			goto TOP;
		}
# endif MPX
	}

	if ( c == 'D') {
		Dbug = TRUE;
	}

	cursswitch (&plain);
	texture16 (&display, Drect, (own()&MOUSE? &plain : &fancy), F_STORE);

	playerrect.origin = Drect.origin;
	playerrect.corner = Drect.corner;

# ifdef SNEAKY
	randomize (Drect);
# endif
/*	rectf (&display, Rect (playerrect.corner.x, Drect.origin.y,
				Drect.corner.x, Drect.corner.y),
	       F_XOR);
	rectf (&display, Rect (playerrect.origin.x, playerrect.corner.y,
				playerrect.corner.x, Drect.corner.y),
	       F_XOR);
*/
	return (TRUE);
}

# ifdef SNEAKY
randomize(rect)
Rectangle	rect;
{
	int	i, j;
	Bitmap	scram;
	unsigned long	mix;

	scram.base = (Word *) &mix;
	scram.width = BLTWIDTH;
	scram.rect.origin.x = 0;
	scram.rect.origin.y = 0;
	scram.rect.corner.x = 30;
	scram.rect.corner.y = 1;

	for (j = rect.origin.y + 32; j < playerrect.corner.y; j++)
		for (i = rect.origin.x; i < playerrect.corner.x; i += 30) {
			mix = rand() << 2 | rand() << 17;
			bitblt (&scram, scram.rect, &display,
				Pt (i, j), F_STORE);
		}
}
# endif
