#ifndef _TCP_STREAM_HPP_
#define _TCP_STREAM_HPP_

#include "socket_stream.hpp"

#include <memory>

namespace snet{
	class TcpStream : public SocketStream{
	public:
		friend class TcpAcceptor;

		TcpStream() : SocketStream() {}
		~TcpStream(){}

		void open(const InternetAddress& address);

		int send();

		int send(unsigned int bytes);

		int read();

		int read(unsigned int bytes);

		const InternetAddress& getEndpoint()const{ return endpoint; }
	private:
		InternetAddress endpoint;
	};

	typedef std::shared_ptr<TcpStream> TcpStreamPtr;
};

#endif