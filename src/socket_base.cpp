#include "socket_base.hpp"

using namespace snet;

InternetAddress::InternetAddress(short family, unsigned int host, unsigned short port){
	this->family = family;
	this->host = host;
	this->port = port;
}

InternetAddress::InternetAddress(short family, const std::string& host, unsigned short port){
	setHost(host);
	this->family = family;
	this->port = port;
}

void InternetAddress::fromOld(sockaddr_in old){
	family = old.sin_family;
	host = old.sin_addr.s_addr;
	port = old.sin_port;
}

void InternetAddress::setHost(const std::string &host){
	if (host.empty()){
		this->host = INADDR_ANY;
	}
	else{
		this->host = inet_addr(host.c_str());

		//the string does not represent a valid host...
		if (this->host == INADDR_NONE){
			//... try to identify it as an URL
			struct hostent *h;

			if ((h = gethostbyname(host.c_str()))){
				memcpy(&this->host, h->h_addr, h->h_length);
			}
			else
				throw(SocketError("Invalid host!"));
		}
	}
}

std::string InternetAddress::getIp() const{
	char* c_ip;
	struct in_addr addr;

	addr.s_addr = host;

	if (!(c_ip = inet_ntoa(addr)))
		throw(SocketError("Invalid host!"));

	return std::string(c_ip, strlen(c_ip));
}

sockaddr_in InternetAddress::toOld()const{
	sockaddr_in sock;

	sock.sin_family = family;
	sock.sin_port = htons(port);
	sock.sin_addr.s_addr = host;

	return sock;
}

Socket::Socket(){
	sock_fd = 0;
	valid = false;
}

Socket::~Socket(){
	close();
}

void Socket::create(int domain, int type, int protocol){
	int _return;

	if ((_return = socket(domain, type, protocol)) == SOCKET_ERROR)
		throw SocketError(Exception::getSystemError());

	sock_fd = _return;
	valid = true;
}

void Socket::close(){
	if (isValid()){
		closing();
#ifdef _WIN32
		closesocket(sock_fd);
#else
		close(sock_fd);
#endif
		valid = false;
	}
}

u_long Socket::getAvailableData(){
	u_long available = 0;

#ifdef _WIN32

	if (ioctlsocket(sock_fd, FIONREAD, &available) == SOCKET_ERROR)
		throw SocketError(Exception::getSystemError());

#else

	if (ioctl(sock_fd, I_NREAD, &available) == SOCKET_ERROR){
		return setError(Exception::getSystemError()), SOCKET_ERROR);

#endif // _WIN32

	return available;
}
