# Neural Network with Multi-Processing and IPC

## Overview
This project implements a simple feedforward neural network using multi-processing in C++. It leverages `fork()`, inter-process communication (IPC) with named pipes (`mkfifo()`), and semaphores for synchronization. Each layer's computations are performed in separate processes, enhancing parallelism.

## Features
- **Multi-processing:** Each layer is handled by a separate process using `fork()`.
- **Inter-process communication (IPC):** Named pipes (`mkfifo()`) facilitate data transfer between layers.
- **Process Synchronization:** Semaphores manage concurrent access to shared resources.
- **Backpropagation:** Implements a backtracking mechanism for updating weights.
- **Command Execution:** Uses `execvp()` to execute a separate program (`hfile`) for layer computations.

## Prerequisites
Ensure you have the following installed on your system:
- GCC/G++ compiler
- POSIX-compliant system (Linux/macOS)
- Required libraries for semaphores and IPC (`sys/types.h`, `sys/ipc.h`, `sys/sem.h`)

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/neural-network-mp-ipc.git
   cd neural-network-mp-ipc
   ```
2. Compile the main program:
   ```sh
   g++ -o neural_net main.cpp -pthread
   ```
3. Compile the layer execution file (`hfile`):
   ```sh
   g++ -o hfile hfile.cpp
   ```

## Usage
Run the main program:
```sh
./neural_net
```

## Code Structure
- **main.cpp**: Initializes the neural network, forks child processes, and manages IPC.
- **hfile.cpp**: Handles individual layer computations.
- **Makefile**: (Optional) Automates the build process.

## Potential Issues & Debugging
- **Process synchronization issues**: Ensure semaphores are correctly initialized.
- **Zombie processes**: Use `waitpid()` to clean up child processes.
- **IPC data corruption**: Ensure pipes are read/written in a controlled manner.

## Future Improvements
- Implement shared memory (`shmget`, `shmat`) instead of named pipes for faster IPC.
- Optimize memory management by properly freeing allocated memory.
- Improve debugging mechanisms using logging.

## Development
Developed by Humayun Abdullah and Furqan sharjeel
