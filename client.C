#include <string>
#include <cstring> 	//contains string functions
#include <iostream>
#include <unistd.h> //contains various constants

#include "SIMPLESOCKET.H"

#define size_x 10       //Festlegen der Spielfeldgröße
#define size_y 10

using namespace std;

class client{
public:
string guessMsg(int x, int y){  //Konvertiert Koordinaten in String für Server: GUESS_X01_Y01

    string msg = "";
    msg = (x < 10) ? "GUESS_X0"+to_string(x) : "GUESS_X"+to_string(x);     //Anpassung der einstelligen Zahlen auf zwei Stellen
    msg = (y < 10) ? msg + "_Y0"+to_string(y) : msg +"_Y"+to_string(y);
    return msg;
}

void strat_1(TCPclient* c){     //chronologisches Abarbeiten der Koordinaten: X01_Y01 bis X01_Y10, X02_Y01 bis X02_Y10, ...
    cout << "Strategie 1:"<< endl;

	string msg = "START";

    c->sendData(msg);
    msg = c->receive(32);

    bool gameOver = false;
    int x = 1, y = 1, ctr = 0;

    while (!gameOver){

        ctr++;
        sleep(0.25);
        msg = guessMsg(x, y);   // Koordinaten in String konvertieren, der von Server ausgewertet werden kann
        c->sendData(msg);
        msg = c->receive(32);
        if (strncmp(msg.c_str(), "GAME_OVER", 9) == 0){
            cout << "Number of Tries: " << ctr << "." << endl;  //Ausgabe der benötigten Versuche zur Lösung
            gameOver = true;
            return;
        }

        x++;
        if (x > size_x){        //Kontrolle, ob x-Koordinate in Präprozessoranweisung festgelegter Spielfeldgröße liegt
            x = 1;
            y++;
        }
        if (y>size_y){
            return;
        }
    }
}

void strat_2(TCPclient* c){     //bei initialem Treffer Schiffe systematisch abarbeiten, jedoch kein Gedächtnis über bereits anvisierte Felder

    cout << "Strategie 2:"<< endl;
	string msg = "START";

    c->sendData(msg);
    msg = c->receive(32);

    bool gameOver = false;
    int x = 1, y = 1, ctr = 0;
    bool ship_find = false;
    bool is_destroyed = true;
    bool second_shot = false;
    bool x_direction = true;
    int x_start = 0;
    int y_start = 0;
    int hits = 0;
    int last_shot_hits = 0;

    while (!gameOver){

        //Implementierung der Kommunikation
        ctr++;
        sleep(0.5);
        msg = guessMsg(x, y);   // Koordinaten in String konvertieren, der von Server ausgewertet werden kann
        c->sendData(msg);
        msg = c->receive(32);
        if (strncmp(msg.c_str(), "GAME_OVER", 9) == 0){
            cout << "Number of Tries: " << ctr << "." << endl;  //Ausgabe der benötigten Versuche zur Lösung
            gameOver = true;
            return;
        }
        //Implementierung der Strategie


        if (strncmp(msg.c_str(), "SHIP_HIT", 8) == 0){
            ship_find = true;
            hits ++;

            if (is_destroyed == true) {
                x_start = x;
                y_start = y;
                is_destroyed = false;
            }
        }

        if (ship_find == true){

            if (hits == last_shot_hits){x_direction = false; x--;}

            if (x_direction == true){x++; last_shot_hits = hits;}
            if (x_direction == false){y++; last_shot_hits = hits;}

            if (strncmp(msg.c_str(), "SHIP_DESTROYED", 14) == 0){
                ship_find = false;
                x = x_start + 2;
                y = y_start;
                if (x > size_x){        //Kontrolle, ob x-Koordinate in Präprozessoranweisung festgelegter Spielfeldgröße liegt
                    x = 1;
                    y++;
                }
                is_destroyed = true;
                hits = 0;
                last_shot_hits = 0;
                x_direction = true;
            }
        }



        else if (ship_find == false){
            x++;

            if (strncmp(msg.c_str(), "SHIP_DESTROYED", 14) == 0){x++;}

            if (x > size_x){        //Kontrolle, ob x-Koordinate in Präprozessoranweisung festgelegter Spielfeldgröße liegt
                x = 1;
                y++;
            }
            if (y>size_y){
                return;
            }
        }
    }
}

void strat_3(TCPclient* c){     //bei Treffer Schiff systematisch abarbeiten, an Schiffe angrenzende Felder werden übersprungen
    cout << "Strategie 3:"<< endl;

	string msg = "START";

    c->sendData(msg);
    msg = c->receive(32);

    int x = 1, y = 1, ctr = 0;          //Startkoordinaten für ersten Schuss
    int board[size_x+2][size_y+2] = {}; //Array mit 0 initialisieren, speichern der bereits bekannten Felder

    while(true){

        if (board[x][y] == 0){

            ctr++;

            msg = guessMsg(x, y);   // Koordinaten in String konvertieren, der von Server ausgewertet werden kann
            c->sendData(msg);
            msg = c->receive(32);

            if (strncmp(msg.c_str(), "GAME_OVER", 9) == 0){
                cout << "Number of Tries: " << ctr << "." << endl;  //Ausgabe der benötigten Versuche zur Lösung
                return;
            }   //end if

            if (strncmp(msg.c_str(), "SHIP_HIT", 8) == 0){
                int x_bfr = x, y_bfr = y;

                while (strncmp(msg.c_str(), "WATER", 5) != 0){  //Schiff in Richtung X+ weiterverfolgen
                    x_bfr++;
                    if (x_bfr > size_x){break;}                 //Richtung x+ nicht weiter verfolgen, wenn out of bounds
                    if (board[x_bfr][y_bfr] == 1){break;}       //nicht weiter verfolgen, wenn bereits abgedeckt
                    ctr++;
                    msg = guessMsg(x_bfr, y_bfr);               // Koordinaten in String konvertieren, der von Server ausgewertet werden kann
                    c->sendData(msg);
                    msg = c->receive(32);

                    if (strncmp(msg.c_str(), "SHIP_DESTROYED", 14) == 0){
                        for (int i = x-1; i<=x_bfr+1; i++){     //merken der umliegenden Felder nach Zerstörung des Schiffs
                            for (int j = y-1; j<=y_bfr+1; j++){
                                board[i][j] = 1;
                            }   //end for
                        }   //end for
                        break;
                    }   //end if (SHIP_DESTROYED)

                    if (strncmp(msg.c_str(), "GAME_OVER", 9) == 0){
                        cout << "Number of Tries: " << ctr << "." << endl;  //Ausgabe der benötigten Versuche zur Lösung
                        return;
                    }   //end if (GAME_OVER)
                }   //end while (!WATER)

                if (strncmp(msg.c_str(), "SHIP_DESTROYED", 14) != 0){   //Schiff nur in Y bearbeiten, wenn noch nicht zerstört
                    msg = "SHIP_HIT";           //Rücksetzen der Nachricht
                    int x_bfr = x, y_bfr = y;
                    while (strncmp(msg.c_str(), "WATER", 5) != 0){
                        y_bfr++;
                        if (y_bfr > size_y){break;}
                        if (board[x_bfr][y_bfr] == 1){break;}       //verhindert Schuss ins Wasser, bricht die Schleife anstelle der eigentlichen Abbruchbedingung ab
                        ctr++;
                        msg = guessMsg(x_bfr, y_bfr);       // Koordinaten in String konvertieren, der von Server ausgewertet werden kann
                        c->sendData(msg);
                        msg = c->receive(32);

                        if (strncmp(msg.c_str(), "SHIP_DESTROYED", 14) == 0){
                            for (int i = x-1; i<=x_bfr+1; i++){     //merken der umliegenden Felder nach Zerstörung des Schiffs
                                for (int j = y-1; j<=y_bfr+1; j++){
                                    board[i][j] = 1;
                                }   //end for
                            }   //end for
                            break;
                        }   //end if (SHIP_DESTROYED)
                        if (strncmp(msg.c_str(), "GAME_OVER", 9) == 0){
                            cout << "Number of Tries: " << ctr << "." << endl;  //Ausgabe der benötigten Versuche zur Lösung
                            return;
                        }   //end if (GAME_OVER)
                    }   //end while (!WATER)
                }   //end if (!SHIP_DESTROYED)
            }   //end if (SHIP_HIT)
        }   //end if (board[x][y] == 0)

        x++;
        if (x > size_x){        //Kontrolle, ob x-Koordinate in Präprozessoranweisung festgelegter Spielfeldgröße liegt
            x = 1;
            y++;
        }   //end if
        if (y>size_y){
            msg = "BYEBYE";
            cout << "client sends: " << msg << endl;
            c->sendData(msg);
            msg = c->receive(32);
            cout << "got response: " << msg << endl;
            return;
        }   //end if
    }   //end while (true)
}   //end strat_3

};  //end_class

