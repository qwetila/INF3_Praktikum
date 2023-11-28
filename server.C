/*
 * server.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */
#include <cstdio> 	//standard input and output library
#include <cstdlib> 	//this includes functions regarding memory allocation
#include <cstring> 	//contains string functions
#include <cerrno> 	//It defines macros for reporting and retrieving error conditions through error codes
#include <ctime> 	//contains various functions for manipulating date and time

#include <unistd.h> 	//contains various constants
#include <sys/types.h> 	//contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> 	//defines in_addr structure
#include <sys/socket.h> //for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses

#include "SIMPLESOCKET.H"


class myServer : public TCPserver{
public:
    myServer(int port, int msgSize) : TCPserver(port, msgSize){
		;
	};


protected:
    string myResponse(string input){
        if (strncmp(input, "START", 5) == 0){
			//World myWorld = new World();
		}	
		if (strncmp(input, "GUESS", 5) == 0){				//Format: 	GUESS_X01_Y01 für Koordinaten x=1 & y=1
			int xCoord = input[8] 	+ input[9];		//			1234567890123
			int yCoord = input[12] 	+ input[13];
			//Methode von TASK3 aufrufen, dafür erst World erstellen, Aufruf über myWorld
		}

		
		switch(input){
			case "NEWGAME": break;
			case "GUESS": break;
			case "NEWGAME": break;
			case "NEWGAME": break;
			 
    };
};


int main(){
	srand(time(nullptr));
	myServer srv(2022,25);
	srv.run();
}
