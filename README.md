# MyThread Library
MyThread is a user-level thread library implemented in C, providing basic thread management functionalities. The library is designed for simplicity and ease of use, featuring thread creation, scheduling, yielding, sleeping, and exiting.

## Features

- **Thread Creation**: Create user-level threads using `myThreadCreate`.
- **Thread Scheduling**: The library includes a simple round-robin scheduler to switch between threads.
- **Thread Yielding**: Use `myThreadYield` to voluntarily yield the CPU to the next runnable thread.
- **Thread Sleeping**: Put a thread to sleep for a specified duration using `myThreadSleep`.
- **Thread Exiting**: Exit a thread using `myThreadExit`.


## Getting Started

### Prerequisites

- **ucontext.h**: Ensure that your system supports the `ucontext.h` library for context switching.

### Building

To build the project, compile the `main.c` file along with `mythread.c`:

```bash
gcc main.c mythread.c -o mythread_example