int main(){
	srand(time(nullptr));

    TCPclient myClient;
    client myc1ient;
	string host = "localhost";
	bool success = myClient.conn(host , 2022);   // Verbindung aufbauen
    if (!success){return 0;}    //Beenden, wenn verbindung nicht aufgebaut werden konnte
    while (1){
        int currentStrat = 0;
        string msg;
        char userCmd;

        cout << "Strategie auswählen (1, 2 oder 3; q zum Beenden): " << endl;
        cin >> userCmd;

        if (isdigit(userCmd)){          //Überprüfen, ob Eingabe eine Zahl ist
            currentStrat = userCmd - '0';
            if (currentStrat < 1 || currentStrat > 3){      //Überprüfen, ob Zahl gültiger Strategie entspricht
                currentStrat = 0;
                cout << "Fehlerhafte Eingabe." << endl;
            }
            if (currentStrat != 0){
                cout << "Selected Strategy: " << currentStrat << endl;
            }
        }
        else if(userCmd == 'q'){            //Auf Befehl zum Beenden des Clients und Schließen der Serververbindung überprüfen
            cout << "Programm wird beendet." << endl;
            msg = "BYEBYE";
            myClient.sendData(msg);
            msg = myClient.receive(32);
            return 0;
        }
        else{
            cout << "Fehlerhafte Eingabe." << endl;
        }

        //Aufruf der in ausgwählten Strategie mit Übergabe des Clients
        if (currentStrat == 1){
            myc1ient.strat_1(&myClient);
        }
        else if (currentStrat == 2){
            myc1ient.strat_2(&myClient);
        }
        else if (currentStrat == 3){
            myc1ient.strat_3(&myClient);
        }
	}
}

