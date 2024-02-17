/**
 *
 * \file client.h
 *
 * \brief Contains the definition of the class client.
 *
 */

#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

/**
 *
 *  \class client
 *  \brief The class implementing the usage of different strategies.
 *
 */
class client{
public:
    /**
	 *
	 * \brief Creates the message to the server containing coordinates of the guess.
	 *
	 * \param int x x-coordinate for the guess.
	 *
	 * \param int y y-coordinate for the guess.
	 *
	 * \return string formatted string which can be interpretet by the server.
	 *
	 */
    string guessMsg(int x, int y);

    /**
	 *
	 * \brief Chronologically guess each coordinate from top left to bottom right.
	 *
	 * \param TCPclient* c Reference to the client with the established connection to the server.
	 *
	 */
    void strat_1(TCPclient* c);

    /**
	 *
	 * \brief Prioritize ship after initial hit whithout memorizing already guessed coordinates.
	 *
	 * \param TCPclient* c Reference to the client with the established connection to the server.
	 *
	 */
    void strat_2(TCPclient* c);

    /**
	 *
	 * \brief Prioritize ship after initial hit while memorizing already guessed coordinates and excluding neighbouring squares of already destroyed ships.
	 *
	 * \param TCPclient* c Reference to the client with the established connection to the server.
	 *
	 */
    void strat_3(TCPclient* c);



}




#endif // CLIENT_H_INCLUDED
