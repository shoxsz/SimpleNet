# SimpleNet API

This is a simple network API writen in C++ 11 based in the C sockets API, it is intended to be used on windows and linux systems, the main goal is to facilitate the creation of network applications that must use the TCP/UDP protocols.

****

## SimpleNet current state

**SimpleNet** is currently in development phase, the core classes are almost complete, the next step is to implement some classes in the utility module, in fact, there are already some classes there, they can give you an idea of what can be expected to this module.

**Note from the author:** The API will be slowly updated since i have almost 0 hours of free time, it is uploaded here to also register my intention in developing this API. I would appreciate some advices about the API: the classes, coding style, things to avoid, things to add, etc. and help is always welcome!

****

## SimpleNet Structure

The API can be divided in two modules: the core and the utility(utility is not a good name, it may change).

* ##### The Core

    Contains the base classes used to create and manipulate sockets: **TcpAcceptor**, **UdpStream**, etc, all this classes are non-asynchronous, but they care for thread safety as they are intended to be part of a multi-threaded enviroment.
* ##### Utilities

    The classes here are meant to extend the API core, they can provide classes to facilitate commom uses of network based applications like: creation of a multi client TCP server or an UDP host to make data streaming. There also helpers classes to create and manipulate threads and services.
    
****

#### For the future

The main features that must be implemented are:
* IPv6 support
