#ifndef _SOCKET_BASE_HPP_
#define _SOCKET_BASE_HPP_

#include <string>
#include <chrono>
#include <exception>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/sockets.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

namespace snet{

#ifdef _WIN32
	WSADATA initWinApiSock(){
		WSADATA data;
		if (WSAStartup(MAKEWORD(2, 2), &data) == SOCKET_ERROR)
			throw SocketError(Exception::getSystemError());
		return data;
	}

	void shutDownWinApiSock(){
		if (WSACleanup() == SOCKET_ERROR)
			throw SocketError(Exception::getSystemError());
	}
#endif

	class SocketError : public std::exception{
    public:
        SocketError(const char* error) : std::exception(error){}
        SocketError(const std::string& error) : std::exception(error.c_str()){}
    private:
    };

	/*InternetAddress simply wraps the sockaddr_in structure, it also provides
	methods to simplify host validation and methods to convert to/from sockaddr_in struct*/
	class InternetAddress{
	public:
		InternetAddress() : family(0),
			host(0),
			port(0){}
		InternetAddress(short family, unsigned int host, unsigned short port);
		InternetAddress(short family, const std::string& host, unsigned short port);
		void fromOld(sockaddr_in sockaddr);

		void setHost(unsigned int host){
			this->host = host;
		}

		void setHost(const std::string& host);

		void setFamily(short family){
			this->family = family;
		}
		
		void setPort(unsigned short port){
			this->port = port;
		}

		std::string getIp()const;

		short getFamily()const{ return family; }
		unsigned int getHost()const{ return host; }
		unsigned short getPort()const{ return port; }

		SOCKADDR_IN toOld()const;

	private:
		short family;
		unsigned int host;
		unsigned short port;
	};

	/*Represents a generic socket, it facilitates the creation and maintance of a socket,
	must be inherited to provide specific socket functionalities(@see SocketStream).
	This class will always keep the socket in block mode, but it provides methods
	to check for read/write operations avaibility, you can also access the socket
	descriptor directly by calling getSocketFd or you can either inherit this class
	to provide additional functionality.*/
	class Socket{
	public:
		Socket();
		virtual ~Socket();

		void create(int domain, int type, int protocol);
		void close();

		unsigned long getAvailableData();

		bool waitRead(std::chrono::milliseconds timeout){
			return testOperation(sock_fd, READ, timeout.count());
		}

		bool waitWrite(std::chrono::milliseconds timeout){
			return testOperation(sock_fd, WRITE, timeout.count());
		}

		bool canRead(){
			return testOperation(sock_fd, READ, 0);
		}

		bool canWrite(){
			return testOperation(sock_fd, WRITE, 0);
		}

		bool hasException(){
			return testOperation(sock_fd, EXCEPTION, 0);
		}

		unsigned short getSocketFD(){ return sock_fd; }
		InternetAddress getLocalAddress(){ return address; }
		bool isValid(){ return valid; }

	protected:
		//will be called in the close method before the socket is closed.
		virtual void closing(){}

		unsigned short sock_fd;
		bool valid;
		InternetAddress address;

	private:
		enum OPERATION{
			READ = 0,
			WRITE,
			EXCEPTION
		};

		/*test if the specific operation is available for this socket*/
		bool testOperation(unsigned short sock, OPERATION operation, long timeout){
			int r;
			struct timeval t_out;
			fd_set fds;

			t_out.sec = timeout / 1000;
			t_out.usec = (timeout - t_out.sec * 1000) * 1000000;

			FD_ZERO(&fds);
			FD_SET(sock, &fds);

			switch (operation){
			case READ:
				r = select(sock + 1, &fds, NULL, NULL, &t_out);
				break;
			case WRITE:
				r = select(sock + 1, NULL, &fds, NULL, &t_out);
				break;
			case EXCEPTION:
				r = select(sock + 1, NULL, NULL, &fds, &t_out);
				break;
			}

			if (r == SOCKET_ERROR)
				throw SocketError(Exception::getSystemError());

			if (FD_ISSET(sock, &fds))
				return true;

			return false;
		}
	};

};


#endif