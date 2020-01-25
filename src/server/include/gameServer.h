#ifndef SERVER_H
#define SERVER_H

class Server {
public:
    Server();
    ~Server();

    void start();
    void shutdown();

    void onConnect();
    void onDisconnect();
    void receiveMessage();
    void sendMessage();

private:
    std::vector<Engine> games;
};

#endif