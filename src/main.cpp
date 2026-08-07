#include "../include/RedisServer.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    int port = 6379; // default port
    if (argc > 1) port = stoi(argv[1]);

    RedisServer server(port);

    return 0;
}