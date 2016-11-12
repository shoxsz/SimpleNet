#include "tcp_acceptor.hpp"

using namespace snet;

TcpAcceptor::~TcpAcceptor(){
	close();
}

bool TcpAcceptor::listen(){
	struct sockaddr_in clientAddr;
	int clientLength;
	unsigned short clientFd;

	if (!isValid())
		throw SocketError("You must bind the server to a port first!");

	if ((clientFd = accept(sock_fd, (sockaddr*)&clientAddr, &clientLength)) == INVALID_SOCKET){
		//if listening is true then acceptor wasn't stopped, this means we got an error...
		if (listening){
			listening = false;
			error(ACCEPTING, SocketError());
			return false;
		}
	}
	else{
		listening = false;
		TcpStream* newConn = new TcpStream();
		newConn->address.fromOld(clientAddr);
		newConn->dest = address;
		newConn->sock_fd = clientFd;
		newConn->valid = true;
		onAccept(newConn);
	}
	return true;
}

bool TcpAcceptor::bind(unsigned short port, unsigned int blocklog){
	struct sockaddr_in addr;
	
	if (isValid())
		throw std::runtime_error("The server is already bound!");

	try{
		create(AF_INET, SOCK_STREAM, 0);
	}
	catch (SocketError& ex){
		error(CREATION, ex);
		return false;
	}

	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (::bind(sock_fd, (sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR){
		error(BINDING, SocketError());
		return false;
	}

	if (::listen(sock_fd, blocklog) == SOCKET_ERROR){
		error(LISTENING, SocketError());
		return false;
	}

	this->blocklog = blocklog;
	this->port = port;
	bound = true;
	return true;
}

void TcpAcceptor::closing(){
	if (listening){
		bound = false;
		listening = false;
	}
}