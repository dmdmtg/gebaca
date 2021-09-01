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

i4	eddie_bits[] = {
0x00003FFC,0x00000000,
0x0000E007,0x00000000,
0x00018FFB,0x00000000,
0x0001FFFF,0x00000000,
0x0003FFFF,0x80000000,
0x00038141,0xC0000000,
0x0007E333,0xE0000000,
0x6305FFBF,0xA0C60000,

0x3305FE3F,0xA0CC0000,
0xDB05FFFF,0xA0DB0000,
0x6F07F801,0xC0F60000,
0xBF01E003,0x80FD0000,
0xFF01F003,0x80FF0000,
0x3F81F007,0x01FC0000,
0x0E80FC3F,0x01700000,
0x06607FFC,0x02600000,

0x01100FF0,0x1C800000,
0x00CE0FF0,0x61000000,
0x0021FFFF,0x86000000,
0x0010CFF6,0x08000000,
0x000867E6,0x10000000,
0x0004300C,0x20000000,
0x00039818,0x60000000,
0x00009FF8,0xC0000000,

0x00008000,0x80000000,
0x0000C000,0x80000000,
0x00008000,0xC0000000,
0x00008000,0x40000000,
0x00008000,0xC0000000,
0x00008000,0x80000000,
0x0000C001,0x00000000,
0x00004001,0x00000000,

0x00004001,0x00000000,
0x0000C001,0x00000000,
0x0000F801,0x00000000,
0x0000077F,0x00000000,
};

Bitmap	eddielooks = { /* For displaying eddie	*/
	(Word*) eddie_bits,
	2 * BLTWIDTH,
	{ { 0, 0 }, { 48, 36 } }
};

struct	object	eddie = { {{0, 0}, {0, 0}},
			   {{0, 0}, {0, 0}}, 0, 0, 0, EDDIE, EDDIEVALUE,
			bobandeddiechoose, &eddielooks};

struct	object	eddiedebris[DEBMAX] = {
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

eddiestart()
{
	Point	poynt;
	
	poynt.x = Drect.corner.x - eddie.looks->rect.corner.x;
	poynt.y = div2 (Drect.corner.y - Drect.origin.y);
	
	eddie.rect = raddp (eddie.looks->rect, poynt);

	/* eddie.value is his absolute velocity	*/
	eddie.value = (score > 30000 ? score/10000 : 3);

	objbirth(&eddie);

	gabp = 0; /* Restart banter	*/
	gabstart();
}

/* 'killeddie' removes the eddie from the screen (for the time being).
 */
killeddie (killerp)
struct	object	*killerp;
{
	int	i;
	
	objdeath(&eddie);
	explode_object (&eddie, eddiedebris);
	eddie.fuse = EDDIEGONE;
	score += EDDIEVALUE;
	disp_score ();

	CLEAR_ATT (*killerp, EXISTS);
	put_object (*killerp);
		
	for (i = 0; i < GABMAX; i++) {
		if (IS_SET (gab[i], EXISTS)) {
			objdeath(&gab[i]);
			bfree (gab[i].looks);
		}
	}
}

