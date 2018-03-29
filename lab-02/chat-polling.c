#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>

int OpenChatFellow();

int main(int argc, char *argv[]) {
    int user_desc = STDIN_FILENO;
    int fellow_desc;
    int file_flags;
    char ch;
    ssize_t ret;

    fellow_desc = OpenChatFellow();
    file_flags = fcntl(fellow_desc, F_GETFL); // read current file flags
    file_flags = file_flags | O_NONBLOCK; // add O_NONOBLOCK bit
    fcntl(fellow_desc, F_SETFL, file_flags);

    file_flags = fcntl(user_desc, F_GETFL);
    file_flags = file_flags | O_NONBLOCK;
    fcntl(user_desc, F_SETFL, file_flags);

    while(1) {
        ret = read(user_desc, &ch, 1);
        if (ret > 0) {
            if (ch == 'Q') {
                printf("Exitting\n");
                close(fellow_desc);
                return 0;
            }

            write(STDOUT_FILENO, &ch, 1);
        }

        ret = read(fellow_desc, &ch, 1);
        if (ret > 0) {
            write(STDOUT_FILENO, &ch, 1);
        }
    }

}
