#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef void (response_function) (long long, char);

#define REQ_PIPE "REQ_PIPE_34379"
#define RESP_PIPE "RESP_PIPE_34379"



int ping(long long pipe_resp, long long pipe_req);
int exit_program(long long pipe_resp, long long pipe_req);


unsigned long long read_size(long long pipe_req)
{
    unsigned long long size;
    char byte_read;
    long long bytes_read;

    bytes_read = read(pipe_req, &byte_read, 1);
    if (bytes_read != 1)
        printf("ERROR"), putchar('\n'), printf("could not read from request pipe"), putchar('\n'), exit(1);

    return (size = byte_read);
}

void write_size(long long pipe_resp, unsigned long long size)
{
    long long bytes_written;
    char char_size = (char) size;
    
    bytes_written = write(pipe_resp, &char_size, 1);
    if (bytes_written != 1)
        printf("ERROR"), putchar('\n'), printf("could not write to the response pipe"), putchar('\n'), exit(1);
}

void read_messege(long long pipe_req, char* messege)
{
    long long bytes_read;
    unsigned long long i;

    i = 0;
    while (i < 255)
    {
        bytes_read = read(pipe_req, messege + i, 1);
        if (bytes_read != 1) 
            printf("ERROR"), putchar('\n'), printf("could not read from request pipe"), putchar('\n'), exit(1);
        if (messege[i] == '#')
            break;
        i++;
    }
    messege[i] = '\0';
}

void write_messege(long long pipe_resp, char* request_name_messege)
{
    unsigned long long size = strlen(request_name_messege);
    unsigned long long i;
    long long bytes_written;

    i = 0;
    while (i < size)
    {
        bytes_written = write(pipe_resp, request_name_messege + i, 1);
        if (bytes_written != 1)
            printf("ERROR"), putchar('\n'), printf("could not write to the response pipe"), putchar('\n'), exit(1);
        i++;
    }

    write(pipe_resp, "#", 1);
}

void write_error(long long pipe_resp, char* error_messege)
{
    unsigned long long size;
    long long bytes_written;

    write_size(pipe_resp, (size = strlen(error_messege)));

    bytes_written = write(pipe_resp, error_messege, size);
    if (bytes_written != size)
        printf("ERROR"), putchar('\n'), printf("could not write to the response pipe"), putchar('\n'), exit(1);

    
}

void write_success(long long pipe_resp, char* success_messege)
{
    unsigned long long size;
    long long bytes_written;

    write_size(pipe_resp, (size = strlen(success_messege)));

    bytes_written = write(pipe_resp, success_messege, size);
    if (bytes_written != size)
        printf("ERROR"), putchar('\n'), printf("could not write to the response pipe"), putchar('\n'), exit(1);

}

int main(int argc, char** argv)
{
    static int first_call = 1;
    static long long pipe_resp = -1, pipe_req = -1;
    char messege[1000];
    int function_index = -1;


    if (first_call == 1)
    {
        long long status;

        status  = mkfifo(RESP_PIPE, 0777);
        if (status == -1)
            printf("ERROR"), putchar('\n'), printf("could not create the response pipe"), putchar('\n'), exit(1);

        status = open(REQ_PIPE, O_RDONLY);
        if (status < 0)
            printf("ERROR"), putchar('\n'), printf("could not open the request pipe"), putchar('\n'), exit(1);
        else
            pipe_req = status;
        
        status = open(RESP_PIPE, O_WRONLY);
        if (status < 0)
            printf("ERROR"), putchar('\n'), printf("could not open the response pipe"), putchar('\n'), exit(1);
        else
            pipe_resp = status;

        write_messege(pipe_resp, "HELLO");

        first_call = 0;
    }

    read_messege(pipe_req, messege);

    if (strncmp(messege, "VARIANT", 7) == 0){
        ping(pipe_resp, pipe_req);
    }      
    if (strncmp(messege, "EXIT", 4) == 0)
        function_index = 1;
    if (function_index == -1)
        return 0;

    main(argc, argv);
}

int ping(long long pipe_resp, long long pipe_req)
{
    unsigned int number_to_write;
    long long bytes_written;
    
    write_messege(pipe_resp, "VARIANT");
    write_messege(pipe_resp, "VALUE");

    number_to_write = 34379;
    bytes_written = write(pipe_resp, &number_to_write, sizeof(unsigned int));
    if (bytes_written != sizeof(number_to_write))
    {
        printf("ERROR"), putchar('\n'), printf("could not write to the response pipe"), putchar('\n');
        return 0;
    }
       
    return 1;
}


