# SimpleNet API

This is just a bunch of classes designed to facilitate the development of applications that use the network, it was written in C++ 11 and use as it base the C socket API.

## Project Structure

All the classes inside the API are inside the `snet` namespace and they can be divided in two modules: the core and the extension.

* ##### The Core

    Contains the base classes used to create and manipulate sockets: **TcpAcceptor**, **UdpStream**, etc, all this classes are non-asynchronous, but they care for thread safety as they are intended to be part of a multi-threaded enviroment.
* ##### Extension(utility folder)

    The classes here are meant to extend the API core, they can facilitate commom uses of network based applications like: creation of a multi client TCP server or an UDP host to make data streaming. There also helpers classes to create and manipulate threads and services.
    
****

#### TODO

The main features that must be implemented are:
* Fix NetworkMessage class
* IPv6 support
* Add clasess to extend the core
