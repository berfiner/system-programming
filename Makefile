main.o: main.c revoke.o dictionary.o
	gcc main.c -o main.o
	
revoke.o: revoke_history.c
	gcc revoke_history.c -o revoke_history.o

dictionary.o: dictionary.c
	gcc dictionary.c -o dictionary.o



