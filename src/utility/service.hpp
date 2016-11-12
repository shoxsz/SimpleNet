#ifndef _Service_HPP_
#define _Service_HPP_

#include "tcp_stream.hpp"
#include "protocol.hpp"

#include <chrono>
#include <memory>

namespace nsa{
    class ServiceManager;

    class Service{
    public:
        friend class ServiceManager;

        Service(const Service& Service) = delete;
        Service& operator=(const Service& Service) = delete;

        Service(SocketStream* stream, Protocol* protocol):
            stream(stream),
            protocol(protocol),
            recvdFirst(false), 
            waiting(false),
            manager(nullptr){
                protocol->setService(this);
        }

        Service(SocketStream* Protocol* protocol, const std::chrono::milliseconds& timeout) : Service(stream, protocol){
            setWaitTimeout(timeout);
        }

        ~Service(){
            if(stream->isValid())
                close();

            delete stream;
            delete protocol;
        }

        void setWaitTimeout(const std::chrono::milliseconds& timeout){ 
            this->timeout = timeout;
            
            if(timeout.count() == 0)
                timeoutEnabled = false;
            else
                timeoutEnabled = true;
        }

        std::chrono::milliseconds& getWaitTimeout()const{ return timeout; }
        bool haveTimeout()const{ return timeoutEnabled; }

        void wait();

        void flush(){
            stream->send();
        }

        void flushSome(unsigned int bytes){
            stream->send(bytes);
        }

		void stop(){
			waiting = false;
			stream->close();
		}

        bool isOpen()const{
            return stream->isValid();
        }

        bool isManaged()const{ return manager != nullptr; }

        Protocol* getProtocol()const{ return protocol; }
        NetworkMessage& outputBuffer(){ return stream->getOutput(); }
        ServiceManager* getManager()const{ return manager; }
    protected:
        void read();

        bool rcvdFirst, waiting;
        SocketStream* stream;
        Protocol* protocol;
        bool timeoutEnabled;
        std::chrono::milliseconds timeout;

        unsigned int managedId;
        ServiceManager* manager;
    };

    typedef std::shared_ptr<Service> ServicePtr;
};

#endif