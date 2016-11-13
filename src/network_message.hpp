#ifndef _NETWORK_MESSAGE_HPP_
#define _NETWORK_MESSAGE_HPP_

#include <algorithm>
#include <memory>
#include <string>

#include "socket_base.hpp"

namespace snet{
	/*A network message is a simple buffer to write and read data*/
	class NetworkMessage{
	public:

		NetworkMessage(){
			buffer = nullptr;
			buffSize = position = length = 0;
		}

		NetworkMessage(unsigned int size){
			resize(size);
		}

		NetworkMessage(const char* buffer, unsigned int size){
			resize(size);
			put(buffer, size);
		}

		NetworkMessage(const NetworkMessage& networkMessage){
			resize(networkMessage.length);
			put(networkMessage.buffer, networkMessage.buffSize);
			position = networkMessage.position;
		}

		NetworkMessage(NetworkMessage&& networkMessage){
			buffer = networkMessage.buffer;
			length = networkMessage.length;
			buffSize = networkMessage.buffSize;
			position = networkMessage.position;
			networkMessage.buffer = nullptr;
		}

		~NetworkMessage(){
			if (buffer)
				delete buffer;
		}

		void setSender(const InternetAddress& sender){
			this.sender = sender;
		}

		void resize(unsigned int size, bool copy = true){
			char* new_buffer = new char[size];

			if (buffer != nullptr){
				if (copy){
					length = std::min<unsigned int>(length, size);
					position = std::min<unsigned int>(length, position);
					memcpy(new_buffer, buffer, length);
				}
				else
					clear();
				delete buffer;
			}
			buffSize = size;
			buffer = new_buffer;
		}

		void put(const char* buffer, unsigned int size){
			size = std::min<unsigned int>(size, (buffSize - position));
			memcpy((this->buffer + position), buffer, size);
			length += size;
			position += size;
		}

		template<typename type>
		void put(type buffer){
			put((const char*)&buffer, sizeof(buffer));
		}

		void putString(const std::string& str){
			put(str.data(), str.length() + 1);//plus one to include the '\0'
		}

		template<typename type>
		type get(){
			return *reinterpret_cast<type*>(get(sizeof(type)));
		}

		std::string getString(unsigned int length){
			return std::string(get(length));
		}

		void clear(){
			length = 0;
			position = 0;
		}

		bool isFull()const{
			return buffSize == length;
		}

		char* data()const{ return buffer; }
		void setPosition(unsigned int position){ this->position = std::min<unsigned int>(position, length); }
		void skipBytes(unsigned int skippedBytes){ setPosition(position + skippedBytes); }
		unsigned int getPosition()const{ return position; }
		unsigned int getbuffSize()const{ return buffSize; }
		unsigned int getLength()const{ return length; }
		InternetAddress& getSender(){ return sender; }
		const InternetAddress getSender()const{ return sender; }

		NetworkMessage& operator=(NetworkMessage networkMessage){
			std::swap(buffer, networkMessage.buffer);
			std::swap(buffSize, networkMessage.buffSize);
			std::swap(length, networkMessage.length);
			std::swap(position, networkMessage.position);
		}

	private:
		char* get(unsigned int type_size){
			char* ref = (buffer + position);
			skipBytes(type_size);
			return ref;
		}

		char* buffer;
		unsigned int length;	//number of bytes being used
		unsigned int buffSize;	//buffer size
		unsigned int position;	//position of write/read operation

		InternetAddress sender;
	};
	typedef std::shared_ptr<NetworkMessage> NetworkMessagePtr;
};

#endif
