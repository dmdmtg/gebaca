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

i4	smurf_bits[]={
0x001FFC00,0x00000000,
0x00180700,0x00000000,
0x0033F980,0x00000000,
0x0027FC80,0x00000000,
0x0027FC80,0x00000000,
0x0067FC80,0x00000000,
0x0063F880,0x00000000,
0x00700180,0x00000000,

0x00F00180,0x00000000,
0x00FFFFC0,0x00000000,
0x03FFFFE0,0x00000000,
0x03FFFFE0,0x00000000,
0x07FFFFE0,0x00000000,
0x07FFFFF0,0x00000000,
0x07000030,0x00000000,
0x0C000018,0x00000000,

0x3DFFFFED,0x80000000,
0x7FFFFFFD,0xC0000000,
0xC980001E,0x60000000,
0x8F000006,0x20000000,
0x841F1F0C,0x20000000,
0x9A3FBF9B,0x20000000,
0xD27FBFC9,0x60000000,
0x5678A3CD,0x40000000,

0x5478A3CD,0x40000000,
0x4478A3C4,0x40000000,
0x643FBF84,0xC0000000,
0x34000005,0x80000000,
0x1C03F807,0x00000000,
0x0C860C4E,0x00000000,
0x05840468,0x00000000,
0x04C60CC8,0x00000000,

0x04C3F8C8,0x00000000,
0x06600198,0x00000000,
0x02300310,0x00000000,
0x031C0E30,0x00000000,
0x0187F860,0x00000000,
0x00C000C0,0x00000000,
0x00700380,0x00000000,
0x001FFE00,0x00000000,
};

Bitmap	smurflooks = { /* For displaying smurfs	*/
	(Word*) smurf_bits,
	2 * BLTWIDTH,
	{ { 0, 0 }, { 35, 40 } }
};

struct	object	smurf = { {{0, 0}, {0, 0}},
			   {{0, 0}, {0, 0}}, 0, 0, 0, SMURF, SMURFVALUE,
			smurfchoose, &smurflooks};

struct	object	smurfdebris[DEBMAX] = {
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

smurfstart ()
{
	Point	poynt;
	
	poynt = div (add (Drect.origin, Drect.corner), 2);
	smurf.rect = raddp (smurf.looks->rect, poynt);
	
	smurf.fuse = SMURFLIFE;
	
	/* smurfs are not treated like enemy objects and are therefore
	 *	entered into the linked object list.
	 */
	put_object (smurf);
	SET_ATT (smurf, EXISTS);
}

/* 
 * 'smurfchoose'
 */
smurfchoose (objp)
struct	object	*objp;
{
	return (FALSE);
}

/* 'killsmurf' removes the smurf from the screen (for the time being).
 */
killsmurf (killerp)
struct object	*killerp;
{
	CLEAR_ATT (smurf, EXISTS);
	put_object (smurf);
	explode_object (&smurf, smurfdebris);

	if ((killerp == &shot) || (killerp == &(exshot[0]))) {
		score += SMURFVALUE;
		CLEAR_ATT (*killerp, EXISTS);
		put_object (*killerp);
	}
	else {
		score += 10 * SMURFVALUE;
	}
	disp_score ();
}
