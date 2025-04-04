CC = gcc
CFLAGS = -std=c11 -Wall
SRC = main.c wlmrt.c strbuild.c cJSON.c system.c map.c

all: DSPD_2_Assignment

DSPD_2_Assignment: $(SRC)
	$(CC) $(CFLAGS) -o DSPD_2_Assignment $(SRC)

clean:
	rm -f DSPD_2_Assignment
