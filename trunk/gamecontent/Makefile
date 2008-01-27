SRCS = *.cpp
HEAD = *.h
OBJS = container.o creature.o globalvars.o inventory.o item.o map.o

all: __all.a

__all.a: $(OBJS)
	ar cru __all.a $(OBJS)
	ranlib __all.a

distdir:
	cp $(SRCS) $(distdir)
	cp $(HEAD) $(distdir)
	cp Makefile $(distdir)
distcheck:
clean:
	rm $(OBJS) __all.a

