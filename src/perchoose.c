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

/* 'personchoose' ('*objp' == me for this one) */

personchoose (objp)
struct	object	*objp;
{
	int	c;
	Point	displace;
/*	static int	set_button;	*/
	
	if ((!IS_SET(shot, EXISTS))) {
		if (bttn2()) {
			SET_ATT(shot, EXISTS);
			shot.rect = raddp (shot.looks->rect,
					   Pt(div2(me.rect.corner.x +
						   me.rect.origin.x),
					      me.rect.origin.y));
			put_object (shot);
		}
# ifdef MPX
		else {
			if ((bttn1() && (!ptinrect (mouse.xy, Drect))) ||
			   bttn3()) {
				serve_mouse ();
				if (P->state & RESHAPED) {
					start_over = TRUE;
					return (FALSE);
				}
			}
		}
# endif MPX
	}
	if (own() & KBD) {
		while ((c = kbdchar()) != -1) {
			switch (c) {
				case 'q':
				case 'Q':
					exit_game();
					start_over = TRUE;
					return (FALSE);
			}
		}
	}
	displace = sub (mouse.xy, oldmouse);
	if ((displace.x == 0) && (displace.y == 0))
		return (FALSE);
# ifndef MACSUCK
	oldmouse = mouse.xy;
# endif MACSUCK
	me.next = raddp (me.rect, displace);

	if (me.next.origin.x < playerrect.origin.x)
		me.next = raddp (me.looks->rect, Pt (playerrect.origin.x,
						  me.next.origin.y));
	else if (me.next.corner.x > playerrect.corner.x) {
		me.next.corner.x = playerrect.corner.x;
		me.next.origin.x = playerrect.corner.x - MEWIDTH;
	}

	if (me.next.origin.y < playerrect.origin.y)
		me.next = raddp (me.looks->rect, Pt (me.next.origin.x,
						  playerrect.origin.y));
	else if (me.next.corner.y > playerrect.corner.y) {
		me.next.origin.y = playerrect.corner.y - MEHEIGHT;
		me.next.corner.y = playerrect.corner.y;
	}
# ifdef MACSUCK
	oldmouse = me.next.origin;
	cursset (me.next.origin);
# endif MACSUCK
	return (TRUE);
}
