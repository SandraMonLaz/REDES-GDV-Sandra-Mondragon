#include "Chat.h"


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char* tmp = _data;
    memcpy(tmp, &type, sizeof(uint8_t));

    tmp += sizeof(uint8_t);
    memcpy(tmp, nick.c_str(), 8*sizeof(char));

    tmp += 8*sizeof(char);
    memcpy(tmp, message.c_str(), 80 * sizeof(char));
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    //Reconstruir la clase usando el buffer _data
    char* tmp = _data;
    memcpy(&type, tmp, sizeof(uint8_t));

    tmp += sizeof(uint8_t);

    char bufferNick[8];
    memcpy(&bufferNick, tmp, 8*sizeof(char));
    nick = bufferNick;
    
    tmp += 8*sizeof(char);

    char bufferMessage[80];
    memcpy(&bufferMessage, tmp, 80 * sizeof(char));
    message = bufferMessage;

    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void ChatServer::addClient(Socket* clientSocket, ChatMessage* message){
    std::unique_ptr<Socket> loginRequest(clientSocket);
    clients.emplace_back(std::move(loginRequest));

    std::cout << "Join: " << message->nick << "\n";
}

void ChatServer::removeClient(Socket* clientSocket, ChatMessage* message){
    std::vector<std::unique_ptr<Socket>>::iterator it = clients.begin();

    while(it != clients.end()){
        if(*it->get() == *clientSocket) break;
        it++;
    } 

    if(it != clients.end()) clients.erase(it);

    std::cout << "Removed: " << message->nick << "\n";
}

void ChatServer::broadcastMessage(Socket* clientSocket, ChatMessage* message){

    for(int i = 0; i < clients.size(); ++i){
        if(*clientSocket == *clients[i].get()) continue;
        socket.send(*message, *clients[i]);  
    }
}
void ChatServer::do_messages()
{
    while (true)
    {
        /*
         * NOTA: los clientes están definidos con "smart pointers", es necesario
         * crear un unique_ptr con el objeto socket recibido y usar std::move
         * para añadirlo al vector
         */
        ChatMessage* message = new ChatMessage();
        Socket* clientSocket;

        int error = socket.recv(*message, clientSocket);
        if(error == -1){
            std::cout << "Error en do_message\n";
        }

        switch (message->type)
        {
            case ChatMessage::LOGIN:    addClient(clientSocket, message);                    break;
            case ChatMessage::LOGOUT:   removeClient(clientSocket, message);                 break;
            case ChatMessage::MESSAGE:  broadcastMessage(clientSocket, message);             break;      
            default:                                                                         break;
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatClient::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void ChatClient::logout()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGOUT;

    socket.send(em, socket);
}

void ChatClient::input_thread()
{
    while (true)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        ChatMessage msg;
        msg.type = ChatMessage::MESSAGE;
        msg.nick = nick;

        std::getline(std::cin, msg.message);
        socket.send(msg,socket);
    }
}

void ChatClient::net_thread()
{
    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        ChatMessage msg;
        socket.recv(msg);

        std::cout << msg.nick << ": " << msg.message << "\n";
    }
}

