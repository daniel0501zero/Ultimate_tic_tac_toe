ultimate_tic_tac_toe : tic_tac_toe.o ultimate_tic_tac_toe.o
	g++ $^ -o ultimate_tic_tac_toe

ultimate_tic_tac_toe.o : ultimate_tic_tac_toe.cpp tic_tac_toe.h
	g++ -c $<

tic_tac_toe.o : tic_tac_toe.cpp tic_tac_toe.h
	g++ -c $<

clean : 
	rm -f *.o ultimate_tic_tac_toe

.PHONY: clean
 
