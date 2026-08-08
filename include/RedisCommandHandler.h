#ifndef REDIS_COMMAND_HANDLER_H
#define REDIS_COMMAND_HANDLER_H

#include <string>

class RedisCommandHandler {
    public:
        RedisCommandHandler();
        //Process comand from a client and return RESP-formatted response
        string processCommand(const string& commandLine);
};

#endif