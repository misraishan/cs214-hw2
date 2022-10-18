objects: ls find
all: $(OBJECTS)
CFLAGS=-g -Wall -Werror -fsanitize=address

$(objects): %: %.c %.h
	gcc $(CFLAGS) -o $@ $< $(LFLAGS)

%: %.c
	gcc $(CFLAGS) -o $@ $< $(LFLAGS)

all: $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)
