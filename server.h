/**
 *
 * \file server.h
 *
 * \brief Contains the definition of the class server.
 *
 */

#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

/**
 *
 *  \class server
 *  \brief The class defining the implementation of the game server.
 *
 */
class server{
private:
    TASK3::World* myWorld;
public:
    /**
	 *
	 * \brief constructor
	 *
	 */
    myServer(int port, int msgSize);
protected:
    /**
	 *
	 * \brief Defines the response of the server regarding the current game.
	 *
	 * \param string input message from client to server.
	 *
	 * \return string answer to the client after internal processing.
	 *
	 */
    string myResponse(string input);

}

#endif // SERVER_H_INCLUDED
