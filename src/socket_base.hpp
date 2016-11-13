#ifndef _SOCKET_BASE_HPP_
#define _SOCKET_BASE_HPP_

#include <string>
#include <chrono>
#include <stdexcept>
#include <mutex>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>//this is for future support for ipv6
#else
#include <sys/types.h>
#include <sys/sockets.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SOCKET_ERROR (-1)
#define INVALID_SOCKET (-1)

#endif

namespace snet{
	class SocketError : public std::runtime_error{
    public:
        SocketError(const char* error) : std::runtime_error(error){}
        SocketError(const std::string& error) : std::runtime_error(error.c_str()){}
		SocketError() : SocketError(lastError()){}
    private:
		static std::mutex excLocker; //used while reading the system error message
		std::string lastError();	 //get the last error from the system
    };

	/*InternetAddress simply wraps the sockaddr_in structure(note that the IP version is 4), 
	it also provide methods to simplify host validation and methods to convert to/from sockaddr_in struct*/
	class InternetAddress{
	public:
		InternetAddress():
			host(0),
			port(0){}
		InternetAddress(unsigned int host, unsigned short port);
		InternetAddress(const std::string& host, unsigned short port);

		static InternetAddress fromOld(const sockaddr_in& address);
		static SOCKADDR_IN toOld(const InternetAddress& address);

		void setHost(unsigned int host){
			this->host = host;
		}

		void setHost(const std::string& host);
		
		void setPort(unsigned short port){
			this->port = port;
		}

		std::string getIp()const;

		unsigned int getHost()const{ return host; }
		unsigned short getPort()const{ return port; }

	private:
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

		unsigned int getSocketFD(){ return sock_fd; }
		InternetAddress getLocalAddress(){ return address; }
		bool isValid(){ return valid; }

	protected:
		//will be called in the close method before the socket is closed.
		virtual void closing(){}

		unsigned int sock_fd;
		bool valid;
		InternetAddress address;

	private:
		enum OPERATION{
			READ = 0,
			WRITE,
			EXCEPTION
		};

		/*test if the specific operation is available for this socket*/
		bool testOperation(unsigned int sock, OPERATION operation, long long timeout){
			int r;
			struct timeval t_out;
			fd_set fds;

			t_out.tv_sec = (long)(timeout / 1000);
			t_out.tv_usec = (long)(timeout - t_out.tv_sec * 1000) * 1000000;

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
				throw SocketError();

			if (FD_ISSET(sock, &fds))
				return true;

			return false;
		}
	};

#ifdef _WIN32
	WSADATA initWinApiSock();
	void shutDownWinApiSock();
#endif

};


#endif