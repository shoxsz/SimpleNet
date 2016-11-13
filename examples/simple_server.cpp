#include <iostream>
#include <string>
#include <simplenet.hpp>

void onAccept(snet::TcpStreamPtr stream);
void sendStringTooBig(snet::TcpStreamPtr stream);
void sendResult(snet::TcpStreamPtr stream, bool ok);

int main(){
    snet::TcpAcceptor server;
    snet::TcpStreamPtr client;

    try{
#ifdef _WIN32
        snet::initWinApiSock();
#endif
        server.bind(80, 1);
        while((client = server.listen()) != nullptr){
            onAccept(client);
        }
    }catch(const snet::SocketError& ex){
        std::cout << "Error: " << ex.what() << std::endl;
    }

#ifdef _WIN32
        snet::shutDownWinApiSock();
#endif
	
	server.close();

    return 0;
}

void onAccept(snet::TcpStreamPtr stream){
	const std::chrono::milliseconds TIMEOUT = std::chrono::milliseconds(10000);
    std::string password;
    snet::NetworkMessage input(2048);

    std::cout << "New connection from " << stream->getEndpoint().getIp() << std::endl;
    if(stream->waitRead(TIMEOUT)){
        unsigned int size = stream->getAvailableData();

        if(size > 20){
            sendStringTooBig(stream);
			std::cout << "client sent a invalid string!\n";
        }else{
			stream->read(input);
            password = input.getString(size);
            if(password == "wordpass"){
                sendResult(stream, true);
				std::cout << "client authenticated!\n";
            }else{
                sendResult(stream, false);
				std::cout << "client not authenticated!\n";
            }
        }
    }else{
        std::cout << "client timed out!";
    }
    stream->close();
}

void sendStringTooBig(snet::TcpStreamPtr stream){
    snet::NetworkMessage msg(256);

    msg.put<int>(0);
    msg.put<unsigned int>(15);
    msg.putString("String too big!");

    stream->send(msg);
}

void sendResult(snet::TcpStreamPtr stream, bool ok){
    snet::NetworkMessage msg(256);

    if(ok){
        msg.put<int>(1);
        msg.put<unsigned int>(8);
        msg.putString("Welcome!");
    }else{
        msg.put<int>(0);
        msg.put<unsigned int>(12);
        msg.putString("Not welcome!");
    }

    stream->send(msg);
}