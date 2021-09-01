
#
#      Copyright (c) 1984, 1985 AT&T
#      All Rights Reserved
#
#      THIS IS UNPUBLISHED PROPRIETARY SOURCE 
#      CODE OF AT&T.
#      The copyright notice above does not 
#      evidence any actual or intended
#      publication of such source code.
#
#
#	'Makefile' for Gebaca by Pat Autilio
#	This makefile will make executables in the same directory
#		for both the blit and 5620, either stand-alone
#		or in a layer. The default action is to make
#		an stand-alone executable for both the blit and
#		5620.
#
#	Conventions:
#
#	make	make for blit (gebaca.jerq.j) 
#		and 5620 (gebaca.dmd.j) stand-alone
#
#	make jerq	make for blit stand-alone (gebaca.jerq.j)
#
#	make dmd	make for 5620 stand-alone (gebaca.dmd.j)
#
#	make gebaca.jerq.m	make for blit layer
#
#	make gebaca.dmd.m	make for dmd layer
#
#	make lint	Run lint. Put output in gebaca.lint
#
#	make print	send source to printer
#
#	make xprint	send source to xerox printer
#
#	make clean	remove object files
#
#	make cpio	Put all source into a cpio file.
#
GEBACAFLAGS	=

DMD	= /usr/add_on/dmd
DMDBIN	= $(DMD)/bin
DMDSGS	= $(DMD)/sgs/1.0ofc
DMDCC	= $(DMDBIN)/dmdcc
DMDINCLUDE	= $(DMD)/include
# Trade these two lines depending on whether or not you want to
#	compile for stand-alone or for layers.
DMDFLAGS	= -I$(DMDINCLUDE) -O
#DMDFLAGS	= -I$(DMDINCLUDE) -J -O $(GEBACAFLAGS)
DMDSTRIP	= /usr/bin/nm32/m32strip

# Trade these two lines dpending on whether or not you want to
#	compile for stand-alone or for layers.
DLIB	=	gebaca.dmd.a
#DLIB	=	gebaca.dmd.d

CFLAGS	=	$(DMDFLAGS)
CC	=	$(DMDCC)

.SUFFIXES: .b .j .d
JERQ	=	/usr/jerq
JERQINCLUDE	=	$(JERQ)/include
JERQBIN	=	$(JERQ)/bin
JERQFLAGS	=  -I$(JERQINCLUDE) $(GEBACAFLAGS) -Dmc68000 -O
#
JERQCC	=	$(JERQBIN)/mcc
JERQSTRIP	=	$(JERQBIN)/mstrip

CFILES =	atan3.c bll.c bob.c chunks.c digits.c eddie.c error.c\
		exit_game.c explode.c extra.c gab.c globals.c ibm.c\
		init_game.c init_round.c init_scr.c isallin.c itoa.c\
		killplayer.c\
		mac.c me.c mtv.c note.c objXobj.c objutils.c\
		perchoose.c\
		playdisp.c player.c srvmouse.c shot.c smurf.c sqrt.c

BFILES =	atan3.b bll.b bob.b chunks.b digits.b eddie.b error.b\
		exit_game.b explode.b extra.b gab.b globals.b ibm.b\
		init_game.b init_round.b init_scr.b isallin.b itoa.b\
		killplayer.b\
		mac.b me.b mtv.b note.b objXobj.b objutils.b\
		perchoose.b\
		playdisp.b player.b srvmouse.b shot.b smurf.b sqrt.b
JFILES =	atan3.j bll.j bob.j chunks.j digits.j eddie.j error.j\
		exit_game.j explode.j extra.j gab.j globals.j ibm.j\
		init_game.j init_round.j init_scr.j isallin.j itoa.j\
		killplayer.j\
		mac.j me.j mtv.j note.j objXobj.j objutils.j\
		perchoose.j\
		playdisp.j player.j srvmouse.j shot.j smurf.j sqrt.j

SRCFILES =	gebaca.6 gebaca.README Makefile playdefs.h $(CFILES)

