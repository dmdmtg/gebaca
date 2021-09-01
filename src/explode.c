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

explode_object(objp, debrisp)
struct object	*objp,
		debrisp[];
{
	int	i;
	
	for (i = 0; i < DEBMAX; i++) {
		debrisp[i].rect = objp->rect;

/*		debrisp[i].rect.corner.x = min (objp->rect.origin.x + 
						chunkbounds[i].corner.x,
						objp->rect.corner.x);
		debrisp[i].rect.corner.y = min (objp->rect.origin.y + 
						chunkbounds[i].corner.y,
						objp->rect.corner.y);
*/		debrisp[i].value = urnd (DECAYTIME - 5, DECAYTIME + 5);
		objbirth(&debrisp[i]);
	}
	SET_ATT (*objp, EXPLODING);
}

debchoose(objp)
struct object	*objp;
{
	objp->next = raddp (objp->rect, Pt(objp->dxdt, objp->dydt));
	if (!(isallin(objp->rect, playerrect)) || objp->value-- <= 0) {
		objdeath(objp);
		return (FALSE);
	}
	return (TRUE);
}
