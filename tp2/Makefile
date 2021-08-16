CC = gcc
CFLAGS = -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion
VALGRIND = valgreen --leak-check=full --track-origins=yes --show-reachable=yes
CFILES = procesar_helper.c hash.c heap.c count_min_sketch.c procesar_tweets.c procesar_usuarios.c
HFILES = procesar_helper.h hash.h heap.h count_min_sketch.h
CMD = procesar_tweets
CMD2 = procesar_usuarios

build: $(CFILES) $(HFILES)
	$ clear
	$(CC) $(CFLAGS) procesar_helper.c hash.c count_min_sketch.c heap.c procesar_tweets.c -o procesar_tweets
	$(VALGRIND) cat tweets_head.txt | ./$(CMD) 2000 5
	$(CC) $(CFLAGS) procesar_helper.c hash.c procesar_usuarios.c -o procesar_usuarios
	$(VALGRIND) ./procesar_usuarios tweets_head.txt

run: build

zip: 
	$ zip tp2.zip $(CFILES) $(HFILES) deps.mk

clean:
	$ rm -f procesar_usuarios procesar_tweets *.zip