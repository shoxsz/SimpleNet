#ifndef _UDP_STREAM_HPP_
#define _UDP_STREAM_HPP_

#include "socket_stream.hpp"

namespace snet{
	class UdpStream : public SocketStream{
	public:
		UdpStream() : SocketStream() {}
		UdpStream(NetworkMessagePtr input, NetworkMessagePtr output) : SocketStream(input, output) {}
		~UdpStream(){}

		void open(unsigned short port);

		int send(const NetworkMessage& msg);
		int read(NetworkMessage& msg);

		void setEndpoint(const InternetAddress& endpoint){
			this->endpoint = endpoint;
		}

		InternetAddress& getEndpoint(){ return endpoint; }

	private:
		InternetAddress endpoint;
	};
}

#endif