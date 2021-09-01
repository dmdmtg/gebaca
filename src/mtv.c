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

i4	mtv_bits[] = {
0xFFF80007,0xFF800000,
0xFFFC000F,0xFF800000,
0xC00E001C,0x01800000,
0xC0070038,0x01800000,
0xC0038070,0x01800000,
0xC001C0E0,0x01800000,
0xC000E1C0,0x01800000,
0xC0007380,0x01800000,

0xC0003F00,0x01800000,
0xC0001E00,0x01800000,
0xC0000C00,0x01800000,
0xC0000000,0x01800000,
0xC0000000,0x01800000,
0xC0000000,0x01800000,
0xC0000180,0x01800000,
0xC00001C0,0x01800000,

0xC00003F0,0x01800000,
0xC00003FE,0x01800000,
0xC00003FF,0x81800000,
0xC000000F,0xF1800000,
0xC000000E,0xF9800000,
0xC003003C,0x7D800000,
0xC003807C,0x3F800000,
0xC003C0FC,0x1D800000,

0xC003E1F8,0x0D800000,
0xC00373BC,0x01800000,
0xC0033F78,0x01800000,
0xC0031E78,0x01800000,
0xC0030CF0,0x71800000,
0xC00300F0,0x71800000,
0xC00300F0,0x7180C000,
0xC00300F0,0x7980C000,

0xC00301F0,0x7981C000,
0xC00303F0,0x39838000,
0xC00303F0,0x39878000,
0xC00307F0,0x398F8000,
0xC00307F0,0x398F0000,
0xC00307B0,0x399F0000,
0xC0030FB0,0x3DBE0000,
0xC0030F30,0x3DFC0000,


0xC0030F30,0x3DFC0000,
0xC0030E30,0x1DF80000,
0xC0030430,0x1FF00000,
0xC0030030,0x0FE00000,
0xC0030030,0x0FC00000,
0xC0030030,0x0F800000,
0xFFFF003F,0xFF800000,
0xFFFF003F,0xFF800000,
};

Bitmap	mtvlooks = { /* For displaying mtv	*/
	(Word*) mtv_bits,
	2 * BLTWIDTH,
	{ { 0, 0 }, { 48, 48 } }
};

struct	object	mtv = { {{0, 0}, {0, 0}},
			   {{0, 0}, {0, 0}}, 0, 0, 0, MTV, 0,
			mtvchoose, &mtvlooks};

struct	object	mtvdebris[DEBMAX] = {
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

mtvstart()
{
	Point	poynt;
	int	i;
	
	if (mtvbuf[0] != '\0') { /* clear old score display	*/
		string (&defont, mtvbuf, &display, mtv.rect.origin, F_XOR);
		mtvbuf[0] = '\0';
	}
	
	poynt.x = div2 (Drect.corner.x + Drect.origin.x - mtv.looks->rect.corner.x);
	poynt.y = Drect.origin.y;
	
	mtv.rect = raddp (mtv.looks->rect, poynt);
	mtv.dydt = MTVSPEED;

	/* Use one or the other or both of these meany factors	*/
/*	if (score > bumpmtvat) {
		healrate = max (1, healrate - 1);
		bumpmtvat += 25000;
	}
*/	if (notecnt == NOTEMAX)
		notespeed = min (notespeed + 1, 12);
	else if (notecnt < NOTEMAX)
		notecnt++;

	curnotes = notecnt;
	thetarange = 180;
	
	objbirth (&mtv);
	for (i = 0; i < notecnt; i++)
		notestart();
}

mtvchoose (objp)
struct	object	*objp;
{
	mtv.next = raddp (mtv.rect, Pt(0, mtv.dydt));
	if (!(isallin(mtv.next, playerrect))) {
		objdeath(&mtv);
		curnotes--;
		mtv.fuse = 0;
		return (FALSE);
	}

	
	return (TRUE);
}

/* 'mtv_collision' checks to see if the mtv bumped into anything.
 */
mtv_collision()
{
}

/* 'killmtv' removes the mtv from the screen (for the time being).
 */

killmtv (killerp)
struct	object	*killerp;
{
	int	i,
		tempscore;
	
	if (curnotes == 0) {
		objdeath(&mtv);
		explode_object (&mtv, mtvdebris);
		mtv.fuse = MTVGONE;
		mtvscore += 150;

		score += mtvscore;
		disp_score ();

		tempscore = mtvscore;
		mtvbuf[5] = '\0';
		for (i = 4; i >= 0; i--) {
			if (tempscore > 0) {
				mtvbuf[i] = tempscore % 10 + '0';
				tempscore /= 10;
			}
			else {
				mtvbuf[i] = ' ';
			}
		}
		string (&defont, mtvbuf, &display, mtv.rect.origin, F_XOR);
	}
	else {
		curnotes--;
		for (i = notecnt - 1; i >= 0; i--)
			if (IS_SET(note[i], EXISTS))
				break;
		if (i < 0)
			error ("Bad notecnt");
		objdeath (&note[i]);
		thetarange = max (1, div2(thetarange));
		if (killerp == &shot) {
			score += MTVVALUE;
			disp_score ();
		}
	}
	CLEAR_ATT (*killerp, EXISTS);
	put_object (*killerp);

}

