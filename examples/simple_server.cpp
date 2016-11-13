#include <iostream>
#include <string>
#include <simplenet.hpp>

class SimpleServer : public snet::TcpAcceptor{
public:
    static const std::chrono::milliseconds TIMEOUT;

    void onAccept(snet::TcpStreamPtr stream);
    void error(ErrorStage errStage, snet::SocketError& error);
private:
    void sendStringTooBig(snet::TcpStreamPtr stream);
    void sendResult(snet::TcpStreamPtr stream, bool ok);
};

int main(){
    SimpleServer server;

    try{
#ifdef _WIN32
        snet::initWinApiSock();
#endif
        server.bind(80, 1);
        while(server.listen());
    }catch(std::exception& ex){
        std::cout << "Error: " << ex.what() << std::endl;
    }

#ifdef _WIN32
        snet::shutDownWinApiSock();
#endif

    return 0;
}

const std::chrono::milliseconds SimpleServer::TIMEOUT = std::chrono::milliseconds(10000);

void SimpleServer::onAccept(snet::TcpStreamPtr stream){
    std::string password;
    snet::NetworkMessage input;

    std::cout << "New connection from " << stream->getEndpoint().getIp() << std::endl;
    if(stream->waitRead(TIMEOUT)){
        unsigned int size = stream->getAvailableData();

        if(size > 20){
            sendStringTooBig(stream);
        }else{
            password = input.getString(size);
            if(password == "wordpass"){
                sendResult(stream, true);
            }else{
                sendResult(stream, false);
            }
        }
    }else{
        std::cout << "Client timed out!";
    }
    stream->close();
}

void SimpleServer::sendStringTooBig(snet::TcpStreamPtr stream){
    snet::NetworkMessage msg(256);

    msg.put<int>(0);
    msg.put<unsigned int>(15);
    msg.putString("String too big!");

    stream->send(msg);
}

void SimpleServer::sendResult(snet::TcpStreamPtr stream, bool ok){
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