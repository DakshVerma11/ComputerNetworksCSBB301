#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define OUTPUT_PATH "/path/to/save/received_image.jpg"  // Path where the image will be saved

int main() {
    int sfd, file_fd;
    char buff[1024];
    struct sockaddr_in server;
    ssize_t bytesReceived, bytesWritten;

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
    inet_aton("192.168.0.140", &server.sin_addr);  // Server's IP address

    // Connect to server
    if (connect(sfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection failed.\n");
        return -1;
    }

    // Open file to write the received image
    file_fd = open(OUTPUT_PATH, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (file_fd < 0) {
        printf("Failed to open output file for saving the image.\n");
        close(sfd);
        return -1;
    }

    // Receive the image file in chunks
    while ((bytesReceived = read(sfd, buff, sizeof(buff))) > 0) {
        bytesWritten = write(file_fd, buff, bytesReceived);
        if (bytesWritten < 0) {
            printf("Error writing data to file.\n");
            break;
        }
    }

    printf("Image received and saved successfully.\n");

    close(file_fd);
    close(sfd);
    return 0;
}










