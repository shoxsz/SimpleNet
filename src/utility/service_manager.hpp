#ifndef _CONNECTION_MANAGER_HPP_
#define _CONNECTION_MANAGER_HPP_

#include "connection.hpp"

#include <unordered_map>

namespace snet{
    /*Use a thread group here*/
    class ServiceManager{
    private:
        class ServiceThread : public Thread{
        public:
            ServiceThread(Service* service):service(service){
                start();
            }

            ~ServiceThread(){
                delete service;
            }

        private:
            void run(){
                service->wait();
            }
            Service* service;
        };

        typedef std::shared_ptr<ServiceThread> ServiceThreadPtr;
    public:

        void add(Service* service){
            if(services.find(service->managedId) == services.end()){
                service->manager = this;
                service->managedId = (unsigned int)(&service);
                services[service->managedId].reset(new ServiceThreadPtr(service));
            }
        }

        void remove(unsigned int id){
            services.remove(id);
        }

    private:
        std::unordered_map<unsigned int, ServiceThreadPtr> services;
    };
};

#endif