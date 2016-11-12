#ifndef _PROTOCOL_HPP_
#define _PROTOCOL_HPP_

#include "network_message.hpp"

namespace snet{
	class Service;

	class Protocol{
	public:
		Protocol(){ service = nullptr; }

		virtual void onReceiveFirst(const NetworkMessage& msg) = 0;
		virtual void onReceiveMessage(const NetworkMessage& msg) = 0;
		virtual void error(SocketError& error) = 0;

		void setService(Service* Service){
			this->Service = Service;
		}

		Service* getService()const{
			return Service;
		}
	protected:
		virtual void parse(const NetworkMessage& msg) = 0;
		Service* Service;
	};
};

#endif