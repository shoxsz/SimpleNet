# SimpleNet API

This is just a bunch of classes designed to facilitate the development of applications that use the network, it was written in C++ 11 and use as it base the C socket API.

With this API you will ease your network operations to just a few method calls, just to give an example:

```
snet::TcpStream stream;
stream.open(snet::InternetAddress(std::string("127.0.0.1"), 80));
```

the code above will create a socket and try to connect to the localhost at port 80. The examples can give you more information on how you can use the API.

****

## SimpleNet current state

**SimpleNet** is currently in development phase, the core classes are almost complete(missing some fixes), the next step is to implement some classes in the extension module, in fact, there are already some classes there, they can give you an idea of what can be expected to this module.

**Note from the author:** The API will be slowly updated since i have almost 0 hours of free time, it is uploaded here to also register my intention in developing this API. I would appreciate some advices about the API: the classes, coding style, things to avoid, things to add, etc. help is always welcome!

****

## SimpleNet Structure

All the classes inside the API are inside the `snet` namespace and they can be divided in two modules: the core and the extension.

* ##### The Core

    Contains the base classes used to create and manipulate sockets: **TcpAcceptor**, **UdpStream**, etc, all this classes are non-asynchronous, but they care for thread safety as they are intended to be part of a multi-threaded enviroment.
* ##### Extension

    The classes here are meant to extend the API core, classes to facilitate commom uses of network based applications like: creation of a multi client TCP server or an UDP host to make data streaming. There also helpers classes to create and manipulate threads and services.
    
****

#### For the future

The main features that must be implemented are:
* IPv6 support
