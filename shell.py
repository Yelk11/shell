#!/usr/bin/env python3

import os
import sys
import subprocess
import readline
import signal
from typing import List, Optional

class Shell:
    def __init__(self):
        self.history_file = os.path.expanduser("~/.shell_history")
        self.setup_history()
        self.running = True

    def setup_history(self):
        """Setup command history functionality"""
        readline.parse_and_bind('tab: complete')
        if os.path.exists(self.history_file):
            readline.read_history_file(self.history_file)

    def save_history(self):
        """Save command history to file"""
        readline.write_history_file(self.history_file)

    def get_prompt(self) -> str:
        """Generate the command prompt"""
        cwd = os.getcwd()
        user = os.getenv('USER', 'user')
        hostname = os.uname().nodename
        return f"{user}@{hostname}:{cwd}$ "

    def execute_command(self, command: str) -> bool:
        """Execute a command and return True if successful"""
        try:
            # Handle built-in commands
            if command == "exit":
                self.running = False
                return True
            elif command == "cd":
                os.chdir(os.path.expanduser("~"))
                return True
            elif command.startswith("cd "):
                path = command[3:].strip()
                os.chdir(path)
                return True

            # Execute external commands
            process = subprocess.Popen(
                command,
                shell=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
            stdout, stderr = process.communicate()
            
            if stdout:
                print(stdout, end='')
            if stderr:
                print(stderr, end='', file=sys.stderr)
            
            return process.returncode == 0
        except Exception as e:
            print(f"Error executing command: {e}", file=sys.stderr)
            return False

    def run(self):
        """Main shell loop"""
        print("Welcome to the Python Shell!")
        print("Type 'exit' to quit")
        
        while self.running:
            try:
                command = input(self.get_prompt()).strip()
                if command:
                    self.execute_command(command)
            except KeyboardInterrupt:
                print("\nUse 'exit' to quit")
            except EOFError:
                print("\nGoodbye!")
                self.running = False
            except Exception as e:
                print(f"Error: {e}", file=sys.stderr)

        self.save_history()

def main():
    shell = Shell()
    shell.run()

if __name__ == "__main__":
    main() 