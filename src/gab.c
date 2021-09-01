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
# include "font.h"

/* See also globals.c for the actual gab	*/

struct	object	gab[GABMAX];

gabstart ()
{
	int	i;
	Point	poynt;
	int	bead;
	int	gablen,
		rateofgab;

	rateofgab = (score < 30000 ? GABSPEED : score / 3000);
	rateofgab = min (rateofgab, 30);
	for (i = 0; i < GABMAX; i++)
		if (!IS_SET(gab[i], EXISTS))
			break;
	if (i >= GABMAX)
		return;
		
	gablen = Gstrlen(gabwords[gabp]);

	gab[i].looks = (Bitmap*) balloc (Rect(0, 0, DEFONTWIDTH * gablen,
					      DEFONTHEIGHT));
	
	string (&defont, gabwords[gabp], gab[i].looks, Pt(0, 0), F_STORE);
		
	gabp = (gabp + 1) % WORDMAX;

	poynt = sub (div (add (eddie.rect.corner, eddie.rect.origin), 2),
		     div (gab[i].looks->rect.corner, 2));
	gab[i].rect = raddp (gab[i].looks->rect, poynt);
	
	bead = atan3(div2(me.rect.origin.y + me.rect.corner.y) -
		     div2(eddie.rect.origin.y + eddie.rect.corner.y),
		     div2(me.rect.origin.x + me.rect.corner.x) -
		     div2(eddie.rect.origin.x + eddie.rect.corner.x));
	
	gab[i].dydt = muldiv (cos(bead), rateofgab, 1024);
	gab[i].dxdt = muldiv (sin(bead), rateofgab, 1024);
	
	gab[0].fuse = GABGONE;
	if (gab[i].dydt > 0)
		gab[0].fuse -= gab[i].dydt / 3; /* Strange but should have words */
			/* come out faster when player is underneath eddie */
	objbirth(&gab[i]);
}

/* 
 * 'gabchoose'
 */
gabchoose (objp)
struct	object	*objp;
{
	objp->next = raddp (objp->rect, Pt(objp->dxdt, objp->dydt));
	if (!(ptinrect (objp->next.origin, playerrect))) {
		objdeath(objp);
		bfree (objp->looks);
		return (FALSE);
	}
	return (TRUE);
}

/*
 * 'killgab' is for shooting eddie's words.
 */
killgab(killerp, objp)
struct object	*killerp, *objp;
{
	objdeath(objp);
	bfree (objp->looks);

	score += GABVALUE;
	disp_score ();

	CLEAR_ATT (*killerp, EXISTS);
	put_object (*killerp);
}

Gstrlen(str)
char	*str;
{
	int	len = 0;

	while (str[len] != '\0')
		len++;
	return (len);
}
