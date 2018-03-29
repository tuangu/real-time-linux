#include <fcntl.h>
#include <unistd.h>


int main(void) {
    int user_desc = STDIN_FILENO;
    int file_flags;
    char ch;
    ssize_t ret;

    // fcntl(user_desc, F_SETFL, fcntl(user_desc, F_GETFL) | O_NONBLOCK);

    while (1) {
        ret = read(user_desc, &ch, 1);
        if (ret > 0) {
            write(STDOUT_FILENO, &ch, 1);    
        }
    }
}
