/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <string>
#include <cstring> 	//contains string functions
#include <iostream>
#include <unistd.h> //contains various constants

#include "SIMPLESOCKET.H"

#define size_x 10
#define size_y 10

using namespace std;

int main() {
	srand(time(NULL));
	TCPclient c;
	string host = "localhost";   // Zuhause könnte man für Local host eine IP einfügen
	string msg = "START";


	//connect to host
	c.conn(host , 2022); // 2022 Server Port    // Verbindung aufbauen

    cout << "client sends: " << msg << endl;
    c.sendData(msg);
    msg = c.receive(32);
    cout << "got response: " << msg << endl;

    bool gameOver = false;
    int x = 1, y = 1, ctr = 0;


    while (!gameOver){
        ctr++;
        sleep(0.25);
        msg = (x < 10) ? "GUESS_X0"+to_string(x) : "GUESS_X"+to_string(x);     //Anpassung der einstelligen Zahlen auf zwei Stellen
        msg = (y < 10) ? msg + "_Y0"+to_string(y) : msg +"_Y"+to_string(y);
        cout << "client sends: " << msg << endl;
        c.sendData(msg);
        msg = c.receive(32);
        cout << "got response: " << msg << endl;
        if (strncmp(msg.c_str(), "GAME_OVER", 9) == 0){
            cout << "Number of Tries: " << ctr << "." << endl;
            gameOver = true;
            return 0;
        }
        x++;
        if (x > size_x){
            x = 1;
            y++;
        }
        if (y>10){
        //cout << "nicht erfolgreich" << endl;
        return 0;
        }
    }
}
