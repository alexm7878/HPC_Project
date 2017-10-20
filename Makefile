src = $(wildcard src/*.c)
obj = $(src:.c=.o)

LDFLAGS =

myprog: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) myprog
