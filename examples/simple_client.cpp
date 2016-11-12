#include <iostream>
#include <string>
#include <simplenet.hpp>

/*Example of a simple client application made with simplenet api*/

int main(){
    snet::NetworkMessage input, output;
    snet::TcpStream stream;
    std::string password;
    const unsigned int PASSWORD_MAX_SIZE = 20;
    const std::chrono::milliseconds TIMEOUT(10000);

    try{
#ifdef _WIN32
        snet::initWinApiSock();
#endif
        stream.open(snet::InternetAddress(AF_INET, "127.0.0.1", 80));

        std::cout << "connection stabilished!\n";
        std::cout << "Inform the password: ";
        std::cin >> password;

        if(stream.waitWrite(TIMEOUT)){
            if(password.size() > PASSWORD_MAX_SIZE){
                std::cout << "Password max length is " << PASSWORD_MAX_SIZE << ", the password suplied will be truncated to that size!\n";
                password.resize(PASSWORD_MAX_SIZE);
            }
            output.resize(password.size());
            output.putString(password);
            stream.send(output);

            if(stream.waitRead(TIMEOUT)){
                int status;
                std::string message;
                unsigned int msg_size;

                input.resize(stream.getAvailableData());
                stream.read(input);

                status = input.get<int>();
                msg_size = input.get<unsigned int>();
                message = input.getString(msg_size);

                if(status == 0){
                    std::cout << "Failed to authenticate: " << message << std::endl;
                }else if(status == 1){
                    std::cout << "Authenticated!\n";
                    std::cout << "Data from server:\n" << message << std::endl;
                }

            }else{
                std::cout << "Couldn't read data from server: TIMEOUT!\n";
            }

        }else{
            std::cout << "Couldn't send data to server!\n";
        }

    }catch(std::exception& ex){
        std::cout << ex.what() << std::endl;
    }

    stream.close();

#ifndef _WIN32
    snet::shutDownWinApiSock();
#endif

    return 0;
}