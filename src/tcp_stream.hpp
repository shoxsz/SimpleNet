#ifndef _TCP_STREAM_HPP_
#define _TCP_STREAM_HPP_

#include "socket_stream.hpp"

#include <memory>

namespace snet{
	class TcpStream : public SocketStream{
	public:
		friend class TcpAcceptor;

		void open(const InternetAddress& address);

		int send(const NetworkMessage& msg);
		int read(NetworkMessage& msg);

		const InternetAddress& getEndpoint()const{ return endpoint; }
	private:
		InternetAddress endpoint;
	};

	typedef std::shared_ptr<TcpStream> TcpStreamPtr;
};

#endif