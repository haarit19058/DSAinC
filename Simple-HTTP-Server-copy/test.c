#include <arpa/inet.h>   // For socket-related functions and structs (e.g., sockaddr_in)
#include <ctype.h>       // For character manipulation (e.g., tolower())
#include <dirent.h>      // For directory traversal functions (e.g., opendir, readdir)
#include <errno.h>       // For error number constants
#include <fcntl.h>       // For file control options (e.g., open)
#include <netinet/in.h>  // For Internet Protocol family
#include <pthread.h>     // For threading (e.g., pthread_create, pthread_detach)
#include <regex.h>       // For regular expressions (e.g., regcomp, regexec)
#include <stdbool.h>     // For boolean data type
#include <stdio.h>       // For input/output operations (e.g., printf)
#include <stdlib.h>      // For memory management and process control (e.g., malloc, exit)
#include <string.h>      // For string manipulation functions
#include <sys/socket.h>  // For socket operations (e.g., socket, bind, listen, accept)
#include <sys/stat.h>    // For file status information (e.g., stat)
#include <sys/types.h>   // For data types used in system calls
#include <unistd.h>      // For POSIX API (e.g., close, read, write)

#define PORT 8080          // Port number the server will listen on
#define BUFFER_SIZE 104857600  // Size of the buffer (100MB)

void *handle_client(void *arg) {
    int client_fd = *((int *)arg);
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);  // Receive request
    if (bytes_received > 0) {
        printf("Received request from client\n");
        log_message("Received request from client");

        regex_t regex_get, regex_post;
        regcomp(&regex_get, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
        regcomp(&regex_post, "^POST /([^ ]*) HTTP/1", REG_EXTENDED);
        regmatch_t matches[2];

        if (regexec(&regex_get, buffer, 2, matches, 0) == 0) {
            printf("GET request matched\n");
            log_message("GET request matched");

            buffer[matches[1].rm_eo] = '\0';
            const char *url_encoded_file_name = buffer + matches[1].rm_so;
            char *file_name = url_decode(url_encoded_file_name);

            char file_ext[32];
            strcpy(file_ext, get_file_extension(file_name));

            printf("File name: %s, File extension: %s\n", file_name, file_ext);
            log_message("File name and extension extracted");

            char *response = (char *)malloc(BUFFER_SIZE * 2 * sizeof(char));
            size_t response_len;
            build_http_response(file_name, file_ext, response, &response_len);

            send(client_fd, response, response_len, 0);
            printf("Response sent to client\n");
            log_message("Response sent to client");

            free(response);
            free(file_name);
        } else if (regexec(&regex_post, buffer, 2, matches, 0) == 0) {
            printf("\n\n%s\n\n", buffer);
            printf("POST request matched\n");
            log_message("POST request matched");

            buffer[matches[1].rm_eo] = '\0';
            const char *file_name = buffer + matches[1].rm_so;
            printf("File name: %s\n", file_name);
            log_message("File name extracted");

            FILE *uploaded_file = fopen(file_name, "wb");
            if (!uploaded_file) {
                perror("Error opening file for upload");
                log_message("Error opening file for upload");
                send(client_fd, "HTTP/1.1 500 Internal Server Error\r\n\r\n", 38, 0);
                close(client_fd);
                free(buffer);
                return NULL;
            }

            char *file_content_start = strstr(buffer, "\r\n\r\n");
            if (file_content_start) {
                file_content_start += 4;

                size_t bytes_to_write = bytes_received - (file_content_start - buffer);
                fwrite(file_content_start, 1, bytes_to_write, uploaded_file);
                printf("File content written to file\n");

                send(client_fd, "HTTP/1.1 200 OK\r\n\r\nFile uploaded successfully", 53, 0);
                log_message("File uploaded successfully");
            } else {
                send(client_fd, "HTTP/1.1 400 Bad Request\r\n\r\nInvalid POST request", 52, 0);
                log_message("Invalid POST request");
            }

            fclose(uploaded_file);
        } else {
            send(client_fd, "HTTP/1.1 400 Bad Request\r\n\r\nInvalid request", 47, 0);
            log_message("Invalid request");
        }
        regfree(&regex_get);
        regfree(&regex_post);
    }

    close(client_fd);
    free(arg);
    free(buffer);

    printf("Client connection closed\n");
    log_message("Client connection closed");

    return NULL;
}

int main() {
    // Create a pair of connected sockets
    int sockets[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == -1) {
        perror("Failed to create socket pair");
        return 1;
    }

    // Simulated POST request string
    const char *post_request =
        "POST /upload.txt HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 19\r\n"
        "\r\n"
        "This is file content";

    // Write the POST request to the mock client socket
    write(sockets[1], post_request, strlen(post_request));

    // Call the handle_client function with the server socket
    int *client_fd = malloc(sizeof(int));
    *client_fd = sockets[0];
    handle_client(client_fd);

    // Close the sockets
    close(sockets[0]);
    close(sockets[1]);

    return 0;
}
