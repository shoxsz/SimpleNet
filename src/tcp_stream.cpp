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
		throw SocketError(Exception::getSystemError());

	dest.setFamily(sock_info.sin_family);
	dest.setPort(sock_info.sin_port);
	dest.setHost(sock_info.sin_addr.s_addr);

	input.reset(new NetworkMessage());
	output.reset(new NetworkMessage());

	input->resize(1024);
	output->resize(1024);
}

int TcpStream::send(){
	return send(output->getLength());
}

int TcpStream::send(unsigned int bytes){
	unsigned int sent;

	output->setPosition(0);
	if ((sent = ::send(sock_fd, output->data(), gmin(bytes, output->getLength()), 0)) == SOCKET_ERROR){
		throw SocketError(Exception::getSystemError());
	}
	output->clear();
}

int TcpStream::read(){
	return read(getAvailableData());
}

int TcpStream::read(unsigned int bytes){
	unsigned int read;

	input->resize(bytes, false);
	input->clear();
	if ((read = recv(sock_fd, input->data(), bytes, 0)) == SOCKET_ERROR){
		throw SocketError(Exception::getSystemError());
	}

	if (read == 0){
		close();
		throw SocketError(Exception::getSystemError());
	}
	input->setPosition(0);

	return read;
}
