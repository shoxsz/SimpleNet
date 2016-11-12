#include "tcp_stream.hpp"

using namespace snet;

#define gmin(a, b) (((a) < (b)) ? (a) : (b))

void TcpStream::open(const InternetAddress& address){
	SOCKADDR_IN sock_info;

	if (!isValid())
		create(AF_INET, SOCK_STREAM, 0);

	sock_info.sin_family = AF_INET;
	sock_info.sin_port = htons(address.getPort());
	sock_info.sin_addr.s_addr = inet_addr(address.getIp().c_str());

	if (connect(sock_fd, (SOCKADDR*)&sock_info, sizeof(SOCKADDR)) == SOCKET_ERROR)
		throw SocketError();

	dest.setFamily(sock_info.sin_family);
	dest.setPort(sock_info.sin_port);
	dest.setHost(sock_info.sin_addr.s_addr);
}

int TcpStream::send(const NetworkMessage& msg){
	unsigned int sent;

	if ((sent = ::send(sock_fd, output->data(), gmin(bytes, output->getLength()), 0)) == SOCKET_ERROR){
		throw SocketError();
	}
}

int TcpStream::read(NetworkMessage& msg){
	unsigned int read;

	msg->setPosition(0);
	if ((read = recv(sock_fd, msg->data(), msg.getbuffSize(), 0)) == SOCKET_ERROR){
		throw SocketError();
	}

	if (read == 0){
		close();
		throw SocketError();
	}

	return read;
}
