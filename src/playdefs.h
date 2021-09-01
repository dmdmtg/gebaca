/*

 *      Copyright (c) 1984, 1985 AT&T
 *      All Rights Reserved

 *      THIS IS UNPUBLISHED PROPRIETARY SOURCE 
 *      CODE OF AT&T.
 *      The copyright notice above does not 
 *      evidence any actual or intended
 *      publication of such source code.

 */
# include "blit.h"
# include "font.h"

# define BLTWIDTH	(32 / WORDSIZE)
# define TRUE	1
# define FALSE	0

#ifdef mc68000 /* attempts at making this portable	*/
# define bttn1	button1
# define bttn2	button2
# define bttn3	button3
# define bttn12	button12
# define bttn23	button23
# define bttn123	button123

# define Texture16	Texture
# define texture16	texture
#endif

# define urnd(a, b) ((a) + rand()%((b) - (a)))	/* a < urnd (a, b) < b */

#ifdef mc68000
# define beep	(*((char*)(384*1024L+062)) = 2) /* blit only */
# define BonW() ((*DSTAT)|= 1)
# define WonB() ((*DSTAT) &= ~(1))
#else
# define beep	ringbell()
# define BonW()	RVIDEO
# define WonB()	TVIDEO
#endif

# define max(a, b)	((a > b) ? (a) : (b))
# define min(a, b)	((a < b) ? (a) : (b))
# define div2(expr)	((expr) >> 1)	/* quick divide by 2	*/
# define mul2(expr)	((expr) << 1)	/* quick multiply by 2	*/

# define put_object(obj) (bitblt ((obj).looks, (obj).looks->rect, &display, (obj).rect.origin, F_XOR))

# define put_icon(icon) (bitblt ((icon).obj.looks, (icon).irect, &display, (icon).obj.rect.origin, F_XOR))

# define put_burger() (string (&defont, macbuf, &display, add (mac.rect.origin, Pt (2, 19)), F_XOR))

/* 'show_extra' (un)displays extra players at top of screen.
# define show_extra(num)	(bitblt (&melooks, melooks.rect, &display, add (Drect.origin, Pt(((num)*48+10), 10)), F_XOR))
*/
/* 'moveobj' undisplays and redisplays objects to move them. It is a
 * 	'# define' for efficiency reasons only.
 */
# define moveobj(obj)	if ((obj.dirchoose) (&obj)) { \
				put_object (obj); \
				obj.rect = obj.next; \
				put_object (obj); \
			} \
			else ;

/* 'moveicon' undisplays and redisplays icons to move them. It is a
 * 	'# define' for efficiency reasons only.
 */
# define moveicon(icon)	if ((icon.obj.dirchoose) (&(icon.obj))) { \
				put_icon (icon); \
				icon.obj.rect = icon.obj.next; \
				put_icon (icon); \
			} \
			else ;

# define MINWIDTH	64	/* Minimum playing area width	*/
# define MINHEIGHT	64	/* Minimum playing area height	*/

/* These 'rates' are correspond roughly to the number of game ticks
 *	that pass between moves of that object.
 */

# define SHOTRATE	2
# define NOTERATE	4
# define EXTRARATE	5	/* Rate that bored extra men move	*/
# define SMURFRATE	5
# define IBMRATE	6
# define BLLRATE	8
# define DIGITRATE	8
# define DEBRATE	9
# define PULLRATE	9
# define GABRATE	11
# define BOBRATE	17
# define EDDIERATE	30
# define MACRATE	31
# define MTVRATE	33
# define WAITRATE	34	/* Game ticks between each 'wait(CPU)'	*/
/*******************************/

# define BLLGONE	200
# define BLLMAX	10
# define BLLVALUE	50

# define BOBVALUE	1000
# define BOBSPEED	2	/* minimum bob speed	*/
# define BOBGONE	600

# define DEFONTWIDTH	9
# define DEFONTHEIGHT	13

# define STARTDIGIT	100000
# define DIGMAX	6

# define EDDIESPEED	3
# define EDDIEGONE	160
# define EDDIEVALUE	500

# define GABGONE	5
# define GABMAX	15		/* Number of words on the screen at one time	*/
# define GABSPEED	13
# define GABVALUE	25
# define WORDMAX	44	/* Num of words in Eddie's spiel	*/


# define IBMSPEED	5
# define IBMGONE	150
# define IBMVALUE	250

# define EXTRAFUSE	900
# define EXMAX	5	/* Maximum number of wandering extra men	*/
# define STARTEXTRA	50000 /* At what score the extra men get bored	*/
# define MEWIDTH	32	/* Width of player object in pixels	*/
# define MEHEIGHT	32	/* Height of player object in pixels	*/
# define ME_MAX  3	/* Number of player objects at start of game	*/

# define STARTBURGER	200
# define MACAFTER	70000
# define MACGONE	400
# define MACVALUE	5000

# define FIRSTMTVBUMP	75000
# define MAXHEAL	6	/* The avg. time for mtv to heal	*/
# define MTVSPEED	5
# define MTVGONE	150
# define MTVVALUE	25

