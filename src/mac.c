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

i4	artop_bits[]={
0x01E00F00,
0x07F83FC0,
0x07F83FC0,
0x0F3C79E0,
0x0E1C70E0,
0x1C0EE070,
0x1C0FE070,
0x3C0FE078,

0x3807C038,
0x3807C038,
0x7807C03C,
0x7807C03C,
0x7807C03C,
0x7003801C,
0x7003801C,
0x7003801C,

0x7003801C
};
Bitmap	artoplooks = { /* For displaying top of macdonalds arch	*/
	(Word*) artop_bits,
	BLTWIDTH,
	{ { 0, 0 }, { 31, 17 } }
};

i4	arbot_bits[]={
0xE003800E,
0xE003800E,
0xE003800E,
0xE003800E,
0xE003800E,
0xE003800E,
0xE003800E,
0xE000000E,

0xE000000E,
0xE000000E
};

Bitmap	arbotlooks = { /* For displaying bottom of macdonalds arch	*/
	(Word*) arbot_bits,
	BLTWIDTH,
	{ { 0, 0 }, { 31, 10 } }
};

Rectangle	macbounds = { {0, 0}, {103, 43} };
struct	object	mac = { {{0, 0}, {0, 0}},
			   {{0, 0}, {0, 0}}, 0, 0, 0, MAC, 0,
			0, 0};

char	macbuf[6];

/* 'macinit' is called once when gebaca is first downloaded	*/
macinit()
{
	Point	poynt;
	
	mac.looks = (Bitmap *) balloc (macbounds);
	if (mac.looks == (Bitmap *) NULL)
		error ("Can't allocate Macbitmap");
	mac.looks->rect = macbounds;
	rectf (mac.looks, mac.looks->rect, F_CLR);
	poynt.x = Drect.origin.x + 20;
	poynt.y = div2 (Drect.origin.y + Drect.corner.y) - 20;
	mac.rect = raddp (macbounds, poynt);
	bitblt(&artoplooks, artoplooks.rect, mac.looks, Pt(36, 0), F_STORE);
	bitblt(&arbotlooks, arbotlooks.rect, mac.looks, Pt(36, 33), F_STORE);
	string (&defont, "served", mac.looks, Pt (47, 19), F_STORE);
	segment (mac.looks, Pt (0, 17), Pt (102, 17), F_STORE);
	segment (mac.looks, Pt (102, 17), Pt (102, 32), F_STORE);
	segment (mac.looks, Pt (102, 32), Pt (0, 32), F_STORE);
	segment (mac.looks, Pt (0, 32), Pt (0, 17), F_STORE);
	macbuf[0] = '\0';
	maccenter = div (add (mac.rect.origin, mac.rect.corner), 2);
}

int	burgerrate;

macstart()
{
	burgerrate = score / 50000;
	mac.value = STARTBURGER;
	itoa (macbuf, mac.value, 4);
	objbirth (&mac);
	put_burger ();
}

domac()
{
	mac.value = min (9999, mac.value + burgerrate);
	put_burger ();
	itoa (macbuf, mac.value, 4);
	put_burger ();
}

killmac(killerp)
struct object	*killerp;
{
	CLEAR_ATT (*killerp, EXISTS);
	put_object (*killerp);
	if (mac.value == 0) {
		objdeath (&mac);
		score += MACVALUE;
		disp_score();
		put_burger ();
		macbuf[0] = '\0';
		mac.fuse = MACGONE;
	}
	else {
		score++;
		disp_score();
		mac.value = max (0, mac.value - 1);
		put_burger ();
		itoa (macbuf, mac.value, 4);
		put_burger ();
	}
}
