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

i4	ball_bits[] = {
0x00007F80,0x00000000,
0x00000000,0x00000000,
0x0007FFF8,0x00000000,
0x001FFFFE,0x00000000,
0x00000000,0x00000000,
0x00601FFF,0x80000000,
0x01FFFFFF,0xE0000000,
0x00000000,0x00000000,

0x030001FF,0xF0000000,
0x07FFFFFF,0xF8000000,
0x00000000,0x00000000,
0x0C00007F,0xFC000000,
0x1FFFFFFF,0xFE000000,
0x00000000,0x00000000,
0x3800003F,0xFF000000,
0x7FFFFFFF,0xFF800000,

0x00000000,0x00000000,
0x7800003F,0xFF800000,
0xFFFFFFFF,0xFFC00000,
0x00000000,0x00000000,
0xFC00007F,0xFFC00000,
0xFFFFFFFF,0xFFC00000,
0x00000000,0x00000000,
0xFE0001FF,0xFFC00000,

0x7FFFFFFF,0xFF800000,
0x00000000,0x00000000,
0x7FC01FFF,0xFF800000,
0x3FFFFFFF,0xFF000000,
0x00000000,0x00000000,
0x1FFFFFFF,0xFE000000,
0x0FFFFFFF,0xFC000000,
0x00000000,0x00000000,

0x07FFFFFF,0xF8000000,
0x03FFFFFF,0xF0000000,
0x00000000,0x00000000,
0x01FFFFFF,0xE0000000,
0x007FFFFF,0x80000000,
0x00000000,0x00000000,
0x001FFFFE,0x00000000,
0x0007FFF8,0x00000000,

0x00000000,0x00000000,
0x00007F80,0x00000000,
};

Bitmap	balllooks = { /* For displaying American Ball	*/
	(Word*) ball_bits,
	2 * BLTWIDTH,
	{ { 0, 0 }, { 41, 42 } }
};

i4	bell_bits[] = {
0x0001FFC0,0x00000000,
0x0007FFF0,0x00000000,
0x001FFFFC,0x00000000,
0x007FFFFF,0x00000000,
0x00FE003F,0x80000000,
0x01F8000F,0xC0000000,
0x03F00007,0xE0000000,
0x07E00003,0xF0000000,

0x0FC03E01,0xF8000000,
0x0F803E00,0xF8000000,
0x1F003E00,0x7C000000,
0x1E03FFE0,0x3C000000,
0x3C07FFF0,0x1E000000,
0x3807FFF0,0x0E000000,
0x780F0078,0x0F000000,
0x780E0038,0x0F000000,

0xF00E0038,0x07800000,
0xF00E0038,0x07800000,
0xF00E0038,0x07800000,
0xF01E003C,0x07800000,
0xF01C001C,0x07800000,
0xF03C001E,0x07800000,
0xF038000E,0x07800000,
0xF0F00007,0x87800000,

0xF0E00003,0x87800000,
0x78E00003,0x8F000000,
0x78FFFFFF,0x8F000000,
0x38FFFFFF,0x8E000000,
0x3CFFFFFF,0x9E000000,
0x1E003E00,0x3C000000,
0x1F003E00,0x7C000000,
0x0F803E00,0xF8000000,

0x0FC00001,0xF8000000,
0x07E00003,0xF0000000,
0x03F00007,0xE0000000,
0x01F8000F,0xC0000000,
0x00FE003F,0x80000000,
0x007FFFFF,0x00000000,
0x001FFFFC,0x00000000,
0x0007FFF0,0x00000000,

0x0001FFC0,0x00000000,
};

Bitmap	belllooks = { /* For displaying American Ball	*/
	(Word*) bell_bits,
	2 * BLTWIDTH,
	{ { 0, 0 }, { 41, 41 } }
};

struct	object	bll[BLLMAX];

