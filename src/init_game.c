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

int	firsttime = TRUE;

/* 'init_game' initializes everything for the beginning of a new
 * game.
 */
init_game ()
{
	int	i, setoff;
	
	score = 0;
	prevscore = -1; /* For updating the score display correctly	*/
	bumpmtvat = FIRSTMTVBUMP;
	healrate = MAXHEAL;
	mtvscore = 100;
	notespeed = NOTESPEED;
	round_cnt = 0;
	me_cnt = 1;
	newmanat = NEWMANSTEP;
	mtvbuf[0] = '\0';
	macbuf[0] = '\0';
	wanderers = 0;

	for (i = 0; i < DIGMAX; i++) {
		CLEAR_ATT (digit[i], EXISTS);
	}
	CLEAR_ATT (shot, EXISTS);
	
	ibm.obj.fuse = 1000;	/* Don't show up for a while	*/

	bob.value = BOBSPEED;
	bob.fuse = BOBGONE;

	notecnt = STARTNOTES;
	
	if (firsttime) {
		firsttime = FALSE;

		/* Initialize macdonalds	*/
		macinit();
		
		/* Reverse chunk bits. Damn!	*/
		for (i = 0; i < DEBMAX; i++) {
			rectf (&(chunks[i]), chunks[i].rect, F_XOR);
		}

		init_debris (&bob, bobdebris);
		init_debris (&eddie, eddiedebris);
		init_debris (&mtv, mtvdebris);
		init_debris (&smurf, smurfdebris);

		for (i = 0; i < BLLMAX; i++) {
			bll[i].dirchoose = bllchoose;
		}
		for (i = 0; i < EXMAX; i++) {
			extra[i].dirchoose = extrachoose;
			extra[i].looks = &melooks;
			exshot[i].dirchoose = shotchoose;
			exshot[i].looks = &shotlooks;
			exshot[i].dxdt = 0;
			exshot[i].dydt = SHOTSPEED;
		}
		for (i = 0; i < NOTEMAX; i++) {
			note[i].dirchoose = notechoose;
			note[i].looks = &notelooks;
			note[i].type = NOTE;
		}
		for (i = 0; i < GABMAX; i++) {
			gab[i].dirchoose = gabchoose;
			gab[i].type = GAB;
		}
	}
	
	CLEAR_ATT (smurf, EXISTS|EXPLODING);
	for (i = 0; i < DEBMAX; i++) {
		CLEAR_ATT (smurfdebris[i], EXISTS|EXPLODING);
	}
	
	allobjdeath();
	
	for (i = 0; i < BADICONMAX; i++) {
		CLEAR_ATT (badicons[i]->obj, EXISTS|EXPLODING);
	}
	Dbug = FALSE;
	init_scr ();
	if (Dbug) {
		score += 90000;
		notecnt = NOTEMAX - 1;
		smurfstart();
	}

	setoff = div2 (Drect.origin.x + Drect.corner.x) - 96;
	for (i = 0; i < 6; i++) {
		digit[i].dirchoose = digitchoose;
		digit[i].rect.origin.y = Drect.origin.y;
		digit[i].rect.origin.x = setoff + i * 32;
		digit[i].rect.corner = add (digit[i].rect.origin,
					Pt (32, 32));
	}

	for (i = 1; i < ME_MAX; i++)
		add_extra ();

	disp_score ();

	me.rect = raddp (me.looks->rect, mouse.xy);

	if (me.rect.origin.x < playerrect.origin.x)
		me.rect = raddp (me.looks->rect, Pt (playerrect.origin.x,
						  me.rect.origin.y));
	else if (me.rect.corner.x > playerrect.corner.x) {
		me.rect.corner.x = playerrect.corner.x;
		me.rect.origin.x = playerrect.corner.x - MEWIDTH;
	}

	if (me.rect.origin.y < playerrect.origin.y)
		me.rect = raddp (me.looks->rect, Pt (me.rect.origin.x,
						  playerrect.origin.y));
	else if (me.rect.corner.y > playerrect.corner.y) {
		me.rect.origin.y = playerrect.corner.y - MEHEIGHT;
		me.rect.corner.y = playerrect.corner.y;
	}
	cursset (me.rect.origin);
	oldmouse = me.rect.origin;
	return;
}

init_debris(objp, debrisp)
struct	object	*objp,
		debrisp[];
{
	int	i;

	for (i = 0; i < DEBMAX; i++) {
		debrisp[i].looks = (Bitmap*) balloc(objp->looks->rect);
		if (debrisp[i].looks  == (Bitmap*)NULL) {
			error ("Can't allocate debris");
		}
/*		debrisp[i].looks->width = objp->looks->width;
*/		debrisp[i].looks->rect = objp->looks->rect;
		debrisp[i].type = DEBRIS;
		rectf (debrisp[i].looks, debrisp[i].looks->rect, F_CLR);
		bitblt (objp->looks, objp->looks->rect, debrisp[i].looks,
			Pt(0, 0), F_STORE);
		rectf (debrisp[i].looks, debrisp[i].looks->rect, F_XOR);
		bitblt (&(chunks[i]), objp->looks->rect, debrisp[i].looks,
			Pt(0, 0), F_OR);
		rectf (debrisp[i].looks, debrisp[i].looks->rect, F_XOR);
	}
}
