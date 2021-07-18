CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lpthread -lm -lrt
 
PROGNAME = dewv_eicar
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))
 
$(PROGNAME): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) 
 
clean:
	rm -f $(OBJS) $(PROGNAME) depend
 
depend: $(SRCS)
	$(CC) $(CFLAGS) -MM *.c > $@
