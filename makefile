mon_executable: Main.o colors.o frtustrum_culling.o geometrie.o lecture.o quadtree.o
	g++ -o mon_executable geometrie.o colors.o lecture.o frtustrum_culling.o quadtree.o Main.o

geometrie.o: geometrie.cpp geometry.h
	g++ -o geometrie.o -c geometrie.cpp -Wall -ansi

quadtree.o: quadtree.cpp quadtree.h
	g++ -o quadtree.o -c quadtree.cpp -Wall -ansi

colors.o: colors.cpp colors.h
	g++ -o colors.o -c colors.cpp -Wall -ansi

lecture.o: lecture.cpp lecture.h main.h
	g++ -o lecture.o -c lecture.cpp -Wall -ansi

frustum_culling.o: frustum_culling.cpp geometry.h quadtree.h 
	g++ -o frustum_culling.o -c frustum_culling.cpp -Wall -ansi

Main.o: Main.cpp main.h quadtree.cpp quadtree.h geometrie.cpp geometry.h colors.cpp colors.h lecture.cpp lecture.h  
	g++ -o Main.o -c Main.cpp -Wall -ansi