#include "../include/RedisCommandHandler.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

// RESP parser 
// *2\r\n$4\r\n\PING\r\n$4\r\nTEST\r\n
// *2 -> Array of 2 elements
// $4 -> Length of the first element
// PING -> First element
// TEST -> Second element

vector<string> parseRespCommand(const string &input) {
    vector<string> tokens;
    if (input.empty()) return tokens;

    // If it doesn't start with '*', fallback to spliiting by whitespace
    if (input[0] != '*') {
        istringstream iss(input);
        string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    size_t pos = 0;
    // Expect '*' followed by number of elements
    if (input[pos] != '*') return tokens;
    pos++;  // skip '*'

    // crlf = Carriage Return (\r),  Line Feed (\n)
    size_t crlf = input.find("\r\n", pos);
    if (crlf == string::npos) return tokens;
    
    int numElements = stoi(input.substr(pos, crlf - pos));
    pos = crlf + 2; // skip "\r\n"

    for (int i = 0; i < numElements; i++) {
        if (pos >= input.size() || input[pos] != '$') break;
        pos++; // skip '$'        crlf = input.find("\r\n", pos);

        crlf = input.find("\r\n", pos);
        if (crlf == string::npos) break;
        int len = stoi(input.substr(pos, crlf - pos));
        pos = crlf + 2;

        if (pos + len > input.size()) break;
        string token = input.substr(pos, len);
        //tokens.push_back(token);
        pos += len + 2; // skip token and CRLF
    }

    return tokens;
}

RedisCommandHandler::RedisCommandHandler() {}

string RedisCommandHandler::processCommand(const string& commandLine) {
    // USe RESP parser
    auto tokens = parseRespCommand(commandLine);
    if (tokens.empty()) return "ERROR: Empty command\r\n";

    cout << commandLine << "\n";
    
    for (auto& t : tokens) {
        cout << t << "\n";
    }
    
    string cmd = tokens[0];
    transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
    ostringstream response;

    // Connect to database

    // Check commands

    return response.str();
}