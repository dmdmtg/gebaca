#include "playdefs.h"

/* 	Centipede by Pat Autilio
 *		Copyright 1983 - AT&T Information Systems
 *
 * 'main' for Centipede contains the main timing loop. Objects are
 *	moved based on the ticks of 'mv_clock'.
 */

short	shotclock,
	ibmclock = 1,
	extraclock,
	noteclock = 2,
	smurfclock = 4,
	bllclock,
	debclock,
	gabclock = 2,
	digitclock,
	pullclock,
	bobclock,
	eddieclock,
	macclock,
	mtvclock,
	waitclock;
	
main ()
{
	register int	i,
			j,
			k,
/*			mv_clock,	*/
			is_one,
			still_deb;
	Point	mecenter, distance, pull;
	int	b;

	jinit ();
	srand (mouse.xy.y ^ mouse.xy.x);
	WonB();

TOP:	start_over = FALSE;
	init_game ();
	
/* This loop may be exited by quitting (typing 'q') or by losing all 	*/
/*	of the players.							*/

	while (TRUE) {
		init_round ();
		round_cnt++;
/*		mv_clock = 0;	*/
		while (TRUE) {
			/* Release the cpu once in a while	*/
			if ((waitclock-- <= 0) &&
			    ptinrect (mouse.xy, Drect)) {
				waitclock = WAITRATE;
				wait (CPU);
			}

# ifdef MPX
			if (P->state & RESHAPED)
				goto TOP;
# endif MPX
			/* Move or maybe start the bob	*/
			if (bobclock-- <= 0) {
				bobclock = BOBRATE;
				if (IS_SET(bob, EXISTS)) {
					moveobj (bob);
					bob_collision();
				}
				else if (IS_SET(bob, EXPLODING)) {
					still_deb = FALSE;
					for (i = 0; i < DEBMAX; i++)
					    if (IS_SET(bobdebris[i], EXISTS)) {
						still_deb = TRUE;
						moveobj (bobdebris[i]);
					    }
					if (!still_deb) {
						CLEAR_ATT (bob, EXPLODING);
						bob.fuse = BOBGONE;
					}
				}
				else if (notecnt > 9 && (bob.fuse-- <= 0))
					bobstart();
			}
			
			/* Move or maybe start the ball	or bell */
			if (bllclock-- <= 0) {
				bllclock = BLLRATE;
				is_one = FALSE;
				for (i = 0; i < BLLMAX; i++) {
					if (IS_SET(bll[i], EXISTS)) {
						is_one = TRUE;
						moveobj (bll[i]);
					}
					else {
						for (j = 0; j < 50; j++)
							k ^= j;
					}
				}
				if ((!is_one) && bll[0].fuse-- <= 0) {
					bllstart (Pt(urnd (Drect.origin.x,
							   Drect.corner.x - 48),
						     Drect.origin.y));
				}
			}

			if ((score > STARTDIGIT) && 
			    (digitclock-- <= 0)) {
				digitclock = DIGITRATE;
				if (IS_SET (digit[0], EXISTS)) {
					for (i = 0; i < DIGMAX; i++) {
						moveobj (digit[i]);
					}
				}
				else {
					startdigits();
				}
			}
			
			if (ibmclock-- <= 0) {
				ibmclock = IBMRATE;
				if (IS_SET(ibm.obj, EXISTS)) {
					moveicon (ibm);
				}
				else {
					is_one = FALSE;

					if (IS_SET(I.obj, EXISTS)) {
						moveicon (I);
						is_one = TRUE;
					}
					if (IS_SET(B.obj, EXISTS)) {
						moveicon (B);
						is_one = TRUE;
					}
					if (IS_SET(M.obj, EXISTS)) {
						moveicon (M);
						is_one = TRUE;
					}
					if (!is_one && ibm.obj.fuse-- <= 0) {
						ibmstart();
					}
				}
			}

			if (IS_SET (mac, EXISTS)) {
# ifdef MACSUCK
				if (pullclock-- <= 0) { /* Add some attraction	*/
					pullclock = PULLRATE;
					mecenter.x = div2 (me.rect.origin.x + me.rect.corner.x);
					mecenter.y = div2 (me.rect.origin.y + me.rect.corner.y);

					distance = sub (mecenter, maccenter);

					b = div2(mac.value) / (abs (distance.y) + abs (distance.x));
					b = max (b, 1);
/* mac push	*/			pull.y = (distance.y > 0) ? b : -b;
					pull.x = (distance.x > 0) ? b : -b;
/* mac pull				pull.y = (distance.y < 0) ? b : -b;
					pull.x = (distance.x < 0) ? b : -b;
*/
					cursset (add (mouse.xy, pull));
				}
# endif MACSUCK
				if (macclock-- <= 0) {
					macclock = MACRATE;
					domac ();
				}
			}
			else if ((score > MACAFTER) && macclock-- <= 0) {
				macclock = MACRATE;
				if (mac.fuse-- <= 0) {
					macstart();
				}
			}

			if (mtvclock-- <= 0) {
				mtvclock = MTVRATE;
				if (IS_SET(mtv, EXISTS)) {
					moveobj (mtv);
				}
				else if (!(IS_SET(mtv, EXPLODING)) &&
					 mtv.fuse-- <= 0) {
					mtvstart();
				}
			}

			if (noteclock-- <= 0) {
				noteclock = NOTERATE;
				for (i = 0; i < curnotes; i++) {
					if (IS_SET(note[i], EXISTS)) {
						moveobj (note[i]);
					}
					else {
						for (j = 0; j < 10; j++)
							k ^= j;
					}
				}
				if (((rand() % healrate) < 1) &&
				    IS_SET(mtv, EXISTS) &&
				    (curnotes < notecnt)) {
					notestart();
					curnotes++;
					if (notecnt - curnotes < 8)
						thetarange = min (180, mul2(thetarange));
				}
			}

			if (IS_SET(eddie, EXISTS)) {
				if (eddieclock-- <= 0) {
					eddieclock = EDDIERATE;
					moveobj (eddie);
				}
				if (gabclock-- <= 0) {
					gabclock = GABRATE;
					for (i = 0; i < GABMAX; i++)
						if (IS_SET(gab[i], EXISTS)) {
							moveobj (gab[i]);
						}
					if (gab[0].fuse-- <= 0) {
						gabstart();
					}
				}
			}
			else if (eddieclock-- <= 0) {
				eddieclock = EDDIERATE;
				if (!(IS_SET(eddie, EXPLODING)) &&
				 notecnt > 7 && eddie.fuse-- <= 0) {
					eddiestart();
				}
				else {
					for (j = 0; j < 100; j++)
						k ^= j;
				}
			}

			if (debclock-- <= 0) {
				debclock = DEBRATE;
				if (IS_SET(eddie, EXPLODING)) {
					still_deb = FALSE;
					for (i = 0; i < DEBMAX; i++)
					    if (IS_SET(eddiedebris[i], EXISTS)) {
						still_deb = TRUE;
						moveobj (eddiedebris[i]);
					    }
					if (!still_deb)
						CLEAR_ATT (eddie, EXPLODING);
				}
				if (IS_SET(mtv, EXPLODING)) {
					still_deb = FALSE;
					for (i = 0; i < DEBMAX; i++)
					    if (IS_SET(mtvdebris[i], EXISTS)) {
						still_deb = TRUE;
						moveobj (mtvdebris[i]);
					    }
					if (!still_deb)
						CLEAR_ATT (mtv, EXPLODING);
				}
				if (IS_SET(smurf, EXPLODING)) {
					still_deb = FALSE;
					for (i = 0; i < DEBMAX; i++)
					    if (IS_SET(smurfdebris[i], EXISTS)) {
						still_deb = TRUE;
						moveobj (smurfdebris[i]);
					    }
					if (!still_deb)
						CLEAR_ATT (smurf, EXPLODING);
				}
			}
			
			/* Check the smurf	*/
			if (IS_SET (smurf, EXISTS) &&
			    (smurfclock-- <= 0)) {
				smurfclock = SMURFRATE;
				if (smurf.fuse-- <= 0) {
					killsmurf((struct object *) NULL);
				}
			}
			
			/* Move the player's missile	*/
			if (shotclock-- <= 0) {
				shotclock = SHOTRATE;
				if (IS_SET (shot, EXISTS)) {
					moveobj (shot);
					shot_collision (&shot);
				}
				else { /* a silly nap loop	*/
					for (j = 0; j < 40; j++)
						k ^= j;
				}
			}
			
			if ((score > STARTEXTRA) &&
			    (digitclock-- <= 0) &&
				(me_cnt > 1)) {
				digitclock = DIGITRATE;
				for (i = 0; i < EXMAX; i++) {
					if (IS_SET(extra[i], MOVING)) {
						moveobj (extra[i]);
						if (IS_SET(exshot[i], EXISTS)) {
							moveobj (exshot[i]);
							shot_collision (&(exshot[i]));
						}
						else {
							stexshot (i);
						}
					}
				}
				if ((!IS_SET(extra[EXMAX-1], MOVING)) &&
					extra[0].fuse-- <= 0) {
					startextra();
				}
			}
			
			if (score >= 314159) {
				berserk ();
			}

			moveobj (me);
			chk_mecollision();
/*			mv_clock++;			*/
			if (start_over) /* END OF GAME - start another	*/
				goto TOP;
		}
	}
}
