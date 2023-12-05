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
#include <iostream>

#include <unistd.h> 	//contains various constants
#include <sys/types.h> 	//contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> 	//defines in_addr structure
#include <sys/socket.h> //for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses

#include "SIMPLESOCKET.H"
#include "TASK3.H"

bool init = false;
TASK3::World myWorld;

class myServer : public TCPserver{
public:
    myServer(int port, int msgSize) : TCPserver(port, msgSize){
		;
	};


protected:
    string myResponse(string input){



        if (!init){
        TASK3::World myWorld(10,10,1,2,3,4);
        init = true;
        }
        while (init){

            if (strncmp(input.c_str(), "START", 5) == 0){       //Ersten 5 Stellen des Statements auf Gleichheit überprüfen
                //myWorld(10,10,1,2,3,4);
                myWorld.printBoard();
                return "READY";
            }
            else if (strncmp(input.c_str(), "GUESS", 5) == 0){		//Format: 	GUESS_X01_Y01 für Koordinaten x=1 & y=1
                string strInput = input.c_str();
                char& x1 = strInput.at(7), x2 = strInput.at(8), y1 = strInput.at(11), y2 = strInput.at(12);
                //cout << x1 << endl;
                //cout << x2 << endl;

                string sX = ""; sX = sX + x1 + x2;
                string sY = ""; sY = sY + y1 + y2;//


                //cout << "sx=" << sX << endl;
                //cout << "sy=" << sY << endl;

                int xCoord = stoi(sX);
                int yCoord = stoi(sY);

                //cout << "x=" << xCoord << endl;
                //cout << "y=" << yCoord << endl;

                //sleep(0.25);
                myWorld.printBoard();

                TASK3::ShootResult eRsp = myWorld.shoot(xCoord, yCoord);
                //cout << "eRSP=" <<  eRsp << endl;
                int rsp = int(eRsp);
                //cout << "rsp=" << rsp << endl;


                //Antworten überprüfen

                if (rsp == TASK3::ShootResult::WATER){
                    //cout << "rsp=" << rsp << endl;
                    return "WATER";
                }
                else if (rsp == TASK3::ShootResult::SHIP_HIT){
                    //cout << "rsp=" << rsp << endl;
                    return "SHIP_HIT";
                }
                else if (rsp == TASK3::ShootResult::SHIP_DESTROYED){
                    return "SHIP_DESTROYED";
                }
                else if (rsp == TASK3::ShootResult::ALL_SHIPS_DESTROYED){
                    return "ALL_SHIPS_DESTROYED";
                }
                else if (rsp == TASK3::ShootResult::GAME_OVER){
                    cout << "rsp=" << rsp << endl;
                    return "GAME_OVER";
                }
                else return "ERROR";

            }
            else return 0; //"Fehlerhafte Eingabe";
        }
    };
};



int main(){
	srand(time(nullptr));
	myServer srv(2022,25);
	srv.run();
}
