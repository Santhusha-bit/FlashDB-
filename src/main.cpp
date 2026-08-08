#include "../include/RedisServer.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]) {
    int port = 6379; // default port
    if (argc > 1) port = stoi(argv[1]);

    RedisServer server(port);

    // Background persistence -> dump the database every 300 seconds (5 * 60s save database to disk)
    thread persistanceThread([]() {
        while(true) {
            this_thread::sleep_for(chrono::seconds(300));
            // dump the database 
        }
    });

    persistanceThread.detach();
    server.run();
    
    return 0;
}