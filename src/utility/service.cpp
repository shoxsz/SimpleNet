#include "service.hpp"

#include "service_manager.hpp"

using namespace snet;

void Service::wait(){
    while(stream->isValid()){
        waiting = true;
        read();
        waiting = false;
    }
    if(manager){
        manager->remove(managedId);
        manager = nullptr;
    }
}

void Service::read(){
    try{
        stream->read();
        if (rcvdFirst){
            protocol->onReceiveFirst(stream->getInput());
            rcvdFirst = true;
        }else
            protocol->onReceiveMessage(stream->getInput());
    }
    catch (SocketError& error){
        if (waiting){
            //service wasn't closed then we got an error
            stop();
            protocol->error(error);
        }
    }
}