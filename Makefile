VERSION=3.0

CC=cc
CFLAGS=-Wall $(DEBUG) $(OPTIM)
OPTIM=#-Os
DEBUG=-Werror -DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED -DGTK_DISABLE_SINGLE_INCLUDES -DGSEAL_ENABLE -g
OBJS=main.o midi-alsa.o device.o dialog.o oscillators.o filters.o envelope.o amplifier.o xmlparser.o
INCS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0` -lasound

all : alphajuno

.c.o :
	$(CC) $(CFLAGS) $(INCS) -c $<

alphajuno : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

strip : all
	strip alphajuno

clean : 
	-rm -f *.o *.core alphajuno

dist : clean
	cd .. && tar cvzf alphajuno-$(VERSION).tar.gz --exclude CVS alphajuno

main.o: main.h midi.h dialog.h device.h oscillators.h filters.h envelope.h amplifier.h xmlparser.h
midi.o: midi.h
device.o: midi.h main.h dialog.h device.h
dialog.o: midi.h main.h dialog.h
oscillators.o: main.h dialog.h oscillators.h
filters.o: main.h dialog.h filters.h
envelope.o: main.h dialog.h envelope.h
amplifier.o: main.h dialog.h amplifier.h
xmlparser.o: main.h xmlparser.h
