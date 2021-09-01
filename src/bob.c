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

i4	bob_bits[]={
0x0003F7BE,0x80000000,
0x000DA7F7,0xE0000000,
0x001CFFDE,0x50000000,
0x00376EEB,0xD0000000,
0x00FD676B,0x50000000,
0x00FF525B,0x5C000000,
0x01EF5FDA,0xFE000000,
0x01EF9BF3,0x92000000,

0x01FC63CC,0x1F000000,
0x01F037F8,0x0F000000,
0x01201830,0x09000000,
0x01E007E0,0x0B000000,
0x01400000,0x0E000000,
0x0143E007,0xCA000000,
0x01C67008,0xCA000000,
0x01400000,0x0E000000,

0x0143644D,0x82000000,
0x0146B45A,0xC2000000,
0x0143644D,0x82000000,
0x01C00440,0x02000000,
0x01800440,0x02000000,
0x00800440,0x07000000,
0x01400440,0x05000000,
0x01400C60,0x05000000,

0x01400C20,0x05000000,
0x01400420,0x05000000,
0x00C00C30,0x05000000,
0x00601810,0x06000000,
0x00200FF0,0x04000000,
0x00300000,0x0C000000,
0x00100000,0x08000000,
0x00100000,0x18000000,

0x0018301E,0x10000000,
0x00084FE2,0x30000000,
0x00084004,0x20000000,
0x000C7FFC,0x20000000,
0x0004F010,0x60000000,
0x0005CFE0,0x40000000,
0x00038000,0x40000000,
0x38070000,0x40000000,

0x6C0F0180,0x80000000,
0xC61D8001,0x80000000,
0x8F38C003,0x00000000,
0xDF7063C6,0x00000000,
0x7FE03FFC,0x00000000,
0x3FC01C38,0x00000000,
0x1F800000,0x00000000,
0x07000000,0x00000000,
};

Bitmap	boblooks = { /* For displaying	J.R. "Bob" Dobbs	*/
	(Word*) bob_bits,
	2 * BLTWIDTH,
	{ { 0, 0 }, { 40, 48 } }
};

struct	object	bob = { {{0, 0}, {0, 0}},
			   {{0, 0}, {0, 0}}, 0, 0, 0, BOB, BOBVALUE,
			bobandeddiechoose, &boblooks};

struct	object	bobdebris[DEBMAX] = {
	{{0, 0}, {0, 0}},
	{{0, 0}, {0, 0}}, -10, -10, 0, 0, 0, debchoose, 0, 0,
	{{0, 0}, {0, 0}},
	{{0, 0}, {0, 0}}, 4, -12, 0, 0, 0, debchoose, 0, 0,
	{{0, 0}, {0, 0}},
	{{0, 0}, {0, 0}}, 12, -2, 0, 0, 0, debchoose, 0, 0,
	{{0, 0}, {0, 0}},
	{{0, 0}, {0, 0}}, 8, 11, 0, 0, 0, debchoose, 0, 0,
	{{0, 0}, {0, 0}},
	{{0, 0}, {0, 0}}, -3, 8, 0, 0, 0, debchoose, 0, 0
};

bobstart ()
{
	Point	poynt;
	
/*	if (bobspeed < me.looks->rect.corner.y)
		bobspeed++;
*/
	/* bob.value is bob's absolute velocity	*/
	bob.value = score / 100000 + 2;
	poynt.x = urnd (playerrect.origin.x,
			playerrect.corner.x - bob.looks->rect.corner.x);
	poynt.y = playerrect.corner.y - bob.looks->rect.corner.y;
	bob.rect = raddp (bob.looks->rect, poynt);
	
	/* bob will start moving soon enough (The first call to
	 *	bobandeddiechoose actually.
	 */
	bob.dydt = 0;
	bob.dxdt = 0;
	objbirth (&bob);
}

/*
 * 'bobandeddiechoose' decides the direction of movement for both
 *	bob and eddie.
 */
bobandeddiechoose (objp)
struct	object	*objp;
{
	int	bead;

	bead = atan3(div2(me.rect.origin.y + me.rect.corner.y) -
		     div2(objp->rect.origin.y + objp->rect.corner.y),
		     div2(me.rect.origin.x + me.rect.corner.x) -
		     div2(objp->rect.origin.x + objp->rect.corner.x));
	
	objp->dydt = muldiv (cos(bead), objp->value, 1024);
	objp->dxdt = muldiv (sin(bead), objp->value, 1024);
	
	objp->next = raddp (objp->rect, Pt(objp->dxdt, objp->dydt));
/*	I don't think this code is necessary
 *		if (!(isallin (objp->next, playerrect))) {
 *		CLEAR_ATT (*objp, EXISTS);
 *		put_object (*objp);
 *		if (objp == &eddie)
 *			eddiestart();
 *		else
 *			bobstart();
 *		return (FALSE);
 *	}
 */
	return (TRUE);
}

bob_collision()
{
	int	i;

	if (IS_SET (mtv, EXISTS)) {
		for (i = 0; i < notecnt; i++)
			if (IS_SET (note[i], EXISTS) &&
			   (rectXrect (bob.rect, note[i].rect))) {
				objdeath(&note[i]);
				notestart();
			}
	}
	if (IS_SET (eddie, EXISTS)) {
		for (i = 0; i < GABMAX; i++)
			if (IS_SET (gab[i], EXISTS) &&
			   (rectXrect (bob.rect, gab[i].rect))) {
				objdeath(&gab[i]);
				bfree (gab[i].looks);
			}
	}
	for (i = 0; i < DEBMAX; i++) {
		if (IS_SET (mtvdebris[i], EXISTS) &&
		    rectXrect (bob.rect, mtvdebris[i].rect)) {
			killbob(&(mtvdebris[i]));
			return;
		}
		if (IS_SET (eddiedebris[i], EXISTS) &&
		    rectXrect (bob.rect, eddiedebris[i].rect)) {
			killbob(&(eddiedebris[i]));
			return;
		}
	}
}

/* 'killbob' removes the bob from the screen (for the time being).
 */
killbob (killerp)
struct object	*killerp;
{
	objdeath (&bob);
	explode_object (&bob, bobdebris);
	if ((killerp == &shot) || (killerp == &(exshot[0]))) {
		score -= BOBVALUE;
		CLEAR_ATT (*killerp, EXISTS);
		put_object (*killerp);
	}
	else {
		score += BOBVALUE * 2;
		smurfstart ();
		objdeath (killerp);
	}

	disp_score ();
}

