#include <stdio.h>

void print_FILE_info(FILE *file);
void print_FILE_info_before_io(FILE *file);

int main(int argc, char *argv[]) {
    FILE *test_file = fopen("fd.c", "r");

    print_FILE_info_before_io(test_file);
    
    printf("===============\nFILE info of stdout\n");
    print_FILE_info_before_io(stdout);

    printf("===============\n");
     
    print_FILE_info(test_file);
    
    printf("===============\nFILE info of stdout\n");
    print_FILE_info(stdout);

    return 0;
}

void print_FILE_info(FILE *file) {
    int ret = fgetc(file);

    printf("File descriptor: %d\n", file->_fileno);
    printf("Size of buffer: %ld\n", file->_IO_buf_end - file->_IO_buf_base);
    printf("_IO_buf_end: 0x%x\n",(unsigned int) file->_IO_buf_end);
    printf("_IO_buf_base: 0x%x\n", (unsigned int) file->_IO_buf_base);

    int unbuffer = file->_flags & _IO_UNBUFFERED;
    int linebuffer = file->_flags & _IO_LINE_BUF;

    if (unbuffer)
        printf("Buffering mode: unbuffered\n");
    else if (linebuffer)
        printf("Buffering mode: line buffered\n");
    else
        printf("Buffering mode: fully buffered\n");
}

void print_FILE_info_before_io(FILE *file) {

    printf("File descriptor: %d\n", file->_fileno);
    printf("Size of buffer: %ld\n", file->_IO_buf_end - file->_IO_buf_base);
    printf("_IO_buf_end: 0x%x\n",(unsigned int) file->_IO_buf_end);
    printf("_IO_buf_base: 0x%x\n", (unsigned int) file->_IO_buf_base);

    int unbuffer = file->_flags & _IO_UNBUFFERED;
    int linebuffer = file->_flags & _IO_LINE_BUF;

    if (unbuffer)
        printf("Buffering mode: unbuffered\n");
    else if (linebuffer)
        printf("Buffering mode: line buffered\n");
    else
        printf("Buffering mode: fully buffered\n");
}
