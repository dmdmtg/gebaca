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

Point	oldmouse;	/* Where the mouse was at last check	*/
Rectangle	playerrect; /* Area of screen player stays in.*/

#ifdef mc68000
Texture16	plain = {
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000
};

Texture16	fancy = {
	0x1000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000
};

#else
Texture plain = {
	0x00000000, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0x00000000, 0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

Texture fancy = {
	0x10001000, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0x10001000, 0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
#endif

int	bumpmtvat,
	Dbug,
	healrate,	/* Inverse chance of mtv healing 	*/
	me_cnt,		/* No. of players remaining			*/
	mtvscore,	/* What the mtv was worth last time	*/
	notecnt,	/* number of notes that come out of 'mtv'	*/
	notespeed,	/* velocity of note	*/
	curnotes,	/* Number of unsuppressed notes	*/
	thetarange,	/* Width of notes stream	*/
	round_cnt,	/* Which Round of the game is it?	*/
	start_over,	/* Start a new game?	*/
	exploding;	/* TRUE => explosion currently on the screen	*/
		
long	newmanat,	/* Score needed for next extra man	*/
	prevscore,
	score;		/* Score					*/

Point	maccenter;	/* The center of the golden arches	*/

char	mtvbuf[6];	/* For displaying value of mtv	*/
char	*gabwords[WORDMAX] = {
	"Crazy", "Eddie's", "going", "video", "game",
	"CRAZY!", "with", "prices", "so", "low",
	"he's", "practically", "giving", "it", "all",
	"away!", "Buy", "a", "car!", "Buy",
	"a", "stereo!!", "Buy", "a", "car",
	"stereo!!!", "Find", "the", "lowest", "prices",
	"around", "then", "come", "to", "Crazy",
	"Eddie's", "and", "he'll", "beat", "'em!",
	"Pat", "Autilio", "is", "INNNSANE!!!!"
};

int	gabp;

struct	objnode	*headnode;

struct	icon	*badicons[BADICONMAX] = {
	&ibm, &I, &B, &M
};

Point	cursor;
