#include "GameServer.h"

#include <algorithm>

#include "MessageType.h"

#include <ctime>

GameServerConfig::GameServerConfig() :
    this{"./games/"}
{
    
}

GameServerConfig::GameServerConfig(const std::string& configLocation) :
    configLocation{configLocation}
{
    // TODO, load the file, parse it, and save it into some class variables
    // TODO, add error handling to this part
}

std::string_view GameServerConfig::getGameConfigDir() const {
    return gameConfigDir;
}


// PUBLIC

GameServer::GameServer(int port, const std::string& htmlFile) :
    keepRunning{true},
    port{port},
    htmlFile{htmlFile},
    server{port, htmlFile,
        [this] (networking::Connection c) {
            this->sessionManager.addConnection(c);
        },
        [this] (networking::Connection c) {
            //this->sessionManager.removeConnection(c); Currently no implementation
        }},
    sessionManager{}
{
    
}

GameServer::GameServer(networking::Server& server, SessionManager& sessionManager) :
    server{std::move(server)},
    sessionManager{sessionManager},
    keepRunning{true}, port{-1}, htmlFile{""}
{
    
}

void GameServer::send(const std::deque<networking::Message>& messages) {
    server.send(messages);
}

void GameServer::sendTextTo(const std::vector<Connection>& connections, std::string text) {
    server.queueMessage(connections, text);
    server.sendQueuedMessages();
}

void GameServer::update() {
    server.update();
}

// HEAVY WIP TODO
std::string GameServer::receive() {
    auto incomingMessages = server.receive();
    std::ostringstream outgoingText;

    // for prepending each message with the time it was received
    time_t currentTime = time(0);
    tm *formatTime = localtime(&currentTime);

    // Check and deal with messages about creating/joining rooms or server shutdowns.
    std::vector<networking::Message> unhandledMessages;
    std::for_each(incomingMessages.front(), incomingMessages.back(),
        [this, &unhandledMessages] (networking::Message msg) {
            auto msgType = MessageType::interpretType(msg.text);
            switch (msgType) {
                case MessageType::Type::ServerStop:
                    this->keepRunning = false;
                    break;
                case MessageType::Type::CreateSession:
                    this->sessionManager.createNewSession();
                    break;
                case MessageType::Type::JoinSession:
                    //this->sessionManager.joinToSession(); Currently no implementation
                    break;
                case MessageType::Type::LeaveServer:
                    server.disconnect(c);
                default:
                    unhandledMessages.push_back(msg);
                    //outgoingText << "[" << formatTime->tm_hour << "] " << c.id << ": " << message.text << "\n";
                    break;
            }
        }
    );
    
    // Pass the remaining messages to SessionManager for distribution and handling
    //sessionManager.process(unhandledMessages); Currently no implementation
    return outgoingText.str();

}

void GameServer::update() {
    server.update();
}

int GameServer::getPort() const {
    return port;
}

bool GameServer::getKeepRunning() const {
    return keepRunning;
}

std::string_view GameServer::getHtmlFile() const {
    return htmlFile;
}

int main(int argc, char* argv[]) {

    while (keepRunning) {
        try {
            server.update();
        } catch (std::exception& e) {
            keepRunning = false;
        }

        auto outgoing = receive();
        sendTextTo(clients, outgoing);
        sleep(1);
    }

    return 0;

}