# define STARTNOTES	4
# define NOTEMAX	15
# define NOTESPEED	7

/* Player's missile parameters	*/
# define SHOTSPEED	(-20)	/* Pixels moved by missile	*/

# define SMURFLIFE	500	/* Time until smurf blows up automatically */
# define SMURFVALUE	200	/* Obtained if you shoot smurf	*/

# define NEWMANSTEP	12000	/* New player at every 'NEWMANSTEP' pts.*/

/* The number of chunks something blows up into	*/
# define DEBMAX	5
# define DECAYTIME	20

/* Miscellaneous attributes <object>.atts' */
# define EXISTS	001	/* TRUE if object is currently on screen.	*/
# define EXPLODING	002	/* TRUE => object exploding	*/
# define MOVING	020

/* attribute manipulating functions	*/

/* 'IS_SET' returns TRUE iff 'att' is TRUE for 'obj'	*/
# define IS_SET(obj, att)	((obj).atts & (att))
/* 'SET_ATT' sets 'att' to TRUE for 'obj'	*/
# define SET_ATT(obj, att)	((obj).atts |= (att))
/* 'CLEAR_ATT' sets att to FALSE for 'obj'	*/
# define CLEAR_ATT(obj, att)	((obj).atts &= ~(att))

/* object types	*/
# define BALL	1
# define BELL	2
# define BOB	3
# define EDDIE	4
# define EXTRA	5
# define GAB	6
# define IBM	7
# define III	8	/* a single 'I' from IBM	*/
# define BBB	9	/* a single 'B' from IBM	*/
# define MMM	10	/* a single 'M' from IBM	*/
# define ME	11
# define MAC	12
# define MTV	13
# define NOTE	14
# define SMURF	15
# define DEBRIS	16
/* end - object types	*/

typedef long	i4;

typedef	struct	object { /* Everything that moves in this game is
				treated as an object. */
	Rectangle	rect,	/* current screen location of object	*/
			next;	/* Scr. loc. to move object to next.	*/
	int	dxdt,	/* Horizontal movement rate in pixels	*/
		dydt,	/* Vertical movement rate in pixels	*/
		fuse,	/* A timer associated with this object	*/
		type,	/* Not used	*/
		value; /* worth or hit points */
	int	(*dirchoose)();	/* function for deciding next move	*/
	Bitmap	*looks;	/* appearance of object	*/
	int	atts; /* bit vector of other attributes. */
};

typedef	struct	icon {
	struct	object	obj;
	Rectangle	irect; /* rectangle within bitmap	*/
};

# define NULLNODE	((struct objnode *) 0)

typedef struct	objnode {
	struct object	*objpoint;
	struct objnode	*nextnode;
};

/* The following stuff is in 'globals.c'	*/
extern	Point	oldmouse;
extern	Rectangle	playerrect;
#ifdef mc68000
extern	Texture16	plain,
		fancy;
#else
extern	Texture	plain,
		fancy;
#endif

extern	Bitmap	balllooks,
		belllooks,
		boblooks,
		melooks,
		mtvlooks,
		notelooks,
		shotlooks,
		spiderlooks;
		
extern	struct	object	bll[BLLMAX],
			bob,
			bobdebris[DEBMAX],
			digit[DIGMAX],
			eddie,
			eddiedebris[DEBMAX],
			exshot[EXMAX],
			extra[EXMAX],
			gab[GABMAX],
			me,
			mac,
			mtv,
			mtvdebris[DEBMAX],
			note[NOTEMAX],
			shot,
			smurf,
			smurfdebris[DEBMAX];

extern	struct	icon	I, B, M, ibm;

extern	int	bumpmtvat,
	Dbug,
	healrate,
	me_cnt,
	mtvscore,
	notecnt,
	notespeed,
	curnotes,
	thetarange,
	round_cnt,
	start_over,
	wanderers;

extern	long	score,
		prevscore,
		newmanat;

extern	Point	maccenter;

extern	char	macbuf[6];
extern	char	mtvbuf[6];
extern	char	*gabwords[WORDMAX];
extern	int	gabp;

/* define BADDIEMAX	BLLMAX+1+DEBMAX+1+DEBMAX+NOTEMAX+1+DEBMAX+GABMAX
 * bells and balls + bob + bobdebris + mtv + mtvdebris
 *	+ notes + eddie + eddiedebris + gab
 */
extern	struct	objnode	*headnode;

# define BADICONMAX	4
/* ibm, I, B, M
 */
extern	struct	icon	*badicons[BADICONMAX];

extern	Bitmap	N[10]; /* in 'digits.c' */
extern	Bitmap	chunks[DEBMAX];
extern	Rectangle	chunkbounds[DEBMAX];

extern	int	bllchoose();
extern	int	bobandeddiechoose();
extern	int	digitchoose();
extern	int	eddiechoose();
extern	int	extrachoose();
extern	int	gabchoose();
extern	int	ibmchoose();
extern	int	macchoose();
extern	int	mtvchoose();
extern	int	notechoose();
extern	int	personchoose();
extern	int	shotchoose();
extern	int	smurfchoose();
extern	int	debchoose();
