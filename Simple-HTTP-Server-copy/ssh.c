#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gcc ssh.c -o ssh -lssh


int main(int argc, char *argv[]) {
    ssh_session my_ssh_session;
    int rc;
    char *hostname;
    char *username = "your_username";
    char *password = "your_password";

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        return EXIT_FAILURE;
    }

    hostname = argv[1];

    my_ssh_session = ssh_new();
    if (my_ssh_session == NULL) {
        fprintf(stderr, "Failed to create SSH session.\n");
        return EXIT_FAILURE;
    }

    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, hostname);

    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to host: %s\n", ssh_get_error(my_ssh_session));
        ssh_free(my_ssh_session);
        return EXIT_FAILURE;
    }

    rc = ssh_userauth_password(my_ssh_session, username, password);
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Authentication failed: %s\n", ssh_get_error(my_ssh_session));
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return EXIT_FAILURE;
    }

    ssh_channel channel = ssh_channel_new(my_ssh_session);
    if (channel == NULL) {
        fprintf(stderr, "Failed to create channel.\n");
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return EXIT_FAILURE;
    }

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        fprintf(stderr, "Failed to open session: %s\n", ssh_get_error(my_ssh_session));
        ssh_channel_free(channel);
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return EXIT_FAILURE;
    }

    rc = ssh_channel_request_shell(channel);
    if (rc != SSH_OK) {
        fprintf(stderr, "Failed to request shell: %s\n", ssh_get_error(my_ssh_session));
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return EXIT_FAILURE;
    }

    printf("Shell access granted. Type commands:\n");

    char buffer[1024];
    while (1) {
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        ssh_channel_write(channel, buffer, strlen(buffer));
        ssh_channel_read(channel, buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);
    }

    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);

    return EXIT_SUCCESS;
}
