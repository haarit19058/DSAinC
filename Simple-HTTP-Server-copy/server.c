#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8081
#define BUFFER_SIZE 104857600
#define LOG_FILE "server.log"

FILE *log_file;

void log_message(const char *message) {
    printf("%s\n", message);  // Print to terminal
    if (log_file) {
        fprintf(log_file, "%s\n", message);  // Log to file
        fflush(log_file);  // Ensure the log is written immediately
    }
}

const char *get_file_extension(const char *file_name) {
    const char *dot = strrchr(file_name, '.');
    if (!dot || dot == file_name) {
        return "";
    }
    return dot + 1;
}

const char *get_mime_type(const char *file_ext) {
    if (strcasecmp(file_ext, "html") == 0 || strcasecmp(file_ext, "htm") == 0) {
        return "text/html";
    } else if (strcasecmp(file_ext, "txt") == 0) {
        return "text/plain";
    } else if (strcasecmp(file_ext, "jpg") == 0 || strcasecmp(file_ext, "jpeg") == 0) {
        return "image/jpeg";
    } else if (strcasecmp(file_ext, "png") == 0) {
        return "image/png";
    } else {
        return "application/octet-stream";
    }
}

bool case_insensitive_compare(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return false;
        }
        s1++;
        s2++;
    }
    return *s1 == *s2;
}

char *get_file_case_insensitive(const char *file_name) {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return NULL;
    }

    struct dirent *entry;
    char *found_file_name = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (case_insensitive_compare(entry->d_name, file_name)) {
            found_file_name = entry->d_name;
            break;
        }
    }

    closedir(dir);
    return found_file_name;
}

char *url_decode(const char *src) {
    size_t src_len = strlen(src);
    char *decoded = malloc(src_len + 1);
    size_t decoded_len = 0;

    for (size_t i = 0; i < src_len; i++) {
        if (src[i] == '%' && i + 2 < src_len) {
            int hex_val;
            sscanf(src + i + 1, "%2x", &hex_val);
            decoded[decoded_len++] = hex_val;
            i += 2;
        } else {
            decoded[decoded_len++] = src[i];
        }
    }

    decoded[decoded_len] = '\0';
    return decoded;
}

void build_http_response(const char *file_name, const char *file_ext, char *response, size_t *response_len) {
    const char *mime_type = get_mime_type(file_ext);
    char *header = (char *)malloc(BUFFER_SIZE * sizeof(char));
    snprintf(header, BUFFER_SIZE,
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "\r\n",
             mime_type);

    int file_fd = open(file_name, O_RDONLY);
    if (file_fd == -1) {
        snprintf(response, BUFFER_SIZE,
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "404 Not Found");
        *response_len = strlen(response);
        return;
    }

    struct stat file_stat;
    fstat(file_fd, &file_stat);
    off_t file_size = file_stat.st_size;

    *response_len = 0;
    memcpy(response, header, strlen(header));
    *response_len += strlen(header);

    ssize_t bytes_read;
    while ((bytes_read = read(file_fd, response + *response_len, BUFFER_SIZE - *response_len)) > 0) {
        *response_len += bytes_read;
    }
    free(header);
    close(file_fd);
}

void list_files(char *response, size_t *response_len) {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        snprintf(response, BUFFER_SIZE,
                 "HTTP/1.1 500 Internal Server Error\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "Internal Server Error");
        *response_len = strlen(response);
        return;
    }

    struct dirent *entry;
    char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                   "<html><body><h1>File List</h1><ul>";
    strcpy(response, header);
    *response_len = strlen(header);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  // Only regular files
            char file_item[512];
            snprintf(file_item, sizeof(file_item), "<li><a href=\"/%s\">%s</a></li>", entry->d_name, entry->d_name);
            strcat(response, file_item);
            *response_len += strlen(file_item);
        }
    }

    // Add upload form at the end of the file list
    const char *upload_form = 
        "</ul><h1>Upload a File</h1>"
        "<form action=\"/upload\" method=\"POST\">"
        "<input type=\"file\" name=\"file\">"
        "<input type=\"submit\" value=\"Upload\">"
        "</form>";
    strcat(response, upload_form);
    *response_len += strlen(upload_form);

    strcat(response, "</body></html>");
    *response_len += strlen("</body></html>");
    closedir(dir);
}

