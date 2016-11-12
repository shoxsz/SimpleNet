#include "udp_stream.hpp"

using namespace snet;

#define gmin(a, b) (((a) < (b)) ? (a) : (b))

void UdpStream::open(unsigned short port){
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (::bind(sock_fd, (struct sockaddr*)&addr, sizeof(sockaddr)) == SOCKET_ERROR){
		throw SocketError(Exception::getSystemError());
	}

	address.fromOld(addr);
}

int UdpStream::send(){
	return send(output->getLength());
}

int UdpStream::send(unsigned int bytes){
	struct sockaddr_in o_addr = endpoint.toOld();
	unsigned int sent;

	output->setPosition(0);
	if ((sent = sendto(sock_fd, output->data(), gmin(bytes, output->getLength()), 0, (struct sockaddr*)&o_addr, sizeof(struct sockaddr))) == SOCKET_ERROR){
		throw SocketError(Exception::getSystemError());
	}

	return sent;
}

int UdpStream::read(){
	return read(getAvailableData());
}

int UdpStream::read(unsigned int bytes){
	struct sockaddr_in o_addr;
	unsigned int read;
	int l = sizeof(struct sockaddr);

	input->resize(bytes, false);
	input->clear();
	if ((read = recvfrom(sock_fd, input->data(), bytes, 0, (struct sockaddr*)&o_addr, &l)) == SOCKET_ERROR){
		throw SocketError(Exception::getSystemError());
	}

	if (read == 0){
		this->close();
		throw SocketError(Exception::getSystemError());
	}

	endpoint.fromOld(o_addr);

	return read;
}