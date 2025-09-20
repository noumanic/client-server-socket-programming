# Client-Server Socket Programming

A comprehensive collection of socket programming implementations in C++ demonstrating various client-server communication patterns and interactive applications.

## Repository Information
- **Repository**: [client-server-socket-programming](https://github.com/noumanic/client-server-socket-programming)
- **Author**: [noumanic](https://github.com/noumanic)

## Overview

This repository contains two distinct socket programming implementations:

1. **Task 1**: Basic Chat Application
2. **Task 2**: Interactive Tic-Tac-Toe Game

Both implementations demonstrate fundamental concepts of network programming including socket creation, binding, listening, connecting, and bidirectional communication between client and server processes.

## Project Structure

```
├── i210416_task1_client.cpp      # Chat application client
├── i210416_task1_server.cpp      # Chat application server
├── i210416_task2_client.cpp      # Tic-Tac-Toe game client
├── i210416_task2_server.cpp      # Tic-Tac-Toe game server
├── i210416_task1_terminals_output.png  # Task 1 execution screenshots
├── i210416_task2_terminals_output.png  # Task 2 execution screenshots
├── A_i210416_LAB5.pdf           # Report document
└── README.md                    # This file
```

## Task 1: Basic Chat Application

### Description
A simple client-server chat application that allows real-time text communication between two processes.

### Features
- **Client (`i210416_task1_client.cpp`)**:
  - Connects to server on port 8080
  - Sends user messages to server
  - Receives acknowledgments from server
  - Graceful exit with 'quit' or 'exit' commands

- **Server (`i210416_task1_server.cpp`)**:
  - Listens on port 8080
  - Accepts single client connection
  - Echoes received messages back to client
  - Handles client disconnection

### Technical Implementation
- **Protocol**: TCP (SOCK_STREAM)
- **Address Family**: IPv4 (AF_INET)
- **Port**: 8080
- **Communication**: Bidirectional text messaging
- **Error Handling**: Comprehensive error checking for socket operations

### Usage
1. Compile both files:
   ```bash
   g++ -o server i210416_task1_server.cpp
   g++ -o client i210416_task1_client.cpp
   ```

2. Run server first:
   ```bash
   ./server
   ```

3. Run client in another terminal:
   ```bash
   ./client
   ```

4. Type messages in client terminal and press Enter
5. Type 'quit' or 'exit' to end the conversation

## Task 2: Interactive Tic-Tac-Toe Game

### Description
A networked implementation of the classic Tic-Tac-Toe game where the client plays against an AI server.

### Features
- **Client (`i210416_task2_client.cpp`)**:
  - Connects to game server on port 8081
  - Interactive game interface
  - Position-based move input (1-9)
  - Real-time game state display
  - Play again functionality

- **Server (`i210416_task2_server.cpp`)**:
  - Game server with AI opponent
  - Complete TicTacToe game logic implementation
  - Random move generation for AI
  - Win/draw detection
  - Multiple game sessions support
  - Client plays as 'X', Server plays as 'O'

### Game Logic Features
- **Board Representation**: 3x3 grid with position mapping (1-9)
- **Move Validation**: Checks for valid positions and occupied cells
- **Win Detection**: Rows, columns, and diagonals
- **Draw Detection**: Full board without winner
- **AI Strategy**: Random move selection from available positions
- **Game State Management**: Reset and replay functionality

### Technical Implementation
- **Protocol**: TCP (SOCK_STREAM)
- **Address Family**: IPv4 (AF_INET)
- **Port**: 8081
- **Communication**: Structured game state exchange
- **Error Handling**: Input validation and connection management

### Usage
1. Compile both files:
   ```bash
   g++ -o game_server i210416_task2_server.cpp
   g++ -o game_client i210416_task2_client.cpp
   ```

2. Run game server:
   ```bash
   ./game_server
   ```

3. Run game client:
   ```bash
   ./game_client
   ```

4. Follow on-screen instructions to play
5. Enter positions 1-9 to make moves
6. Type 'quit' to exit anytime

## Technical Details

### Socket Programming Concepts Demonstrated

1. **Socket Creation**: Using `socket()` system call
2. **Address Structures**: `sockaddr_in` for IPv4 addressing
3. **Port Binding**: `bind()` for server socket binding
4. **Listening**: `listen()` for incoming connections
5. **Connection Handling**: `accept()` for client connections
6. **Data Transmission**: `send()` and `recv()` for communication
7. **Socket Options**: `SO_REUSEADDR` for address reuse
8. **Error Handling**: Comprehensive error checking and reporting

### Network Configuration
- **Local Development**: Both applications use localhost (127.0.0.1)
- **Port Assignment**: 
  - Task 1: Port 8080
  - Task 2: Port 8081
- **Protocol**: TCP for reliable communication
- **Address Family**: IPv4

### Compilation Requirements
- **Compiler**: g++ (GNU C++ Compiler)
- **Standard**: C++11 or later
- **Libraries**: Standard C++ libraries, POSIX socket libraries
- **Platform**: Linux/Unix systems (uses POSIX socket API)

### Compilation Commands
```bash
# Task 1 - Chat Application
g++ -o chat_server i210416_task1_server.cpp
g++ -o chat_client i210416_task1_client.cpp

# Task 2 - Tic-Tac-Toe Game
g++ -o game_server i210416_task2_server.cpp
g++ -o game_client i210416_task2_client.cpp
```

## Screenshots
- `i210416_task1_terminals_output.png`: Task 1 execution demonstration
- `i210416_task2_terminals_output.png`: Task 2 execution demonstration

## Learning Objectives

This project demonstrates:
- **Network Programming Fundamentals**: Socket creation, binding, listening, and communication
- **Client-Server Architecture**: Request-response and interactive communication patterns
- **Error Handling**: Robust error checking and graceful failure handling
- **Game Development**: Real-time interactive applications over network
- **C++ Programming**: Object-oriented design, STL containers, and system programming

## Future Enhancements

Potential improvements for this project:
- Multi-client support for chat application
- Advanced AI strategies for Tic-Tac-Toe
- GUI implementation using graphics libraries
- Database integration for game statistics
- Web-based interface using web sockets
- Encryption for secure communication

## License

This project is part of a Computer Networks Lab assignment and is intended for educational purposes.

## Contact

- **GitHub**: [noumanic](https://github.com/noumanic)
- **Repository**: [client-server-socket-programming](https://github.com/noumanic/client-server-socket-programming)
