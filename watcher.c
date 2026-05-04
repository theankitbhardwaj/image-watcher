#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <string.h>
#define BUF_LEN (1024 * (sizeof(struct inotify_event) + 16))

const char *SRC_DIR = "/home/ankit/Downloads";
const char *DEST_DIR = "/home/ankit/Pictures";

int is_image(const char *filename) {
    return strstr(filename, ".jpg") ||
           strstr(filename, ".jpeg") ||
           strstr(filename, ".webp") ||
           strstr(filename, ".png");
}

void move_file(const char *filename) {
    char src[512];
    char dest[512];

    snprintf(src, sizeof(src), "%s/%s", SRC_DIR, filename);
    snprintf(dest, sizeof(dest), "%s/%s", DEST_DIR, filename);

    if (rename(src, dest) == 0) {
        printf("Moved: %s → %s\n", src, dest);
    } else {
        perror("rename");
    }
}

int main() {
  int fd = inotify_init1(IN_NONBLOCK);
  if (fd == -1) {
    perror("inotify_init1");
    exit(EXIT_FAILURE);
  }

  int wd = inotify_add_watch(fd, SRC_DIR, IN_CLOSE_WRITE | IN_MOVED_TO);
  if (wd < 0) {
      perror("inotify_add_watch");
      exit(1);
  }

  char buffer[BUF_LEN];

  while (1) {
    int length = read(fd, buffer, BUF_LEN);
    int i = 0;
    while (i < length) {
        struct inotify_event *event = (struct inotify_event *)&buffer[i];
        if (event->len) {
            if ((event->mask & IN_CLOSE_WRITE) || (event->mask & IN_MOVED_TO)) {

                if (is_image(event->name)) {
                    move_file(event->name);
                } else {
                    printf("Ignored: %s\n", event->name);
                }
            }
        }
        i += sizeof(struct inotify_event) + event->len;
    }
  }

  close(fd);

  return 0;
}
