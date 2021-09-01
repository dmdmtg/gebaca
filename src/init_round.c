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

init_round ()
{
	int	i;

	eddie.fuse = EDDIEGONE;
	mtv.fuse = MTVGONE;

	curnotes = notecnt;
	thetarange = 180;
	if (mtvbuf[0] != '\0') {
		string (&defont, mtvbuf, &display, mtv.rect.origin, F_XOR);
		mtvbuf[0] = '\0';
	}

	put_object (me);

	/* This is for the player to situate himself 	*/
	/*	before the shit begins to fly.		*/
	for (i = 0; i < 100; i++) {
		sleep (1);
		moveobj(me);
	}
}
