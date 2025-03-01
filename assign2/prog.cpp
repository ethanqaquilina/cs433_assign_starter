
/**
 * Assignment 2: Simple UNIX Shell
 * @file pcbtable.h
 * @author Ethan Aquilina (TODO: your name)
 * @brief This is the main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @version 0.1
 */
// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <sys/wait.h>

using namespace std;

#define MAX_LINE 80 // The maximum length command

/**
 * @brief parse out the command and arguments from the input command separated by spaces
 *
 * @param command
 * @param args
 * @return int Number of arguments parsed
 */
int parse_command(char command[], char *args[])
{
    int count = 0;
    char *pointer = strtok(command, " ");

    while (pointer != NULL)
    {
        args[count] = pointer;
        count++;
        pointer = strtok(NULL, " ");
    }
    args[count] = NULL;
    return count;
}
/**
 * @brief The main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[])
{
    char command[MAX_LINE];           // The command entered by the user
    char last_command[MAX_LINE] = ""; // Store last executed command
    char *args[MAX_LINE / 2 + 1];     // Holds parsed out command line arguments
    int should_run = 1;               // Flag to determine when to exit program

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);

        // Read the input command
        fgets(command, MAX_LINE, stdin);
        command[strcspn(command, "\n")] = '\0'; // Remove newline character

        // Check if user typed "!!"
        if (strcmp(command, "!!") == 0)
        {
            if (strlen(last_command) == 0)
            {
                printf("No command history found.\n");
                continue;
            }
            else
            {
                printf("%s\n", last_command);  // Print the last command
                strcpy(command, last_command); // Copy last command to command buffer
            }
        }
        else
        {
            strcpy(last_command, command); // Store command in history before parsing
        }

        // Check if command should run in the background
        bool background = false;
        int len = strlen(command);

        if (len > 0 && command[len - 1] == '&') // If '&' is the last character
        {
            background = true;
            command[len - 1] = '\0'; // Remove "&" from command
        }

        // Parse the input command
        int num_args = parse_command(command, args);
        if (num_args == 0)
            continue; // Ignore empty input

        // Check for "exit" command
        if (strcmp(args[0], "exit") == 0)
        {
            should_run = 0;
            continue;
        }

        // Check for input/output redirection
        int fd;
        bool redirect_output = false;
        bool redirect_input = false;
        char *filename = NULL;

        for (int i = 0; i < num_args; i++)
        {
            if (strcmp(args[i], ">") == 0)
            { // Output Redirection
                redirect_output = true;
                filename = args[i + 1];
                args[i] = NULL; // Remove ">" and filename from args
                break;
            }
            else if (strcmp(args[i], "<") == 0)
            { // Input Redirection
                redirect_input = true;
                filename = args[i + 1];
                args[i] = NULL; // Remove "<" and filename from args
                break;
            }
        }

        // Fork a child process
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Fork failed");
            return 1;
        }
        else if (pid == 0)
        {
            if (redirect_output)
            { // Handle output redirection
                fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0)
                {
                    perror("Error opening file for output redirection");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (redirect_input)
            { // Handle input redirection
                fd = open(filename, O_RDONLY);
                if (fd < 0)
                {
                    perror("Error opening file for input redirection");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            // Child process
            if (execvp(args[0], args) == -1)
            {
                perror("Command not found");
                exit(1);
            }
        }
        else
        { // Parent process
            if (!background)
            {
                wait(NULL);
            }
        }
    }
    return 0;
}
