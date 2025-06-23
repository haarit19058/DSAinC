#include <libssh/libssh.h>
#include <libssh/server.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define AUTH_PASSWORD "password"

void handle_client(ssh_session session) {
    ssh_message message;
    int auth_success = 0;

    while (!auth_success) {
        message = ssh_message_get(session);
        if (!message) break;

        if (ssh_message_type(message) == SSH_REQUEST_AUTH &&
            ssh_message_subtype(message) == SSH_AUTH_METHOD_PASSWORD) {
            const char *password = ssh_message_auth_password(message);
            if (strcmp(password, AUTH_PASSWORD) == 0) {
                ssh_message_auth_reply_success(message, 0);
                auth_success = 1;
            } else {
                ssh_message_auth_set_methods(message, SSH_AUTH_METHOD_PASSWORD);
                ssh_message_reply_default(message);
            }
        } else {
            ssh_message_reply_default(message);
        }
        ssh_message_free(message);
    }

    if (!auth_success) {
        printf("Authentication failed\n");
        ssh_disconnect(session);
        return;
    }

    ssh_channel channel = ssh_channel_new(session);
    if (channel == NULL) {
        fprintf(stderr, "Failed to create channel\n");
        return;
    }

    if (ssh_channel_open_session(channel) != SSH_OK) {
        fprintf(stderr, "Failed to open session channel\n");
        ssh_channel_free(channel);
        return;
    }

    if (ssh_channel_request_shell(channel) != SSH_OK) {
        fprintf(stderr, "Failed to request shell\n");
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return;
    }

    printf("Client connected. Handling session...\n");

    char buffer[1024];
    int nbytes;
    while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, nbytes, stdout);
        ssh_channel_write(channel, buffer, nbytes);
    }

    ssh_channel_close(channel);
    ssh_channel_free(channel);
}

int main() {
    ssh_bind sshbind;
    ssh_session session;
    int rc;

    sshbind = ssh_bind_new();
    if (sshbind == NULL) {
        fprintf(stderr, "Failed to create ssh_bind\n");
        return EXIT_FAILURE;
    }

    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDADDR, "0.0.0.0");
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT, "2222");
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_HOSTKEY, "ssh_host_rsa_key");

    if (ssh_bind_listen(sshbind) < 0) {
        fprintf(stderr, "Error starting SSH server: %s\n", ssh_get_error(sshbind));
        ssh_bind_free(sshbind);
        return EXIT_FAILURE;
    }

    printf("SSH server listening on port 2222...\n");

    while (1) {
        session = ssh_new();
        if (session == NULL) {
            fprintf(stderr, "Failed to create session\n");
            continue;
        }

        rc = ssh_bind_accept(sshbind, session);
        if (rc != SSH_OK) {
            fprintf(stderr, "Error accepting connection: %s\n", ssh_get_error(sshbind));
            ssh_free(session);
            continue;
        }

        printf("New client connected\n");
        handle_client(session);

        ssh_disconnect(session);
        ssh_free(session);
    }

    ssh_bind_free(sshbind);
    return EXIT_SUCCESS;
}
