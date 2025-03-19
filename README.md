# C Shell

A simple command-line shell implementation in C that provides basic shell functionality.

## Features

- Command prompt showing current user, hostname, and working directory
- Command history support
- Built-in commands:
  - `cd` - Change to home directory
  - `cd <path>` - Change to specified directory
  - `exit` - Exit the shell
- Support for executing external commands
- Signal handling (Ctrl+C)
- Memory management and cleanup

## Requirements

- GCC compiler
- Unix-like operating system (Linux, macOS)
- Standard C libraries

## Building

To build the shell:

```bash
make
```

To clean the build:

```bash
make clean
```

## Usage

Run the shell:

```bash
./shell
```

## Example Commands

```bash
$ ls
$ pwd
$ cd /path/to/directory
$ echo "Hello, World!"
$ exit
```

## Notes

- Use Ctrl+C to interrupt a running command
- Use Ctrl+D to exit the shell
- The shell maintains command history in memory during the session
- Built with debugging symbols (-g) for easier debugging

## Implementation Details

The shell is implemented in C and uses the following system calls:
- `fork()` and `execvp()` for command execution
- `chdir()` for directory changes
- `getcwd()` for current directory
- `signal()` for signal handling
- `waitpid()` for process management