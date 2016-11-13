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

		/*wait until some data is available, the maximum amount of data read is the size of the buffer in msg
		throw SocketError exception if error or disconnection occurs*/
		int read(NetworkMessage& msg){
			return internalRead(msg, 0);
		}

		/*wait until some data is available, the maximum amount of data read is the size of the buffer in msg
		note that that the data will not be removed from the buffer, subsequent read calls will return the same data
		throw SocketError exception if error or disconnection occurs*/
		int peek(NetworkMessage& msg){
			return internalRead(msg, MSG_PEEK);
		}

		/*wait until there is enough data to fill the buffer in msg
		throw SocketError exception if error or disconnection occurs*/
		int fill(NetworkMessage& msg){
			return internalRead(msg, MSG_WAITALL);
		}

		const InternetAddress& getEndpoint()const{ return endpoint; }
	private:
		int internalRead(NetworkMessage& msg, int flags);

		InternetAddress endpoint;
	};

	typedef std::shared_ptr<TcpStream> TcpStreamPtr;
};

#endif