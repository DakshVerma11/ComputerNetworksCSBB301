




#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define IMAGE_PATH "/home/nitdelhi/Downloads/Walterserverside.webp"  // Path to the image you want to send

int main() {
    int sfd, cfd, file_fd;
    fd_set rset;
    char buff[1024];
    struct sockaddr_in server;
    ssize_t bytesRead, bytesSent;
    
    // Create socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        printf("Socket creation failed.\n");
        return -1;
    }

    // Configure server
    bzero(&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    inet_aton("192.168.0.140", &server.sin_addr);

    // Bind socket
    if (bind(sfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Binding failed. Port may already be in use.\n");
        return -1;
    }

    // Listen for connections
    listen(sfd, 7);

    printf("Server is listening on 192.168.0.140:5000...\n");

    // Accept connection from client
    cfd = accept(sfd, NULL, NULL);
    if (cfd < 0) {
        printf("Failed to accept connection.\n");
        return -1;
    }

    // Open image file to send
    file_fd = open(IMAGE_PATH, O_RDONLY);
    if (file_fd < 0) {
        printf("Failed to open image file.\n");
        close(cfd);
        close(sfd);
        return -1;
    }

    // Send the image file in chunks
    while ((bytesRead = read(file_fd, buff, sizeof(buff))) > 0) {
        bytesSent = write(cfd, buff, bytesRead);
        if (bytesSent < 0) {
            printf("Error sending data to client.\n");
            break;
        }
    }

    printf("Image sent successfully.\n");

    close(file_fd);
    close(cfd);
    close(sfd);
    return 0;
}

