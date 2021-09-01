/*

 *      Copyright (c) 1984, 1985 AT&T
 *      All Rights Reserved

 *      THIS IS UNPUBLISHED PROPRIETARY SOURCE 
 *      CODE OF AT&T.
 *      The copyright notice above does not 
 *      evidence any actual or intended
 *      publication of such source code.

 */
# include	"playdefs.h"

i4	shot_bits[] = { /* What player's missile looks like	*/
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0x00000000,
	0x00000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000,
	0xE0000000
};

Bitmap	shotlooks = { /* For displaying	player's missile	*/
	(Word*) shot_bits,
	BLTWIDTH,
	{ { 0, 0 }, { 3, 26 } }
};


struct	object	shot = { {{0, 0}, {0, 0}},
		       {{0, 0}, {0, 0}}, 0, SHOTSPEED, 0, 0, 0,
			shotchoose, &shotlooks};

/* 'shotchoose' determines how the player's missile should move
 * next.
 */
shotchoose (objp)
struct	object	*objp;
{
	objp->next = raddp (objp->rect, Pt(objp->dxdt, objp->dydt));
	/* check for going off the screen in shot_collision. BECAUSE! */
	return (TRUE);
}

/* 'shot_collision' checks to see if the player's missile hit anything
 */
shot_collision(objp)
struct	object	*objp;
{
	struct	objnode	*np;
	int	i;
		
	if (!(ptinrect (objp->rect.origin, Drect))) {
		CLEAR_ATT (*objp, EXISTS);
		put_object (*objp);
		return;
	}

	for (np = headnode; np != NULLNODE; np = np->nextnode) {
		if (rectXrect (objp->rect, np->objpoint->rect)) {
			switch (np->objpoint->type) {
				case BALL:
				case BELL:
					killbll (objp, np->objpoint);
					return;
				case BOB:
					killbob (objp);
					return;
				case EDDIE:
					killeddie (objp);
					return;
				case GAB:
					killgab (objp, np->objpoint);
					return;
				case MAC:
					killmac (objp);
					return;
				case MTV:
					killmtv (objp);
					return;
				default:
					break;
			}
		}
	}
	for (i = 0; i < BADICONMAX; i++) {
		if (rectXrect (objp->rect, badicons[i]->obj.rect) &&
		    IS_SET(badicons[i]->obj, EXISTS)) {
			switch (badicons[i]->obj.type) {
				case IBM:
					killibm (objp);
					return;
				case III:
				case BBB:
				case MMM:
					killIBM(objp, badicons[i]);
					return;
			}
		}
	}
	if (rectXrect (objp->rect, smurf.rect) && IS_SET(smurf, EXISTS)) {
		killsmurf (objp);
		return;
	}
	/* Some special cases	*/
	for (i = 0; i < EXMAX; i++) {
		if (objp == &shot) {
			if (IS_SET(extra[i], MOVING) &&
			    (rectXrect (objp->rect, extra[i].rect))) {
				killextra (i, objp);
				return;
			}
		}
		else {
			if (rectXrect (objp->rect, me.rect)) {
				killplayer ();
				return;
			}
		}
	}
}

