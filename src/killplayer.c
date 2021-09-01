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

/* 'killplayer' is called when the player has collided with an object.
 * 	Things are reset and the game may be exited as a result.
 */
killplayer ()
{
	int	i,
		yesmac = FALSE;
	
	for (i = 1; i <= 10; i++) {
		nap (25);
		if ((bttn1() && (!ptinrect (mouse.xy, Drect))) || bttn3())
			serve_mouse ();
		rectf (&display, me.rect, F_XOR);
	}
	put_object (me);

	if (IS_SET(mac, EXISTS)) {
		yesmac = TRUE;
	}
	
	if (IS_SET(mtv, EXISTS)) {
		notecnt--;
	}	

	allobjdeath();

	for (i = 0; i < BADICONMAX; i++)
		if (IS_SET(badicons[i]->obj, EXISTS)) {
			CLEAR_ATT (badicons[i]->obj, EXISTS);
			put_icon (*badicons[i]);
	}
	
	ibm.obj.fuse = IBMGONE;

	if (mtvbuf[0] != '\0') {
		string (&defont, mtvbuf, &display, mtv.rect.origin, F_XOR);
		mtvbuf[0] = '\0';
	}			
	if (macbuf[0] != '\0') {
		put_burger();
		macbuf[0] = '\0';
	}
	if (IS_SET(shot, EXISTS)) {
		CLEAR_ATT (shot, EXISTS);
		put_object (shot);
	}
	if (me_cnt <= 1) {
		exit_game ();
		start_over = TRUE;
		return;
	}
	stop_wanderers();
	del_extra();
	if (yesmac)
		objbirth(&mac);

	init_round ();
}
