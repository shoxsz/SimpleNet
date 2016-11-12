#ifndef _TCP_STREAM_HPP_
#define _TCP_STREAM_HPP_

#include "socket_stream.hpp"

#include <memory>

namespace nsa{
	class TcpStream : public SocketStream{
	public:
		friend class TcpServer;

		TcpStream() : SocketStream() {}
		TcpStream(NetworkMessagePtr input, NetworkMessagePtr output) : SocketStream(input, output) {}
		~TcpStream(){}

		void open(const InternetAddress& address);

		int send();

		int send(unsigned int bytes);

		int read();

		int read(unsigned int bytes);

		const InternetAddress& getDestination()const{ return dest; }
	private:
		InternetAddress dest;
	};

	typedef std::shared_ptr<TcpStream> TcpStreamPtr;
};

#endif