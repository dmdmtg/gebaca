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

serve_mouse ()
{
	request(0);
	cursswitch ((Texture16*)0);
	sleep(2);
	request (MOUSE|KBD);
	wait (MOUSE);
	cursswitch (&plain);

}
