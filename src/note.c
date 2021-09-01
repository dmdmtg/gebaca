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

i4	note_bits[]={
	0x01000000,
	0x03000000,
	0x06000000,
	0x0C000000,
	0x18000000,
	0x30000000,
	0x60000000,
	0x60000000,

	0x30000000,
	0x18000000,
	0x0C000000,
	0x06000000,
	0x03000000,
	0x03000000,
	0x06000000,
	0x0C000000,

	0x18000000,
	0x30000000,
	0x18000000,
	0x0C000000,
	0x06000000,
	0x03000000,
	0x39000000,
	0x7F000000,

	0xFF000000,
	0xFF000000,
	0xFF000000,
	0xFF000000,
	0x7E000000,
	0x3C000000,
};

Bitmap	notelooks = { /* For displaying	notes coming from mtv	*/
	(Word*) note_bits,
	BLTWIDTH,
	{ { 0, 0 }, { 8, 30 } }
};


struct	object	note[NOTEMAX];

notestart ()
{
	int	anytheta;
	int	i, j;
	Point	poynt;
	int	bead;
	
	for (i = 0; i < notecnt; i++)
		if (!IS_SET(note[i], EXISTS))
			break;
	if (i >= notecnt)
		return;
		
	poynt = sub (div (add (mtv.rect.corner, mtv.rect.origin), 2),
		     div (note[i].looks->rect.corner, 2));
	note[i].rect = raddp (note[i].looks->rect, poynt);
	
	bead = atan3(div2(me.rect.origin.y + me.rect.corner.y) -
		     div2(mtv.rect.origin.y + mtv.rect.corner.y),
		     div2(me.rect.origin.x + me.rect.corner.x) -
		     div2(mtv.rect.origin.x + mtv.rect.corner.x));
	anytheta = urnd (bead - thetarange, bead + thetarange);
	
	note[i].dydt = muldiv (cos(anytheta), notespeed, 1024);
	note[i].dxdt = muldiv (sin(anytheta), notespeed, 1024);
	

	/* Prevent overlapping notes	*/
	for (j = 0; j < notecnt; j++) {
		if (IS_SET (note[j], EXISTS) &&
		    (note[j].rect.origin.x == note[i].rect.origin.x) &&
		    (note[j].rect.origin.y == note[i].rect.origin.y) &&
		    (note[j].dxdt == note[i].dxdt) &&
		    (note[j].dydt == note[i].dydt)) {
			anytheta = urnd (bead - 20, bead + 20);
			note[i].rect = raddp (note[i].rect,
					      Pt(urnd (-2, 2), 0));
			note[i].dydt = muldiv (cos(anytheta), notespeed, 1024);
			note[i].dxdt = muldiv (sin(anytheta), notespeed, 1024);
			j = -1;
		}
	}
	
	objbirth(&note[i]);
}

/* 
 * 'notechoose'
 */
notechoose (objp)
struct	object	*objp;
{
	objp->next = raddp (objp->rect, Pt(objp->dxdt, objp->dydt));
	if (!(ptinrect (objp->next.origin, playerrect))) {
		objdeath(objp);
		if (IS_SET(mtv, EXISTS))
			notestart();
		return (FALSE);
	}
	return (TRUE);
}

