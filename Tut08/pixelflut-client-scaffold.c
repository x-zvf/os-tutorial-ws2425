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
    // TODO: connect to server, return fd
}

void flood(int fd, uint32_t const *argbbuf, int w, int h, int x, int y) {
    //TODO: send pixels to server
    //      The (w * h) image should appear at (x, y) on the server
    //      argbbuf contains the loaded image in ARGB format (layed out 
    //      row-by-row). For example, pixel (x=0, y=1) is at argbbuf[width].
    //      If it were pure green, argbbuf[width] would be 0x__00ff00,
    //      where __ is the alpha channel, which can be ignored.
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
