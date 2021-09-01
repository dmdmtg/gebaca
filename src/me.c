/*

 *      Copyright (c) 1984, 1985 AT&T
 *      All Rights Reserved

 *      THIS IS UNPUBLISHED PROPRIETARY SOURCE 
 *      CODE OF AT&T.
 *      The copyright notice above does not 
 *      evidence any actual or intended
 *      publication of such source code.

 */
# include "playdefs.h"

i4	me_bits[] = {
	0x007FFE00,
	0x01FFFF80,
	0x7FFFFFFE,
	0xFFE1D3FF,
	0x0380C3C0,
	0x03781EF0,
	0x07FC3FFA,
	0x0FCC33FA,
	0x1FB42DFC,
	0x33B4ADDC,
	0x7385E1CE,
	0x63CDB3C7,
	0xC1FD9F83,
	0xC1FD9F83,
	0xC0FB0F13,
	0xD8030013,
	0xC2060003,
	0xC31E0303,
	0xC01C0003,
	0xCC3C7023,
	0xC03CDB11,
	0xE23FF807,
	0xE2003007,
	0x7000001F,
	0x3EADFFFE,
	0x1FFFFFFC,
	0x00E1C380,
	0x00F3E7C0,
	0x01FBE7E0,
	0x07BBEE70,
	0x0F0FFC38,
	0x0E07F83C
};

Bitmap	melooks = { /* For displaying slug (mushroom dropper) */
	(Word*) me_bits,
	BLTWIDTH,
	{ { 0, 0 }, { 32, 32 } }
};

struct	object	me = { {{0, 0}, {0, 0}},
		       {{0, 0}, {0, 0}}, 0, 0, 0, ME, 0,
			personchoose, &melooks};

/* 'chk_mecollision' sees if the player bumped into anything. */
chk_mecollision()
{
	register struct	objnode	*np;
	register int	i;
	
	for (np = headnode; np != NULLNODE; np = np->nextnode) {
		if (rectXrect (me.rect, np->objpoint->rect) &&
		    objXobj (&me, np->objpoint)) {
			killplayer ();
			return;
		}
	}
	for (i = 0; i < BADICONMAX; i++) {
		if (rectXrect (me.rect, badicons[i]->obj.rect) &&
		    IS_SET(badicons[i]->obj,EXISTS) &&
		    objXobj (&me, &(badicons[i]->obj))) {
			killplayer ();
			return;
		}
	}
}
