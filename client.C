/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <string>
#include <iostream>
#include <unistd.h> //contains various constants

#include "SIMPLESOCKET.H"

using namespace std;

int main() {
	srand(time(NULL));
	TCPclient c;
	string host = "localhost";   // Zuhause könnte man für Local host eine IP einfügen
	string msg;

	//connect to host
	c.conn(host , 2022); // 2022 Server Port    // Verbindung aufbauen

	int i=0;
	bool goOn=1;
	while(goOn){ // send and receive data

		/*
		if((rand() % 20) < i++){
			msg = string("BYEBYE");
			goOn = 0;
		}else{
			msg = string("client wants this");
		}
		*/

		cout << "client sends: " << msg << endl;
		c.sendData(msg);                               //Paar senden und empfangen
		msg = c.receive(32);                           //receive wartet auf Antwort! 32 gibt message size an
		cout << "got response: " << msg << endl;
		sleep(500);

	}
}
