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
#include "TASK3.H"


class myServer : public TCPserver{
public:
    myServer(int port, int msgSize) : TCPserver(port, msgSize){
		;
	};


protected:
    string myResponse(string input){
        TASK3::World myWorld;

        while (1){

            if (strncmp(input.c_str(), "START", 5) == 0){       //Ersten 5 Stellen des Statements auf Gleichheit überprüfen
                myWorld(10,10,1,2,3,4);
                myWorld.printBoard();
                return "DONE";
            }
            else if (strncmp(input.c_str(), "GUESS", 5) == 0){		//Format: 	GUESS_X01_Y01 für Koordinaten x=1 & y=1
                int xCoord = input[8] 	+ input[9];		        //          1234567890123
                int yCoord = input[12] 	+ input[13];

                TASK3::ShootResult rsp = myWorld.shoot(xCoord, yCoord);

                //Antworten überprüfen
                if (rsp = TASK3::ShootResult::WATER){
                    return "WATER";
                }
                if (rsp = TASK3::ShootResult::SHIP_HIT){
                    return "SHIP_HIT";
                }
                if (rsp = TASK3::ShootResult::SHIP_DESTROYED){
                    return "SHIP_DESTROYED";
                }
                if (rsp = TASK3::ShootResult::ALL_SHIPS_DESTROYED){
                    return "ALL_SHIPS_DESTROYED";
                }
                if (rsp = TASK3::ShootResult::GAME_OVER){
                    return "GAME_OVER";
                }
                else return "ERROR";

                myWorld.printBoard();
            }
            else return "Fehlerhafte Eingabe";
        }
    };
};


int main(){
	srand(time(nullptr));
	myServer srv(2022,25);
	srv.run();
}