bllstart(stpt)
Point	stpt;
{
	int	startnum = 2;
	int	i, j;
	int	angle;
	
	if (rand() % 5 == 0)
		startnum = 3;
		
	for (i = 0; i < startnum; i++) {
		for (j = 0; j < BLLMAX; j++)
			if (!IS_SET(bll[j], EXISTS))
				break;
		if (j >= BLLMAX)
			return;
		
/*		put_object (bll[j]);
		SET_ATT (bll[j], EXISTS);
*/
		bll[j].type = 0;
		if (i == 0)
			if (rand() % 3 == 0)
				bll[j].type = BALL;
			else
				bll[j].type = BELL;
		else
			if (rand() % 5 == 0)
				bll[j].type = BELL;
			else
				bll[j].type = BALL;
				
		if (bll[j].type == BALL)
			bll[j].looks = &balllooks;
		else
			bll[j].looks = &belllooks;

		bll[j].rect = raddp (bll[j].looks->rect, stpt);

		angle = urnd(2, 178);
		bll[j].value = urnd (2, notecnt * 2);

		bll[j].dydt = muldiv (sin(angle), bll[j].value, 1024);
		bll[j].dxdt = muldiv (cos(angle), bll[j].value, 1024);

		bll[j].fuse = urnd (1, notecnt);
		objbirth (&bll[j]);
	}
}

# define RIGHT	0
# define LEFT	1
# define TOP	2
# define BOTTOM	3

bllchoose (objp)
struct	object	*objp;
{
	if (!(isallin(raddp(objp->rect, Pt(0, objp->dydt)), playerrect))) {
		objp->dydt = -objp->dydt;
		if (objp->dydt > 0)
			adjustvel (objp, TOP);
		else
			adjustvel (objp, BOTTOM);
	}

	if (!(isallin(raddp(objp->rect, Pt(objp->dxdt, 0)), playerrect))) {
		objp->dxdt = -objp->dxdt;
		if (objp->dxdt > 0)
			adjustvel (objp, LEFT);
		else
			adjustvel (objp, RIGHT);
	}

	objp->next = raddp (objp->rect, Pt(objp->dxdt, objp->dydt));
	
	return (TRUE);
}

adjustvel(objp, which_wall)
struct object	*objp;
int	which_wall;
{
	int	curdir,
		medir,
		newdir;
		
	curdir = atan3(objp->dxdt, objp->dydt);
	medir = atan3(div2(me.rect.origin.x + me.rect.corner.x) -
		     div2(objp->rect.origin.x + objp->rect.corner.x),
		     div2(me.rect.origin.y + me.rect.corner.y) -
		     div2(objp->rect.origin.y + objp->rect.corner.y));
	
	if (medir == 0)
		medir = 360;
	if (curdir == 0)
		curdir = 360;
	if (medir > curdir) {
		if (medir - curdir > 180)
			curdir += 360;
	}
	else if (curdir - medir > 180)
		medir += 360;
		
	newdir = (long) curdir * 20000L / (long)(20000L + score) +
		 (long) medir * score / (long)(20000L + score);
	
	objp->dydt = muldiv (sin(newdir), objp->value, 1024);
	objp->dxdt = muldiv (cos(newdir), objp->value, 1024);

	/* These conditions happen because the player is a little
	 *	smaller than a ball or bell.
	 */
	if ((which_wall == RIGHT && objp->dxdt > 0) ||
	    (which_wall == LEFT && objp->dxdt < 0))
		objp->dxdt = -objp->dxdt;
	if ((which_wall == TOP && objp->dydt < 0) ||
	    (which_wall == BOTTOM && objp->dydt > 0))
		objp->dydt = -objp->dydt;
}

/* 'bll_collision' checks to see if the bll bumped into anything.
 */
bll_collision()
{
}

/* 'killbll' removes the bll from the screen (for the time being).
 */
killbll (killerp, objp)
struct object	*killerp, *objp;
{
	int	i, bllcnt = 0;
	
	for (i = 0; i < BLLMAX; i++)
		if (IS_SET(bll[i], EXISTS))
			bllcnt++;
	
	CLEAR_ATT (*killerp, EXISTS);
	put_object (*killerp);

	if (objp->fuse-- > 0)
		return;
		
	objdeath (objp);

	if (objp->type == BALL) {
		score += BLLVALUE * bllcnt;
		disp_score ();

		if (bllcnt <= 1) {
			bll[0].fuse = BLLGONE;
		}
	}
	else {
		bllstart(objp->rect.origin);
	}
}
