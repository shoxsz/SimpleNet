#include "udp_stream.hpp"

using namespace snet;

void UdpStream::open(unsigned short port){
	struct sockaddr_in addr;

	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (::bind(sock_fd, (struct sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR){
		throw SocketError();
	}

	address.fromOld(addr);
}

int UdpStream::send(const NetworkMessage& msg){
	struct sockaddr_in o_addr = InternetAddress::toOld(msg.getSender());
	unsigned int sent;

	if ((sent = sendto(sock_fd, msg.data(), msg.getLength(), 0, (struct sockaddr*)&o_addr, sizeof(struct sockaddr))) == SOCKET_ERROR){
		throw SocketError();
	}

	return sent;
}

int UdpStream::internalRead(NetworkMessage& msg, int flags){
	struct sockaddr_in o_addr;
	unsigned int read;
	int l = sizeof(struct sockaddr);

	msg.setPosition(0);
	if ((read = recvfrom(sock_fd, msg.data(), msg.getbuffSize(), flags, (struct sockaddr*)&o_addr, &l)) == SOCKET_ERROR){
		throw SocketError();
	}

	if (read == 0){
		this->close();
		throw SocketError();
	}

	msg.setSender(InternetAddress::fromOld(o_addr));

	return read;
}