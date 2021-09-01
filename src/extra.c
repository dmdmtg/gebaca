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

struct	object	extra[EXMAX];
struct	object	exshot[EXMAX];

int	wanderers = 0;

#define	show_extra(num)	(bitblt (&melooks, melooks.rect, &display, add (Drect.origin, Pt(((num)*48+10), 10)), F_XOR))

add_extra()
{
	int	newex = me_cnt-1;

	/* After a certain number of extra men are attained	*/
	/*	just display the men without having		*/
	/*	(potentially moving) objects representing them.	*/
	
	if (me_cnt >= EXMAX + 1) {
		show_extra (me_cnt);
		me_cnt++;
		return;
	}
	
	SET_ATT(extra[newex], EXISTS);
	CLEAR_ATT(extra[newex], MOVING);
	CLEAR_ATT(exshot[newex], EXISTS);
	extra[newex].rect = raddp (extra[newex].looks->rect,
				    add (Drect.origin,
					 Pt(me_cnt*48+10, 10)));
	put_object (extra[newex]);
	me_cnt++;
}

stop_wanderers()
{	
	int	i;
	
	for (i = 0; i < wanderers; i++) {
		put_object (extra[i]);
		CLEAR_ATT (extra[i], MOVING);
		extra[i].rect = raddp (extra[i].looks->rect,
					    add (Drect.origin,
						 Pt(i*48+58, 10)));
		put_object (extra[i]);
	
		/* stop shot too	*/
		if (IS_SET(exshot[i], EXISTS)) {
			CLEAR_ATT (exshot[i], EXISTS);
			put_object(exshot[i]);
		}
	}
	wanderers = 0;
}

del_extra()
{
	me_cnt--;
	if (Dbug)
		extra[0].fuse = 100;
	else
		extra[0].fuse = EXTRAFUSE;
		
	/* delete youngest extra	*/
	if (me_cnt >= EXMAX + 1) {
		show_extra (me_cnt);
	}
	else {
		CLEAR_ATT(extra[me_cnt-1], EXISTS);
		put_object (extra[me_cnt-1]);
	}
}

startextra()
{
	if (Dbug)
		extra[0].fuse = 100;
	else
		extra[0].fuse = EXTRAFUSE;
		
	if (wanderers < EXMAX && wanderers < me_cnt-1) {
		SET_ATT (extra[wanderers], MOVING);
		extra[wanderers].dydt = 3;
		extra[wanderers].dxdt = 4;
	
		/* start shot too	*/
		wanderers++;
	}
}

extrachoose(objp)
struct	object	*objp;
{
	if ((objp->value != objp->dydt) && (objp->value != 0))
		objp->value = objp->dydt;
	if (!(isallin(raddp(objp->rect, Pt(objp->dydt, 0)), playerrect))) {
		objp->dydt = -objp->dydt;
		objp->value = -objp->value;
	}
	if (urnd (0, 60) < 1)
		if (objp->value == 0)
			objp->value = urnd (0, 10);
		else
			objp->value = 0;
			
	if (urnd (0, 60) < 1)
		objp->dxdt = urnd (-10, 10);

	if (!(isallin(raddp(objp->rect, Pt(0, objp->dxdt)), playerrect)))
		objp->dxdt = -objp->dxdt;
		
	objp->next = raddp(objp->rect, Pt(objp->value, objp->dxdt));
	return (TRUE);
}

stexshot(which)
int	which;
{
	SET_ATT (exshot[which], EXISTS);
	exshot[which].rect = raddp (exshot[which].looks->rect,
				Pt(div2(extra[which].rect.corner.x +
				        extra[which].rect.origin.x),
				   extra[which].rect.origin.y));
	put_object (exshot[which]);
}

killextra(i, killerp)
int	i;
struct	object	*killerp;
{
	CLEAR_ATT (*killerp, EXISTS);
	put_object (*killerp);

	put_object (extra[i]);
	CLEAR_ATT (extra[i], MOVING);
	if (IS_SET(exshot[i], EXISTS)) {
		CLEAR_ATT (exshot[i], EXISTS);
		put_object (exshot[i]);
	}
	wanderers--;
	/* Stop youngest wanderer from moving	*/
	extra[i].rect = extra[wanderers].rect;
	exshot[i].rect = exshot[wanderers].rect;
	extra[wanderers].rect = raddp(extra[wanderers].looks->rect,
				       add (Drect.origin,
					    Pt(wanderers*48 + 58, 10)));
	put_object (extra[wanderers]);
	SET_ATT (extra[i], MOVING);
	SET_ATT (exshot[i], EXISTS);
	CLEAR_ATT (extra[wanderers], MOVING);
	CLEAR_ATT (exshot[wanderers], EXISTS);

	del_extra ();
}
