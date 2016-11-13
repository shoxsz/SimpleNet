#ifndef _TCP_ACCEPTOR_HPP_
#define _TCP_ACCEPTOR_HPP_

#include "socket_base.hpp"
#include "tcp_stream.hpp"

#include <vector>
#include <memory>

namespace snet{
	class TcpAcceptor : public Socket{
	public:
		enum { defaultBlocklog = 10 };

		TcpAcceptor(const TcpAcceptor& server) = delete;
		TcpAcceptor& operator=(const TcpAcceptor& server) = delete;

		TcpAcceptor():listening(false), bound(false){}
		~TcpAcceptor();

		void bind(unsigned short port, unsigned int blocklog = defaultBlocklog);

		TcpStreamPtr listen();

		unsigned int getBlocklog()const{
			return blocklog;
		}

		unsigned short getPort()const{
			return port;
		}

		bool isListening()const{
			return listening;
		}

		bool isBound()const{
			return bound;
		}

	private:
		void closing();

		bool listening, bound;
		unsigned int blocklog;
		unsigned short port;
	};
};

#endif