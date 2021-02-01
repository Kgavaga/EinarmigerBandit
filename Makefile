CC = gcc
PN = EinarmigerBandit

build:
	$(CC) *.c -o $(PN).exe

run:
	$(PN).exe