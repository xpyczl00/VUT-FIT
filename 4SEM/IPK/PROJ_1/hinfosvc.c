#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define WELCOME_MSG "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n"
#define ERROR_MSG ""

int CPU_LOAD()
{
    long double a[7], b[7], loadavg;
    FILE *fp;

    fp = fopen("/proc/stat", "r");
    fscanf(fp, "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf", &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6]);
    fclose(fp);
    sleep(1);
    fp = fopen("/proc/stat", "r");
    fscanf(fp, "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf", &b[0], &b[1], &b[2], &b[3], &b[4], &b[5], &b[6]);
    fclose(fp);

    loadavg = ((b[0] + b[1] + b[2] + b[4] + b[5] + b[6]) - (a[0] + a[1] + a[2] + a[4] + a[5] + a[6])) / ((b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6]) - (a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6]));

    int output = (int)(round(loadavg * 1000));
    return (output);
}

bool NumericArg(const char *str)
{

    for (int i = 0, n = strlen(str); i < n; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{

    int rc;
    int server_socket;
    char cpu_load[4];
    sprintf(cpu_load, "%d%%", CPU_LOAD());
    int argument = atoi(argv[1]);

    // arg check 1 , 2
    if (argc != 2)
    {
        fprintf(stderr, "INCORRECT ARG COUNT.\n");
        return -1;
    }
    //checking if arg is numeric
    if (NumericArg(argv[1]) == false)
    {
        fprintf(stdout, "%d", argument);
        fprintf(stderr, "INCORRECT ARGUMENT FORMAT.\n");
        return -1;
    }

    //  creating socket

    if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    if ((setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) || (setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int)) < 0))
    {
        perror("ERROR: set");
        exit(EXIT_FAILURE);
    }

    // specify
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(argument);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind
    if ((rc = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address))) < 0)
    {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }

    // listen
    if ((rc = listen(server_socket, 1)) < 0)
    {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sa_client;
    socklen_t sa_client_len = sizeof(sa_client);

    // accept
    while (1)
    {
        // if succesfull
        int connect_socket = accept(server_socket, (struct sockaddr *)&sa_client, &sa_client_len);
        if (connect_socket >= 0)
        {
            send(connect_socket, WELCOME_MSG, strlen(WELCOME_MSG), 0);

            char buffer[1024];

            int res = 0;
            for (;;)
            {

                res = recv(connect_socket, buffer, 1024, 0);
                // response failed
                if (res < 0)
                {
                    perror("ERROR: no response");
                    close(connect_socket);
                    break;
                }
                else // response successful
                {
                    if (strstr(buffer, "GET /load "))
                    {
                        send(connect_socket, cpu_load, strlen(cpu_load), 0);
                        close(connect_socket);
                    }
                    else if (strstr(buffer, "GET /hostname "))
                    {

                        FILE *fp;
                        char hostname[1024];
                        fp = popen("cat /proc/sys/kernel/hostname", "r");
                        fgets(hostname, 1024, fp);
                        pclose(fp);
                        send(connect_socket, hostname, strlen(hostname), 0);
                        close(connect_socket);
                    }
                    else if (strstr(buffer, "GET /cpu-name "))
                    {
                        FILE *fp;
                        char cpuname[1024];
                        fp = popen("cat /proc/cpuinfo | grep -E 'model name[[:space:]]*:'| awk -F ': ' '{print $2}' |sort|uniq", "r");
                        fgets(cpuname, 1024, fp);
                        printf("%s", cpuname);
                        pclose(fp);
                        send(connect_socket, cpuname, strlen(cpuname), 0);
                        close(connect_socket);
                    }
                    else
                    {
                        perror("ERROR: UNKNOWN ARGUMENT");
                        // send error massage

                        close(connect_socket);
                    }
                    break;
                    // kodes more
                }
            }
        }
        else
        {
            send(connect_socket, ERROR_MSG, strlen(ERROR_MSG), 0);
            close(connect_socket);
            perror("ERROR: REQUEST ERROR");

            // send
        }
    }
}