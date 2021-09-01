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

objbirth(objp)
struct object	*objp;
{
	struct objnode	*tempnodep;
	
	if ((tempnodep = (struct objnode *) alloc(sizeof(struct objnode *))) == NULLNODE) {
		error ("Can't allocate object node!");
	}
	SET_ATT(*objp, EXISTS);
	put_object(*objp);
	tempnodep->objpoint = objp;
	tempnodep->nextnode = headnode;
	headnode = tempnodep;
}

objdeath(objp)
struct object	*objp;
{
	register struct objnode	*np1, *np2, *np3;
	
	CLEAR_ATT (*objp, EXISTS);
	put_object (*objp);
/*	np1 = headnode;
	if (np1->objpoint == objp) {
		np3 = np1->nextnode->nextnode;
		free (np1->nextnode);
		np1->nextnode = np3;
		return;
	}
*/	for (np2 = headnode;
	     np2 != NULLNODE && np2->objpoint != objp;
	     np2 = np2->nextnode)
		np1 = np2;
	if (np2 == NULLNODE)
		error ("Can't find object to delete it!");
	np3 = np2->nextnode;
	free (np2);
	if (np2 == headnode)
		headnode = np3;
	else
		np1->nextnode = np3;
	return;
}

allobjdeath()
{
	register struct objnode	*np;
	int	i = 0;
	
	while (headnode != NULLNODE) {
		if (headnode->objpoint->type == GAB) {
			bfree (headnode->objpoint->looks);
		}
		CLEAR_ATT (*(headnode->objpoint), EXISTS);
		put_object (*(headnode->objpoint));
		np = headnode->nextnode;
		free (headnode);
		headnode = np;
		i++;
	}
	return (i);
}
