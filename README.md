# Command-Line Task Management System

This is a **C-based productivity tracker** that allows users to manage tasks directly from the terminal. Users can add, view, edit, delete, and mark tasks as completed. Tasks are saved in a text file, with date validation to ensure deadlines are today or in the future.

## Features

- Add tasks with a title and due date (DD-MM-YYYY format)
- View all tasks with status (Pending/Completed) and overdue warning
- Edit task title and date
- Delete tasks by ID
- Mark tasks as completed
- Daily summary of total, completed, and pending tasks
- Terminal-based, no GUI required

## How It Works

- Tasks are stored in a text file (`tasks.txt`) in CSV format.
- Includes date validation and checks for overdue tasks.
- Data is automatically saved after any modification.

## Requirements

- C compiler (e.g., GCC)
- Terminal or command-line environment

## How to Run

1. Compile the code:

```bash
gcc task_manager.c -o task_manager

