#*********************************************
# MakeFile Telemetria
#*********************************************

telemetria: telemetria.c makefile
	gcc -ansi -Wall -O telemetria.c -o telemetria

clean:
	rm -f telemetria.o
