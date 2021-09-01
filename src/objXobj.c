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

/* This routine returns TRUE if the two objects given as arguments
 *	really really overlap. The object soverlap if they have
 *	ON bits on the same spot on the screen.
 *	Note: This doesn't work yet with icon types
 */

objXobj(objp1, objp2)
struct	object	*objp1, *objp2;
{
	int	i1, i2, j1, j2,
		si1 = 0,
		si2 = 0,
		ei1 = objp1->looks->rect.corner.x,
		sj1 = 0,
		sj2 = 0,
		ej1 = objp1->looks->rect.corner.y;
		
	if (objp1->rect.origin.x < objp2->rect.origin.x)
		si1 = objp2->rect.origin.x - objp1->rect.origin.x;
	else
		si2 = objp1->rect.origin.x - objp2->rect.origin.x;

	if (objp1->rect.corner.x > objp2->rect.corner.x)
		ei1 = objp2->rect.corner.x - objp1->rect.origin.x;

	if (objp1->rect.origin.y < objp2->rect.origin.y)
		sj1 = objp2->rect.origin.y - objp1->rect.origin.y;
	else
		sj2 = objp1->rect.origin.y - objp2->rect.origin.y;

	if (objp1->rect.corner.y > objp2->rect.corner.y)
		ej1 = objp2->rect.corner.y - objp1->rect.origin.y;

	for (i1 = si1, i2 = si2; i1 < ei1; i1++, i2++)
		for (j1 = sj1, j2 = sj2; j1 < ej1; j1++, j2++)
			if (is_on (objp1->looks, Pt(i1, j1)) &&
			    is_on (objp2->looks, Pt(i2, j2)))
				return (TRUE);
				
	return (FALSE);
}

/* 'is_on' return TRUE iff the corres. bit on the display is on.
 */
int	is_on (bmap, pnt)
Bitmap	*bmap;
Point	pnt;
{
	register Word	*bitword;
	
	bitword = addr(bmap, pnt);
	return (*bitword & (1 << ((WORDSIZE-1) - (pnt.x & WORDMASK))));
}