#
# 'make' is all you have to make for this to work.
#
# Trade these lines depending on if you want to make stand-alone
#	or for a layer
# all: gebaca.jerq.j gebaca.dmd.j
all: gebaca.jerq.m gebaca.dmd.m

dmd: gebaca.dmd.j

jerq: gebaca.jerq.j

gebaca.dmd.m: $(DLIB)
	DMDSGS=$(DMDSGS) DMD=$(DMD) $(DMDCC) $(DLIB) -o gebaca.dmd.m
	$(DMDSTRIP) -r gebaca.dmd.m
	chmod 0755 gebaca.dmd.m

gebaca.dmd.j: $(DLIB)
	DMDSGS=$(DMDSGS) DMD=$(DMD) $(DMDCC) -J -o gebaca.dmd.j $(DLIB)
	$(DMDSTRIP) -r gebaca.dmd.j
	chmod 0755 gebaca.dmd.j

.PRECIOUS: $(DLIB)

$(DLIB):: \
		$(DLIB)(atan3.o)	\
		$(DLIB)(bll.o)	\
		$(DLIB)(bob.o)	\
		$(DLIB)(chunks.o)	\
		$(DLIB)(digits.o)	\
		$(DLIB)(eddie.o)	\
		$(DLIB)(error.o)	\
		$(DLIB)(exit_game.o)	\
		$(DLIB)(explode.o)	\
		$(DLIB)(extra.o)	\
		$(DLIB)(gab.o)	\
		$(DLIB)(globals.o)	\
		$(DLIB)(ibm.o)	\
		$(DLIB)(init_game.o)	\
		$(DLIB)(init_round.o)	\
		$(DLIB)(init_scr.o)	\
		$(DLIB)(isallin.o)	\
		$(DLIB)(itoa.o)	\
		$(DLIB)(killplayer.o)	\
		$(DLIB)(mac.o)	\
		$(DLIB)(me.o)	\
		$(DLIB)(mtv.o)	\
		$(DLIB)(note.o)	\
		$(DLIB)(objXobj.o)	\
		$(DLIB)(objutils.o)	\
		$(DLIB)(perchoose.o)	\
		$(DLIB)(playdisp.o)	\
		$(DLIB)(player.o)	\
		$(DLIB)(srvmouse.o)	\
		$(DLIB)(shot.o)	\
		$(DLIB)(smurf.o)	\
		$(DLIB)(sqrt.o)
	DMDSGS=$(DMDSGS) DMD=$(DMD) $(DMDCC) $(DMDFLAGS) -c $(?:.o=.c)
	ar rvu $(DLIB) $?
	rm $?

.c.a:;

gebaca.jerq.m: $(BFILES)
	$(JERQCC) -o gebaca.jerq.m $(BFILES)
	$(JERQSTRIP) gebaca.jerq.m
	chmod 0755 gebaca.jerq.m

gebaca.jerq.j: $(JFILES)
	$(JERQCC) -j -o gebaca.jerq.j $(JFILES)
	$(JERQSTRIP) gebaca.jerq.j
	chmod 0755 gebaca.jerq.j

.c.b: playdefs.h
	$(JERQCC) $(JERQFLAGS) -c $<
	mv $*.o $*.b

.c.j: playdefs.h
	$(JERQCC) $(JERQFLAGS) -c -j $<
	mv $*.o $*.j
#
#
lint: $(CFILES)
	lint -I$(JERQINCLUDE) -Dmc68000 $(CFILES) > gebaca.lint
#
# 'make print' will print all source on the local line printer.
#
print:
	pr -n $(SRCFILES) | opr -fstandard
#
# 'make xprint' will print all source on a xerox printer if
#	available.
#
xprint:
	pr -n $(SRCFILES) | opr -txr -pland -fhole -uautilio -bT120 -dLZ

clean: shrink

shrink:
	rm -f *.b $(DLIB)
#
# 'make cpio' packs the source up for sending it somewhere.
#
cpio:
	ls $(SRCFILES) | cpio -oc > gebaca.cpio
#
# End of 'Makefile'
#
