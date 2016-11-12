#ifndef _SOCKET_STREAM_HPP_
#define _SOCKET_STREAM_HPP_

#include "socket_base.hpp"
#include "network_message.hpp"

namespace snet{
	/*A socket stream is used to keep a message flow within a socket*/
	class SocketStream : public Socket{
	public:
		virtual ~SocketStream(){}

		virtual int send(const NetworkMessage& msg) = 0;
		virtual int read(NetworkMessage& msg) = 0;
	};
};

#endif