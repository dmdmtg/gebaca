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

i4	ibm_bits[] = {
0xFFF3FFE1,0xF007C000,
0xFFF3FFF1,0xF80FC000,
0x00000000,0x00000000,
0x00000000,0x00000000,
0xFFF3FFF9,0xFC1FC000,
0xFFF3FFF9,0xFC1FC000,
0x00000000,0x00000000,
0x00000000,0x00000000,
0x0F00F0F8,0x7E3F0000,
0x0F00F0F0,0x7F7F0000,
0x00000000,0x00000000,
0x00000000,0x00000000,
0x0F00FFF0,0x7FFF0000,
0x0F00FFE0,0x7FFF0000,
0x00000000,0x00000000,
0x00000000,0x00000000,
0x0F00FFE0,0x7FFF0000,
0x0F00FFF0,0x7FFF0000,
0x00000000,0x00000000,
0x00000000,0x00000000,
0x0F00F0F0,0x77F70000,
0x0F00F0F8,0x73E70000,
0x00000000,0x00000000,
0x00000000,0x00000000,
0xFFF3FFF9,0xF1C7C000,
0xFFF3FFF9,0xF087C000,
0x00000000,0x00000000,
0x00000000,0x00000000,
0xFFF3FFF1,0xF007C000,
0xFFF3FFE1,0xF007C000,
};

Bitmap	ibmlooks = { /* For displaying American Ball	*/
	(Word*) ibm_bits,
	2 * BLTWIDTH,
	{ { 0, 0 }, { 50, 30 } }
};

struct	icon	I = {
	{ {{0, 0}, {0, 0}}, 
	  {{0, 0}, {0, 0}}, 0, 0, 0, III, IBMVALUE, ibmchoose, &ibmlooks},
	{{0, 0}, {12, 30}}
};

struct	icon	B = {
	{ {{0, 0}, {0, 0}},
	  {{0, 0}, {0, 0}}, 0, 0, 0, BBB, IBMVALUE, ibmchoose, &ibmlooks},
	{{14, 0}, {29, 30}}
};

struct	icon	M = {
	{ {{0, 0}, {0, 0}},
	  {{0, 0}, {0, 0}}, 0, 0, 0, MMM, IBMVALUE, ibmchoose, &ibmlooks},
	{{31, 0}, {50, 30}}
};

struct	icon	ibm = {
	{ {{0, 0}, {0, 0}},
	  {{0, 0}, {0, 0}}, 0, 0, 0, IBM, IBMVALUE, ibmchoose, &ibmlooks},
	{{0, 0}, {50, 30}}
};

ibmstart()
{
	Point	poynt;

	SET_ATT (ibm.obj, EXISTS);
	poynt.x = urnd (Drect.origin.x,
			Drect.corner.x - (ibm.irect.corner.x + 1));
	poynt.y = Drect.origin.y;
	
	ibm.obj.rect = raddp (ibm.irect, poynt);
	ibm.obj.dydt = urnd (4, 8);
	ibm.obj.dxdt = 12 - ibm.obj.dydt;

	put_icon (ibm);
}

ibmchoose (iconp)
struct	icon	*iconp;
{
	struct	object	*objp = &(iconp->obj);

	if (!(isallin(raddp(objp->rect, Pt(0, objp->dydt)), playerrect))) {
		put_icon (*iconp);
		
		if (objp->dydt > 0) {
			objp->rect.origin.y = Drect.origin.y;
		}
		else {
			objp->rect.origin.y = Drect.corner.y -
			   (iconp->irect.corner.y - iconp->irect.origin.y);
		}
		objp->rect.corner.y = objp->rect.origin.y +
					iconp->irect.corner.y -
					iconp->irect.origin.y;
		put_icon (*iconp);
	}
	
	if (!(isallin(raddp(objp->rect, Pt(objp->dxdt, 0)), playerrect)))
		objp->dxdt = -objp->dxdt;

	objp->next = raddp (objp->rect, Pt(objp->dxdt, objp->dydt));
	
	return (TRUE);
}

/* 'killibm' removes the ibm from the screen (for the time being).
 */
killibm (killerp)
struct	object	*killerp;
{
	int	range;
	
	CLEAR_ATT (*killerp, EXISTS);
	put_object (*killerp);
	CLEAR_ATT (ibm.obj, EXISTS);
	
	/* Start individual I, B and M	*/
	SET_ATT (I.obj, EXISTS);
	SET_ATT (B.obj, EXISTS);
	SET_ATT (M.obj, EXISTS);
	
	I.obj.rect = raddp (I.irect, ibm.obj.rect.origin);
	B.obj.rect = raddp (B.irect, ibm.obj.rect.origin);
	M.obj.rect = raddp (M.irect, ibm.obj.rect.origin);
	
	range = 3 + notecnt;
	I.obj.dxdt = urnd (-range, range);
	I.obj.dydt = urnd (-range, range);
	B.obj.dxdt = ibm.obj.dxdt;
	B.obj.dydt = ibm.obj.dydt;
	M.obj.dxdt = urnd (-range, range);
	M.obj.dydt = urnd (-range, range);
}

killIBM (killerp, iconp)
struct	object	*killerp;
struct	icon 	*iconp;
{
	CLEAR_ATT (iconp->obj, EXISTS);
	put_icon (*iconp);
	CLEAR_ATT (*killerp, EXISTS);
	put_object (*killerp);

	score += IBMVALUE;
	disp_score();

	
	if (!IS_SET(I.obj, EXISTS) &&
	    !IS_SET(B.obj, EXISTS) &&
	    !IS_SET(M.obj, EXISTS))
		ibm.obj.fuse = IBMGONE;
}
