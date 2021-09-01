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
#include <font.h>

/* 'error' is used almost nowhere to show fatal error messages and
 *	exit.
 */
error (str)
char	*str;
{
	Point pete;

	pete = div (add (Drect.origin, Drect.corner), 2);
	string(&defont, str, &display, pete, F_XOR);
	string(&defont, "Hit return to exit", &display,
		add (pete, Pt(0, 16)), F_XOR);
	while (kbdchar() == -1) {
		wait (CPU);
	}
	exit (1);
}

/*
 * 'berserk' just makes the terminal go nuts when you get a very
 *	high score.
 */

# ifdef mc68000

# undef	bitblt
# undef texture
# define bitblt(s, r, d, p, c)	(*((void(*)())0x430d6))(s, r, d, p, c)
# define texture(s, r, d, c)	(*((void(*)())0x4372c))(s, r, d, c)
# endif

# define MADMAX	4

char	*laststr = "GAME OVER";

berserk ()
{
# ifdef MPX
# ifdef mc68000
	Point	kursor;
	Layer	*madlayers[MADMAX];
	Bitmap	*gameover;

	gameover = (Bitmap*) balloc (Rect (0, 0, 80, 16));
/*	dellayer(P->layer);
	for (i = 0; i < MADMAX; i++) {
		madlayers[i] = (Layer*) NULL;
	}
	addrspot = 1024L;
	for (i = 0; i < 300; i++) {
		if (urnd (0, 2) < 1)
			BonW();
		else
			WonB();
		
		splotch();
		
		newone = i % MADMAX;
		
		addrspot = (addrspot - (i * 10)) % (1024 * 39);
		*DADDR= addrspot;

		if (urnd (0, 3) == 0) {
			upleft.x = urnd (0, XMAX-20);
			upleft.y = urnd (0, YMAX-10);
			if (madlayers[newone] != (Layer*)NULL)
				dellayer(madlayers[newone]);

			madlayers[newone]=newlayer(Rect(upleft.x, upleft.y,
					       urnd(upleft.x+20, XMAX),
				       urnd(upleft.y+10, YMAX)));
		}
	}
	for (i = 0; i < MADMAX; i++) {
		dellayer(madlayers[i]);
	}
*/	texture (&display, Jrect, &plain, F_CLR);
	kursor.x = XMAX/2 - 40;
	kursor.y = YMAX/2;
	string (&defont, laststr, gameover, kursor, F_STORE);
	bitblt (gameover, gameover->rect, &display, kursor, F_STORE);
	*DADDR=(156*1024L/4);
# endif mc68000
# endif MPX
}

splotch()
{
	Rectangle	mungerect;

	mungerect.origin.x = urnd (0, XMAX-20);
	mungerect.origin.y = urnd (0, YMAX-20);
	mungerect.corner.x = urnd (mungerect.origin.x+20, XMAX);
	mungerect.corner.y = urnd (mungerect.origin.y+20, YMAX);
	texture (&display, mungerect, &plain, F_STORE);
}

twitch()
{
# ifdef mc68000
	*DADDR = (((urnd(0, 1023))*156L)/100)*25;
# endif
}
