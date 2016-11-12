#ifndef _SOCKET_STREAM_HPP_
#define _SOCKET_STREAM_HPP_

#include "socket_base.hpp"
#include "network_message.hpp"

namespace snet{
	/*A socket stream is used to keep a message flow within a socket*/
	class SocketStream : public Socket{
	public:
		enum {defaultBufferSize = 1024}
		SocketStream(){
			input->resize(defaultBufferSize);
			output->resize(defaultBufferSize);
		}

		virtual ~SocketStream(){}

		virtual int send() = 0;
		virtual int send(unsigned int bytes) = 0;
		virtual int read() = 0;
		virtual int read(unsigned int bytes) = 0;

		NetworkMessage& getOutput(){ return output; }
		const NetworkMessage& getOutput()const{ return output; }
		NetworkMessage& getInput(){ return input; }
		const NetworkMessage& getInput()const{ return input; }

	protected:
		NetworkMessage input;
		NetworkMessage output;
	};
};

#endif