#include "tcp_acceptor.hpp"

using namespace snet;

TcpAcceptor::~TcpAcceptor(){
	close();
}

TcpStreamPtr TcpAcceptor::listen(){
	TcpStreamPtr newConn(nullptr);
	struct sockaddr_in clientAddr;
	int clientLength = sizeof(struct sockaddr_in);
	unsigned int clientFd;

	if (!isValid()){
		throw SocketError("You must bind the server to a port first!");
	}

	listening = true;
	if ((clientFd = accept(sock_fd, (struct sockaddr*)&clientAddr, &clientLength)) == INVALID_SOCKET){
		//if listening is true then this acceptor wasn't stopped, this means we got an error...(this can happen for multithreaded applications)
		if (listening){
			listening = false;
			throw SocketError();
		}
	}
	else{
		listening = false;
		newConn = TcpStreamPtr(new TcpStream());
		newConn->address = InternetAddress::fromOld(clientAddr);
		newConn->endpoint = address;
		newConn->sock_fd = clientFd;
		newConn->valid = true;
	}
	return newConn;
}

void TcpAcceptor::bind(unsigned short port, unsigned int blocklog){
	struct sockaddr_in addr;
	
	if (isValid()){
		throw SocketError("The server is already bound!");
	}

	//can throw
	create(AF_INET, SOCK_STREAM, 0);

	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (::bind(sock_fd, (struct sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR){
		throw SocketError();
	}

	if (::listen(sock_fd, blocklog) == SOCKET_ERROR){
		throw SocketError();
	}

	this->blocklog = blocklog;
	this->port = port;
	bound = true;
}

void TcpAcceptor::closing(){
	if (listening){
		bound = false;
		listening = false;//called on closing operation
	}
}