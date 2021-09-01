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

/* 'isallin' determines if Rectangle 'rect' is completely contained
 *	in Rectangle 'bigrect'.
 */
isallin (rect, bigrect)
Rectangle	rect,
		bigrect;
{
	if ((rect.origin.x >= bigrect.origin.x) &&
	    (rect.origin.y >= bigrect.origin.y) &&
	    (rect.corner.x <= bigrect.corner.x) &&
	    (rect.corner.y <= bigrect.corner.y))
		return (TRUE);
	else
		return (FALSE);
}
