/* 
 * PixelFlut client scaffold using Imlib2
 * Compile with `gcc -o pc file.c -lImlib2`
 * Usage: ./pc hostip port file_path x y repeat
 *        hostip: IP address of the server
 *        port: port of the server (should be 1337)
 *        file_path: path to the image file
 *        x, y: coordinates of the image on the server
 *        repeat: how many times to send the image (0 for infinite)
 * (c) 2023 Péter Bohner
 * License: MIT
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/tcp.h>
#include<Imlib2.h>

uint32_t *get_pixelbuf(char *file_path, int *width, int *height) {
    Imlib_Image image = imlib_load_image(file_path);
    if (!image) {
        printf("Error: cannot load image\n");
        exit(1);
    }
    imlib_context_set_image(image);
    *width = imlib_image_get_width();
    *height = imlib_image_get_height();
    
    uint32_t *argbbuf = imlib_image_get_data_for_reading_only();
    if (!argbbuf) {
        printf("Error: cannot get image data\n");
        exit(1);
    }
    return argbbuf;
}

int connect_to_server(char *host, int port) {
    int fd = socket(AF_INET, SOCK_STREAM, SOL_TCP);
    if (fd < 0) {
        printf("Error: cannot create socket\n");
        exit(1);
    }
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
    };
    inet_pton(AF_INET, host, &addr.sin_addr);
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("Error: cannot connect to server\n");
        exit(1);
    }
    return fd;
}

void flood(int fd, uint32_t const *argbbuf, int w, int h, int x, int y) {
    //char buf[256];
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            uint32_t pixel = argbbuf[i * w + j];
            int r = (pixel >> 16) & 0xff;
            int g = (pixel >> 8) & 0xff;
            int b = pixel & 0xff;
            //int len = sprintf(buf, "PX %d %d %02x%02x%02x\n", x + j, y + i, r, g, b);
            //write(fd, buf, len);
            dprintf(fd, "PX %d %d %02x%02x%02x\n", x + j, y + i, r, g, b);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        printf("Usage: %s hostip port file_path x y repeat\n", argv[0]);
        return 1;
    }
    char *host = argv[1];
    int port = atoi(argv[2]);
    char *file_path = argv[3];
    int x = atoi(argv[4]);
    int y = atoi(argv[5]);
    int repeat = atoi(argv[6]);

    int width, height;
    uint32_t *argbbuf = get_pixelbuf(file_path, &width, &height);
    int fd = connect_to_server(host, port);

    for (int i = 0; i < repeat || repeat == 0; i++) {
        flood(fd, argbbuf, width, height, x, y);
    }
    return 0;
}
