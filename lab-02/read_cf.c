#include <fcntl.h>
#include <unistd.h>

int OpenChatFellow();

int main(void) {
    int fellow_desc;
    int file_flags;
    char ch;
    ssize_t ret;

    fellow_desc = OpenChatFellow();
    // fcntl(fellow_desc, F_SETFL, fcntl(fellow_desc, F_GETFL) | O_NONBLOCK);

    while (1) {
        ret = read(fellow_desc, &ch, 1);
        if (ret > 0) {
            write(STDOUT_FILENO, &ch, 1);    
        }
    }

    close(fellow_desc);
}
