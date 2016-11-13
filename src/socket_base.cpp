#include "socket_base.hpp"

using namespace snet;

#ifdef _WIN32
WSADATA initWinApiSock() {
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data) == SOCKET_ERROR)
		throw SocketError();
	return data;
}

void shutDownWinApiSock() {
	if (WSACleanup() == SOCKET_ERROR)
		throw SocketError();
}
#endif

std::string SocketError::lastError(){
	int error;
	char *buffer;
	
	std::lock_guard<std::mutex> locker(excLocker);//strerror is not thread-safe and Format
#ifdef _WIN32
	error = WSAGetLastError();
	if(FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		nullptr,
		error,
		0,
		(LPTSTR)&buffer,
		20,//random :p
		nullptr) == 0)
			return std::string("failed to read system error!");
#else
	error = errno;
	buffer = strerror();
#endif
	return std::string(buffer);
}

InternetAddress::InternetAddress(unsigned int host, unsigned short port){
	this->host = host;
	this->port = port;
}

InternetAddress::InternetAddress(const std::string& host, unsigned short port){
	setHost(host);
	this->port = port;
}

InternetAddress InternetAddress::fromOld(const sockaddr_in& address){
	return InternetAddress(old.sin_addr.s_addr, old.sin_port);
}

sockaddr_in InternetAddress::toOld(const InternetAddress& address)const{
	sockaddr_in sock;

	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);
	sock.sin_addr.s_addr = host;

	return sock;
}

void InternetAddress::setHost(const std::string &host){
	if (host.empty()){
		this->host = INADDR_ANY;
	}
	else{
		//the string does not represent a valid ipv4...
		if (inet_pton(AF_INET, host.c_str(), &this->host) == 0){
			//... try to identify it as an URL
			struct hostent *h;

			if ((h = gethostbyname(host.c_str()))){
				memcpy(&this->host, h->h_addr, h->h_length);
			}
			else
				throw SocketError("Invalid host!");
		}
	}
}

std::string InternetAddress::getIp() const{
	char ip_buff[17];

	if (inet_ntop(AF_INET, (void*)&host, ip_buff, 16) == nullptr)
		throw SocketError("Invalid host!");

	return std::string(ip_buff);
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
		throw SocketError();

	sock_fd = _return;
	valid = true;
}

void Socket::close(){
	if (isValid()){
		int r = 0;

		closing();
#ifdef _WIN32
		r = closesocket(sock_fd);
#else
		r = close(sock_fd);
#endif
		if(r != 0){
			throw SocketError();
		}else{
			valid = false;
		}
	}
}

u_long Socket::getAvailableData(){
	u_long available = 0;

#ifdef _WIN32

	if (ioctlsocket(sock_fd, FIONREAD, &available) == SOCKET_ERROR)
		throw SocketError();

#else

	if (ioctl(sock_fd, I_NREAD, &available) == SOCKET_ERROR){
		throw SocketError();

#endif // _WIN32

	return available;
}
