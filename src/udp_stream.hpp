#ifndef _UDP_STREAM_HPP_
#define _UDP_STREAM_HPP_

#include "socket_stream.hpp"

namespace snet{
	class UdpStream : public SocketStream{
	public:
		void open(unsigned short port);

		int send(const NetworkMessage& msg);
		
		int read(NetworkMessage& msg){
			return internalRead(msg, 0);
		}

		int peek(NetworkMessage& msg){
			return internalRead(msg, MSG_PEEK);
		}
	private:
		int internalRead(NetworkMessage& msg, int flags);

		InternetAddress endpoint;
	};
};

#endif