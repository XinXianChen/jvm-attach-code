#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        return -1;
    }
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "tmp.sock");
    int ret = bind(fd, (struct sockaddr *) &addr, sizeof(addr));
    if (ret == -1) {
        perror("bind error");
        exit(-1);
    }
    if (listen(fd, 5) == -1) {
        perror("listen error");
        exit(-1);
    }

    int accept_fd;
    char buf[100];

    while (1) {
        if ((accept_fd = accept(fd, NULL, NULL)) == -1) {
            perror("accept error");
            continue;
        }

        while ((ret = read(accept_fd, buf, sizeof(buf))) > 0) {
            printf("receive %u bytes: %s\n", ret, buf);
        }
        if (ret == -1) {
            perror("read");
            exit(-1);
        } else if (ret == 0) {
            printf("EOF\n");
            close(accept_fd);
        }
    }
}
