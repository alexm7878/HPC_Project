src = $(wildcard src/*.c)
obj = $(src:.c=.o)

LDFLAGS =

myprog: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)
	sh bash.sh

.PHONY: clean
clean:
	rm -f $(obj) myprog
	sh supr.sh
