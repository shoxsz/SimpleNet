#include "tcp_stream.hpp"

using namespace snet;

void TcpStream::open(const InternetAddress& address){
	SOCKADDR_IN sock_info;

	if (!isValid())
		create(AF_INET, SOCK_STREAM, 0);

	sock_info.sin_family = AF_INET;
	sock_info.sin_port = htons(address.getPort());
	sock_info.sin_addr.s_addr = inet_addr(address.getIp().c_str());

	if (connect(sock_fd, (struct sockaddr*)&sock_info, sizeof(struct sockaddr)) == SOCKET_ERROR)
		throw SocketError();

	endpoint.fromOld(sock_info);
}

int TcpStream::send(const NetworkMessage& msg){
	unsigned int sent;

	if ((sent = ::send(sock_fd, msg.data(), msg.getLength(), 0)) == SOCKET_ERROR){
		throw SocketError();
	}
	return sent;
}

int TcpStream::internalRead(NetworkMessage& msg, int flags){
	unsigned int read;

	msg.setPosition(0);
	if ((read = recv(sock_fd, msg.data(), msg.getbuffSize(), flags)) == SOCKET_ERROR){
		throw SocketError();
	}

	if (read == 0 && msg.getbuffSize() > 0){
		close();
		throw SocketError();
	}
	msg.setLength(read);

	return read;
}