void handle_file_upload(int client_fd, char *buffer, size_t bytes_received) {
    // Look for the file name and content in the request body
    // printf("%s",buffer);
    char *file_name_start = strstr(buffer, "Content-Disposition: form-data; name=\"file\"; filename=\"");
    if (file_name_start) {
        file_name_start += 44;  // Skip the "Content-Disposition: form-data; name=\"file\"; filename=\""
        char *file_name_end = strchr(file_name_start, '\"');
        if (file_name_end) {
            *file_name_end = '\0';  // Null terminate the file name
            char *file_name = file_name_start;  // The file name is now extracted

            // Find where the file content starts (after the headers)
            char *file_data_start = strstr(buffer, "\r\n\r\n");  // Skip headers
            if (file_data_start) {
                file_data_start += 4;  // Skip the two \r\n after headers

                // Save the file with the extracted name
                FILE *file = fopen(file_name, "wb");
                if (file) {
                    // Write the file content
                    fwrite(file_data_start, 1, bytes_received - (file_data_start - buffer), file);
                    fclose(file);
                    log_message("File uploaded successfully");
                } else {
                    log_message("Failed to save file");
                }
            }
        }
    }
}


void* handle_client(void *arg) {
    int client_fd = *((int *)arg);
    char *buffer = (char *)malloc(BUFFER_SIZE);

    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
    printf("%s",buffer);
    if (bytes_received > 0) {
        regex_t regex_get, regex_post;
        regcomp(&regex_get, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
        regcomp(&regex_post, "^POST /([^ ]*) HTTP/1", REG_EXTENDED);
        regmatch_t matches[2];

        if (regexec(&regex_get, buffer, 2, matches, 0) == 0) {
            buffer[matches[1].rm_eo] = '\0';
            const char *url_encoded_file_name = buffer + matches[1].rm_so;
            char *file_name = url_decode(url_encoded_file_name);

            if (strcmp(file_name, "home.html") == 0) {
                char *response = (char *)malloc(BUFFER_SIZE * 2);
                size_t response_len;
                list_files(response, &response_len);
                send(client_fd, response, response_len, 0);
                free(response);
            } else {
                char file_ext[32];
                strcpy(file_ext, get_file_extension(file_name));

                char *response = (char *)malloc(BUFFER_SIZE * 2);
                size_t response_len;
                build_http_response(file_name, file_ext, response, &response_len);
                send(client_fd, response, response_len, 0);
                free(response);
            }
            free(file_name);
        } else if (regexec(&regex_post, buffer, 2, matches, 0) == 0) {
            log_message("POST request received");

            // Handle file upload
            handle_file_upload(client_fd, buffer, bytes_received);

            // Send redirect response to home.html after upload
            const char *redirect_response =
                "HTTP/1.1 302 Found\r\n"
                "Location: /home.html\r\n"
                "\r\n";
            send(client_fd, redirect_response, strlen(redirect_response), 0);

        } else {
            send(client_fd, "HTTP/1.1 400 Bad Request\r\n\r\nInvalid request", 47, 0);
        }

        regfree(&regex_get);
        regfree(&regex_post);
    }

    close(client_fd);
    free(arg);
    free(buffer);
}

int main() {
    log_file = fopen(LOG_FILE, "a");
    if (!log_file) {
        perror("Could not open log file");
        exit(EXIT_FAILURE);
    }

    int server_fd;
    struct sockaddr_in server_addr;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log_message("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        log_message("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        log_message("listen failed");
        exit(EXIT_FAILURE);
    }

    log_message("Server started");

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));

        if ((*client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
            log_message("accept failed");
            continue;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)client_fd);
        pthread_detach(thread_id);
    }

    close(server_fd);
    fclose(log_file);
    return 0;
}
