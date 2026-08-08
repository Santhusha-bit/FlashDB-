#include "../include/RedisServer.h"
#include "../include/RedisCommandHandler.h"

#include <iostream>
#ifdef _WIN32

    // -------------------------
    // Windows
    // -------------------------
    // Windows uses Winsock instead of POSIX sockets.
    #include <winsock2.h>
    #include <ws2tcpip.h>

#else

    // -------------------------
    // Linux / macOS
    // -------------------------
    // Both Linux and macOS use POSIX socket APIs.
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>

#endif  // _WIN32

using namespace std;

static RedisServer* globalServer = nullptr;

RedisServer::RedisServer(int port) : port(port), server_socket(-1), running(true) {
    globalServer = this;
}

void RedisServer::shutdown() {
    running = false;
    if (server_socket != -1) {
        close(server_socket);
        server_socket = -1;
    }
    cout << "Redis Server Shutdown Successfully!\n";
}

void RedisServer::run() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        cerr << "Failed to create server socket!\n";
        return;
    }
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in server_addr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Failed to bind server socket!\n";
        return;
    }
    if (listen(server_socket, 10) < 0) {
        cerr << "Redis Server Started Successfully!\n";
        return;
    }
    
    cout << "Redis Server Listening on port " << port << "\n";

    vector<thread> threads;
    RedisCommandHandler cmdHandler;

    while (running) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0) {
            if (running) 
                cerr << "Error accepting client connection!\n";
            break;
        }

        threads.emplace_back([client_socket, &cmdHandler]() {
            char buffer[1024];
            while (true) {
                memset(buffer, 0, sizeof(buffer));
                int bytes  = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
                if (bytes <= 0) break;
                string request(buffer, bytes);
                string response = cmdHandler.processCommand(request);
                send(client_socket, response.c_str(), response.size(), 0);
            }
            close(client_socket);
        });

        for (auto& thread : threads) {
            if (t.joinable()) t.join();
        }

        // Shutdown
        
    }
}