#ifndef _TCP_ACCEPTOR_HPP_
#define _TCP_ACCEPTOR_HPP_

#include "socket_base.hpp"
#include "tcp_stream.hpp"
#include "protocol.hpp"

#include <vector>
#include <memory>

namespace snet{
	class TcpAcceptor : public Socket{
	public:
		enum ErrorStage{
			CREATION = 0,
			BINDING,
			LISTENING,
			ACCEPTING
		};

		enum { defaultBlocklog = 10 };

		TcpAcceptor(const TcpAcceptor& server) = delete;
		TcpAcceptor& operator=(const TcpAcceptor& server) = delete;

		TcpAcceptor():listening(false), bound(false){}
		virtual ~TcpAcceptor();

		virtual void onAccept(TcpStreamPtr stream) = 0;

		virtual void error(ErrorStage errorStage, SocketError& ex) = 0;

		bool bind(unsigned short port, unsigned int blocklog = defaultBlocklog);

		bool listen();

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

	protected:
		void closing();

		bool listening, bound;
		unsigned int blocklog;
		unsigned short port;
	};
};

#endif