# pipex

A C program that replicates the behavior of the shell's pipe (`|`) operator, executing two commands with input and output redirection.

## Overview

The `pipex` project is part of the 42 curriculum and focuses on process creation, file redirection, and inter-process communication using pipes in Unix systems. It simulates the behavior of:

```sh
< infile cmd1 | cmd2 > outfile

## Features

- Executes two commands in a pipeline, similar to the shell's `|` operator.
- Handles input and output file redirection:
  - `< infile` → Reads input from a file.
  - `> outfile` → Writes output to a file.
- Uses system calls such as:
  - `pipe()` → Creates a pipe for inter-process communication.
  - `fork()` → Spawns child processes.
  - `dup2()` → Redirects input/output file descriptors.
  - `execve()` → Executes commands.
  - `waitpid()` → Waits for child processes to finish.
- Supports both absolute and relative command paths.
- Works with environment variables (`PATH`).
- Proper error handling for invalid commands or files.
