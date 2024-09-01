// ISA projekt 2023: DNS resolver
// Lukasz Pycz
// xpyczl00



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <getopt.h>
#include <netdb.h>

#define DNS_SERVER_PORT 53 // DNS typically uses port 53

// DNS header structure
struct DNS_HEADER
{
    unsigned short id; // identification number

    unsigned char rd : 1;     // recursion desired
    unsigned char tc : 1;     // truncated message
    unsigned char aa : 1;     // authoritive answer
    unsigned char opcode : 4; // purpose of message
    unsigned char qr : 1;     // query/response flag

    unsigned char rcode : 4; // response code
    unsigned char cd : 1;    // checking disabled
    unsigned char ad : 1;    // authenticated data
    unsigned char z : 1;     // its z! reserved
    unsigned char ra : 1;    // recursion available

    unsigned short q_count;    // number of question entries
    unsigned short ans_count;  // number of answer entries
    unsigned short auth_count; // number of authority entries
    unsigned short add_count;  // number of resource entries
};
// Constant sized fields of query structure
struct QUESTION
{
    unsigned short qtype;
    unsigned short qclass;
};
// Structure of a Query
typedef struct
{
    unsigned char *name;
    struct QUESTION *ques;
} QUERY;

typedef struct
{
    int recursion;
    int reverse;
    int ipv6;
    const char *server;
    int port;
    char *address;
} DnsOptions;

// Function to parse command line arguments
void parseArguments(int argc, char *argv[], DnsOptions *options)
{
    int opt;

    while ((opt = getopt(argc, argv, "rx6s:p:")) != -1)
    {
        switch (opt)
        {
        case 'r':
            options->recursion = 1;
            break;
        case 'x':
            options->reverse = 1;
            break;
        case '6':
            options->ipv6 = 1;
            break;
        case 's':
            options->server = optarg;
            break;
        case 'p':
            options->port = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s [-r] [-x] [-6] -s server [-p port] adresa\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Check for the required address argument
    if (optind < argc)
    {
        options->address = argv[optind];
    }
    else
    {
        fprintf(stderr, "Missing address argument.\n");
        exit(EXIT_FAILURE);
    }
}

void print_dns_query(const unsigned char *dns_query, size_t query_length)
{
    for (size_t i = 0; i < query_length; i++)
    {
        printf("%02X ", dns_query[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");
}



// Function to convert a domain name to DNS format
void change_hostname_to_dns_query_name(char *query_name, char **address)
{
    unsigned int previous_index = 0;
    int length;

    strcpy(&query_name[1], *address);

    while ((length = strcspn(&query_name[previous_index + 1], ".")) != 0)
    {
        query_name[previous_index] = length;
        previous_index = previous_index + length + 1;
    }
    if (strlen(*address) != previous_index - 1)
        query_name[previous_index] = 0;
}

void reverse_IP(char *pIP)
{
    char seps[] = ".";
    char *token;
    char pIPSec[4][4];
    int i = 0;
    token = strtok(pIP, seps);
    while (token != NULL)
    {
        /* While there are "." characters in "string" */
        sprintf(pIPSec[i], "%s", token);
        /* Get next "." character: */
        token = strtok(NULL, seps);
        i++;
    }
    sprintf(pIP, "%s.%s.%s.%s.%s", pIPSec[3], pIPSec[2], pIPSec[1], pIPSec[0], "IN-ADDR.ARPA");
}

void reverse_string(char *str)
{
    /* skip null */
    if (str == 0)
    {
        return;
    }

    /* skip empty string */
    if (*str == 0)
    {
        return;
    }

    /* get range */
    char *start = str;
    char *end = start + strlen(str) - 1; /* -1 for \0 */
    char temp;

    /* reverse */
    while (end > start)
    {
        /* swap */
        temp = *start;
        *start = *end;
        *end = temp;

        /* move */
        ++start;
        --end;
    }
}

int main(int argc, char *argv[])
{
   
    DnsOptions options = {0}; // Initialize options struct with default values
    if (options.port == 0)
    {
        options.port = 53;
    }

    // Call the parseArguments function to populate the options struct
    parseArguments(argc, argv, &options);

   

    // Create a UDP socket for sending the DNS query
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed");
        return -1;
    }
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // Use IPv4
    hints.ai_socktype = SOCK_DGRAM; // Use UDP

    int status = getaddrinfo(options.server, NULL, &hints, &res);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DNS_SERVER_PORT);

    // Extract the resolved IP address and assign it to the server_addr.sin_addr
    struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
    server_addr.sin_addr = addr->sin_addr;

    // Construct a DNS query
    struct DNS_HEADER dns_header;


    // dns query setup and fill it according to DNS documentation
    char query[255];
    int query_len;

    char query_name[255];
    int index_after_query_name;

    unsigned short query_type;
    unsigned short query_class;

    if (options.ipv6 == 1)
        query_type = htons(28); // ipv6 type AAAA
    else if (options.reverse)
        query_type = htons(12);
    else
        query_type = htons(1); // A type

    query_class = htons(1); // internet

    dns_header.id = (unsigned short)htons(getpid());
    dns_header.qr = 0;
    dns_header.opcode = 0;
    dns_header.aa = 0;
    dns_header.tc = 0;
    if (options.recursion == 1)
    {
        dns_header.rd = 1;
    }
    else
        dns_header.rd = 0;
    dns_header.ra = 0;
    dns_header.z = 0;
    dns_header.ad = 0;
    dns_header.cd = 0;
    dns_header.rcode = 0;
    dns_header.q_count = htons(1);
    dns_header.ans_count = 0;
    dns_header.auth_count = 0;
    dns_header.add_count = 0;

    // set empty string of query name
    memset(query_name, '\0', 255);

    if (options.reverse == 1)
    {
        if (strchr(options.address, '.') != NULL)
        {
            char buf[255];
            int ret_func_val;
            ret_func_val = inet_pton(AF_INET, options.address, &buf);
            if (ret_func_val < 1)
                perror("DNS query sendto failed");

            char reversed_ip[255];
            strcpy(reversed_ip, options.address);
            reverse_IP(reversed_ip);
            char *p = &reversed_ip[0];
            change_hostname_to_dns_query_name(query_name, &p);
        }

        else if (strchr(options.address, ':') != NULL)
        {

            unsigned char addr[16];
            int ret_func_val;
            // check ipv6 address
            ret_func_val = inet_pton(AF_INET6, options.address, &addr);
            if (ret_func_val < 1)
                perror("DNS query sendto failed");

            // get long version of ipv6
            char long_ipv6[40];
            sprintf(long_ipv6, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                    (int)addr[0], (int)addr[1],
                    (int)addr[2], (int)addr[3],
                    (int)addr[4], (int)addr[5],
                    (int)addr[6], (int)addr[7],
                    (int)addr[8], (int)addr[9],
                    (int)addr[10], (int)addr[11],
                    (int)addr[12], (int)addr[13],
                    (int)addr[14], (int)addr[15]);

            // reverse and add .ip6.arpa
            char reversed_ip_6[255];
            reverse_string(long_ipv6);
            sprintf(reversed_ip_6,
                    "%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.%c.IP6.ARPA",
                    long_ipv6[0], long_ipv6[1], long_ipv6[2], long_ipv6[3], long_ipv6[5], long_ipv6[6], long_ipv6[7],
                    long_ipv6[8],
                    long_ipv6[10], long_ipv6[11], long_ipv6[12], long_ipv6[13], long_ipv6[15], long_ipv6[16],
                    long_ipv6[17], long_ipv6[18],
                    long_ipv6[20], long_ipv6[21], long_ipv6[22], long_ipv6[23], long_ipv6[25], long_ipv6[26],
                    long_ipv6[27], long_ipv6[28],
                    long_ipv6[30], long_ipv6[31], long_ipv6[32], long_ipv6[33], long_ipv6[35], long_ipv6[36],
                    long_ipv6[37], long_ipv6[38]);

            char *p = &reversed_ip_6[0];
            change_hostname_to_dns_query_name(query_name, &p);
        }
        else
        {
            perror("DNS query sendto failed");
        }
    }
    else
    {
        change_hostname_to_dns_query_name(query_name, &options.address);
    }

    // count index where to continue in putting data
    index_after_query_name = 12 + strlen(query_name) + 1;

    query_len = index_after_query_name + 4;

    memset(query, '\0', query_len);

    // copy data to query
    memcpy(query, &dns_header, 12);
    memcpy(&query[12], &query_name, strlen(query_name) + 1);
    memcpy(&query[index_after_query_name], &query_type, 2);
    memcpy(&query[index_after_query_name + 2], &query_class, 2);

    if (sendto(sockfd, query, query_len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("DNS query sendto failed");
        close(sockfd);
        return -1;
    }

    // receive response or exit
    unsigned char response_buffer[65536];
    socklen_t response_length;
    ssize_t bytes_recieved = recvfrom(sockfd, response_buffer, sizeof(response_buffer), 0, (struct sockaddr *)&server_addr,
                                      &response_length);

    if (bytes_recieved == -1)
    {
        perror("Error receiving DNS response");
    }
    else
    {
        // Process the DNS response here
        struct DNS_HEADER *dns_response = NULL;
        dns_response = (struct DNS_HEADER *)response_buffer;

        // check response code for errors
        if (ntohs(dns_response->rcode) == 1)
        {
            perror("Format error");
        }
        else if (ntohs(dns_response->rcode) == 2)
        {
            perror("Server failure");
        }
        else if (ntohs(dns_response->rcode) == 3)
        {
            perror("Name Error");
        }
        else if (ntohs(dns_response->rcode) == 4)
        {
            perror("Not Implemented");
        }
        else if (ntohs(dns_response->rcode) == 5)
        {
            perror("Refused");
        }
        // print dns query info
        if (dns_response->aa)
            printf("Authoritative: Yes, ");
        else
            printf("Authoritative: No, ");
        if (dns_response->ra && dns_response->rd)
            printf("Recursive: Yes, ");
        else
            printf("Recursive: No, ");
        if (dns_response->tc)
            printf("Truncated: Yes\n");
        else
            printf("Truncated: No\n");

        

        unsigned char *offset;
        int total_offset = 12; // header bytes
        char qname[256];
        char ipv6address[256];

        printf("Question section (%d)\n", ntohs(dns_response->q_count));
        for (int i = 0; i < ntohs(dns_response->q_count); i++)
        {
            offset = (unsigned char *)(response_buffer + total_offset);
           
            int qtype, qclass;
           
            //  Parse domain name

            offset = response_buffer + (offset - response_buffer);

            int length = *offset;
            int j = 0;

            while (length != 0)
            {
                offset++;
                for (int k = 0; k < length; k++)
                {
                    qname[j] = *offset;
                    offset++;
                    j++;
                }
                qname[j] = '.';
                j++;
                length = *offset;
            }
            qname[j - 1] = '\0';

            printf("%s ,", qname);
            offset++;

            qtype = (offset[0] << 8) | offset[1];
            offset += 2;
            qclass = (offset[0] << 8) | offset[1];
            offset += 2;
            // Print the type, class, and TTL
            if (qtype == 1)
            {
                printf("A, ");
            }
            else if (qtype == 5)
            {
                printf("CNAME, ");
            }
            else if (qtype == 28)
            {
                printf("AAAA, ");
            }
            else if (qtype == 12)
            {
                printf("PTR, ");
            }
            else
            {
                printf("Unknown Type %d, ", qtype);
            }
            if (qclass == 1)
                printf("IN, \n");

            total_offset += strlen(qname) + 6;

            
        }

        printf("Answer section (%d)\n", ntohs(dns_response->ans_count));
        for (int i = 0; i < ntohs(dns_response->ans_count); i++)
        {
            offset = (unsigned char *)(response_buffer + total_offset);
            int qtype, qclass;
            uint32_t ttl, rdlength;

            

            if ((offset[0] & 0xC0) == 0xC0)
            {

                int index = ((offset[0] & 0x3F) << 8) | offset[1];
                *offset = index;
                char qname[256];

                offset = response_buffer + index;

                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {
                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    length = *offset;
                }
                qname[j - 1] = '\0';
                printf("%s ,", qname);

                offset = response_buffer + total_offset + 2;
                total_offset += 2;

                // The domain name is not compressed.
            }
            else
            {
                char qname[256];

                offset = response_buffer + (offset - response_buffer);
                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {
                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    length = *offset;
                }
                qname[j - 1] = '\0';
                printf("%s ,", qname);
                offset++;
                total_offset += strlen(qname);

                // The domain name is not compressed.
            }

            qtype = (offset[0] << 8) | offset[1];
            offset += 2;
            qclass = (offset[0] << 8) | offset[1];
            offset += 2;

            // Parse the TTL
            ttl = (offset[0] << 24) | (offset[1] << 16) | (offset[2] << 8) | offset[3];
            offset += 4;

            rdlength = (offset[0] << 8) | offset[1];
            offset += 2;

            // Print the type, class, and TTL
            if (qtype == 1)
            {
                printf("A, ");
            }

            else if (qtype == 2)
            {
                printf("NS, ");
            }

            else if (qtype == 5)
            {
                printf("CNAME, ");
            }
            else if (qtype == 6)
            {
                printf("SOA, ");
            }
            else if (qtype == 28)
            {
                printf("AAAA, ");
            }
            else if (qtype == 12)
            {
                printf("PTR, ");
            }
            else
            {
                printf("Unknown Type %d, ", qtype);
            }
            if (qclass == 1)
                printf("IN, %d, ", ttl);

            // Print the data based on the type (A or CNAME)
            if (qtype == 1)
            {
                // Parse IPv4 address
                printf("%d.%d.%d.%d\n", offset[0], offset[1], offset[2], offset[3]);
                offset += rdlength;
            }

            else if (qtype == 5)
            {

                if ((offset[0] & 0xC0) == 0xC0)
                {
                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;
                    char qname[256];

                    offset = response_buffer + index;

                    int length = *offset;
                    int j = 0;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';
                    printf("%s \n", qname);

                    // The domain name is not compressed.
                }
                else
                {
                    char qname[256];

                    offset = response_buffer + (offset - response_buffer);
                    int length = *offset;
                    int j = 0;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';
                    printf("%s \n", qname);
                    offset++;
                    // The domain name is not compressed.
                }
            }

            else if (qtype == 28)
            {
                
                //  Parse and print IPv6 address
                for (int i = 0; i < 16; i++)
                {
                    if (i % 2 == 0 && i > 0)
                    {
                        printf(":");
                    }
                    printf("%02x", offset[i]);
                }
                printf("\n");
            }

            else if (qtype == 12)
            {
                char qname[256];

                offset = response_buffer + (offset - response_buffer);
                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {

                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    if ((offset[0] & 0xC0) == 0xC0)
                        break;

                    length = *offset;
                }
                if ((offset[0] & 0xC0) != 0xC0)
                {

                    qname[j - 1] = '\0';
                    printf("%s \n", qname);
                    offset++;
                }
                else
                {
                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;

                    offset = response_buffer + index;

                    int length = *offset;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';
                    printf("%s. \n", qname);
                }
            }

            total_offset += 10 + rdlength;
        }

        printf("Authority section (%d)\n", ntohs(dns_response->auth_count));
        for (int i = 0; i < ntohs(dns_response->auth_count); i++)
        {
            offset = (unsigned char *)(response_buffer + total_offset);
            int qtype, qclass;
            uint32_t ttl, rdlength;

            
            if ((offset[0] & 0xC0) == 0xC0)
            {

                int index = ((offset[0] & 0x3F) << 8) | offset[1];
                *offset = index;
                char qname[256];

                offset = response_buffer + index;

                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {
                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    length = *offset;
                }
                qname[j - 1] = '\0';
                printf("%s. ,", qname);

                offset = response_buffer + total_offset + 2;
                total_offset += 2;

            }
            else
            {
                char qname[256];

                offset = response_buffer + (offset - response_buffer);
                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {
                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    length = *offset;
                }
                qname[j - 1] = '\0';
                printf("%s. ,", qname);
                offset++;

                total_offset += strlen(qname) + 2;
             
            }

            qtype = (offset[0] << 8) | offset[1];
            offset += 2;
            qclass = (offset[0] << 8) | offset[1];
            offset += 2;

            // Parse the TTL
            ttl = (offset[0] << 24) | (offset[1] << 16) | (offset[2] << 8) | offset[3];
            offset += 4;

            rdlength = (offset[0] << 8) | offset[1];
            offset += 2;

            // Print the type, class, and TTL
            if (qtype == 1)
            {
                printf("A, ");
            }
            else if (qtype == 2)
            {
                printf("NS, ");
            }
            else if (qtype == 5)
            {
                printf("CNAME, ");
            }
            else if (qtype == 12)
            {
                printf("PTR, ");
            }
            else if (qtype == 6)
            {
                printf("SOA, ");
            }
            else if (qtype == 28)
            {
                printf("AAAA, ");
            }
            else
            {
                printf("Unknown Type %d, ", qtype);
            }
            if (qclass == 1)
                printf("IN, %d, ", ttl);

            // Print the data based on the type (A or CNAME)
            if (qtype == 1)
            {
                // Parse IPv4 address
                printf("%d.%d.%d.%d\n", offset[0], offset[1], offset[2], offset[3]);
                offset += rdlength;
            }
            else if (qtype == 5)
            {

                if ((offset[0] & 0xC0) == 0xC0)
                {
                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;
                    char qname[256];

                    offset = response_buffer + index;

                    int length = *offset;
                    int j = 0;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';
                    printf("%s \n", qname);

                }
                else
                {
                    char qname[256];

                    offset = response_buffer + (offset - response_buffer);
                    int length = *offset;
                    int j = 0;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';
                    printf("%s \n", qname);
                    offset++;
                }
            }

            else if (qtype == 28)
            {
                //  Parse and print IPv6 address
                for (int i = 0; i < 16; i++)
                {
                    if (i % 2 == 0 && i > 0)
                    {
                        printf(":");
                    }
                    printf("%02x", offset[i]);
                }
                printf("\n");
            }

            else if (qtype == 2 && options.reverse == 1)
            {

                char qname[256];

                offset = response_buffer + (offset - response_buffer);
                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {

                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    if ((offset[0] & 0xC0) == 0xC0)
                    {

                        break;
                    }

                    length = *offset;
                }
                if ((offset[0] & 0xC0) != 0xC0)
                {

                    qname[j - 1] = '\0';

                    printf("%s.\n", qname);


                    offset++;
                }
                else
                {

                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;

                    offset = response_buffer + index;

                    int length = *offset;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            ipv6address[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        ipv6address[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';

                    ipv6address[j - 1] = '\0';
                    printf("%s.\n", qname);
                   
                }
            }

            else if (qtype == 2 && options.reverse != 1)
            {
                char qname[256];

                offset = response_buffer + (offset - response_buffer);
                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {

                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    if ((offset[0] & 0xC0) == 0xC0)
                        break;

                    length = *offset;
                }
                if ((offset[0] & 0xC0) != 0xC0)
                {

                    qname[j - 1] = '\0';
                    printf("%s.\n", qname);
                    offset++;
                }
                else
                {
                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;

                    offset = response_buffer + index;

                    int length = *offset;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';
                    printf("%s.\n", qname);
                }
            }

            else if (qtype == 6)
            {
                char qname[256];

                offset = response_buffer + (offset - response_buffer);
                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {

                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    if ((offset[0] & 0xC0) == 0xC0)
                        break;

                    length = *offset;
                }
                if ((offset[0] & 0xC0) != 0xC0)
                {

                    qname[j - 1] = '\0';
                    printf("%s. \n", qname);
                    offset++;
                }
                else
                {
                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;

                    offset = response_buffer + index;

                    int length = *offset;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';
                    printf("%s. \n", qname);
                }
            }

            else if (qtype == 12)
            {
                char qname[256];

                offset = response_buffer + (offset - response_buffer);
                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {

                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    if ((offset[0] & 0xC0) == 0xC0)
                        break;

                    length = *offset;
                }
                if ((offset[0] & 0xC0) != 0xC0)
                {

                    qname[j - 1] = '\0';
                    printf("%s \n", qname);
                    offset++;
                }
                else
                {
                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;

                    offset = response_buffer + index;

                    int length = *offset;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';
                    printf("%s. \n", qname);
                }
            }

            total_offset += 10 + rdlength;
        }

        printf("Additional section (%d)\n", ntohs(dns_response->add_count));

        for (int i = 0; i < ntohs(dns_response->add_count); i++)
        {
            offset = (unsigned char *)(response_buffer + total_offset);
            int qtype, qclass;
            uint32_t ttl, rdlength;
            
            

            if ((offset[0] & 0xC0) == 0xC0)
            {
                if (options.reverse == 1)
                {

                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;

                    offset = response_buffer + index;

                    int length = *offset;
                    int j = 0;

                    offset++;
                    for (int k = 0; k < length; k++)
                    {
                        ipv6address[j] = *offset;
                        offset++;
                        j++;
                    }
                    ipv6address[j] = '.';

                    printf("%s. ,", ipv6address);

                    offset = response_buffer + total_offset + 2;
                    total_offset += 2;

                }

                else
                {
                     int index = ((offset[0] & 0x3F) << 8) | offset[1];
                *offset = index;
                char qname[256];

                offset = response_buffer + index;

                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {
                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    length = *offset;
                }
                qname[j - 1] = '\0';
                printf("%s. ,", qname);

                offset = response_buffer + total_offset + 2;
                total_offset += 2;
                }
            }
            else
            {
                char qname[256];

                offset = response_buffer + (offset - response_buffer);
                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {

                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    if ((offset[0] & 0xC0) == 0xC0)
                        break;

                    length = *offset;
                }
                if ((offset[0] & 0xC0) != 0xC0)
                {

                    qname[j - 1] = '\0';
                    printf("%s\n", qname);
                    offset++;
                }
                else
                {
                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;

                    offset = response_buffer + index;

                    int length = *offset;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';
                    printf("%s\n", qname);
                    total_offset += 4;
                }
            }

            qtype = (offset[0] << 8) | offset[1];
            offset += 2;
            qclass = (offset[0] << 8) | offset[1];
            offset += 2;

            // Parse the TTL
            ttl = (offset[0] << 24) | (offset[1] << 16) | (offset[2] << 8) | offset[3];
            offset += 4;

            rdlength = (offset[0] << 8) | offset[1];
            offset += 2;

            // Print the type, class, and TTL
            if (qtype == 1)
            {
                printf("A, ");
            }
            else if (qtype == 2)
            {
                printf("NS, ");
            }
            else if (qtype == 5)
            {
                printf("CNAME, ");
            }
            else if (qtype == 12)
            {
                printf("PTR, ");
            }
            else if (qtype == 6)
            {
                printf("SOA, ");
            }
            else if (qtype == 28)
            {
                printf("AAAA, ");
            }
            else if (qtype == 15)
            {
                printf("MX ,");
            }
            else
            {
                printf("Unknown Type %d, ", qtype);
            }
            if (qclass == 1)
                printf("IN, %d, ", ttl);

            // Print the data based on the type (A or CNAME)
            if (qtype == 1)
            {
                // Parse IPv4 address
                printf("%d.%d.%d.%d\n", offset[0], offset[1], offset[2], offset[3]);
                offset += 4;
            }
            else if (qtype == 28)
            {
                //  Parse and print IPv6 address
                for (int i = 0; i < 16; i++)
                {
                    if (i % 2 == 0 && i > 0)
                    {
                        printf(":");
                    }
                    printf("%02x", offset[i]);
                }
                printf("\n");
            }
            else if (qtype == 5)
            {
                // Parse domain name for CNAME
                int cname_offset = (offset[0] << 8) | offset[1];
                printf("%s\n", qname + cname_offset);
                offset += 2;
            }
            else if (qtype == 2 && options.reverse == 1)
            {

                char qname[256];

                offset = response_buffer + (offset - response_buffer);
                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {

                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    if ((offset[0] & 0xC0) == 0xC0)
                    {

                        break;
                    }

                    length = *offset;
                }
                if ((offset[0] & 0xC0) != 0xC0)
                {

                    qname[j - 1] = '\0';
                    printf("%s.\n", qname);

                    offset++;
                }
                else
                {

                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;

                    offset = response_buffer + index;

                    int length = *offset;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';

                    printf("%s.\n", qname);
                    // might be a problem because its compression plus strlen imo
                }
            }

            else if (qtype == 2 && options.reverse != 1)
            {
                char qname[256];

                offset = response_buffer + (offset - response_buffer);
                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {

                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    if ((offset[0] & 0xC0) == 0xC0)
                        break;

                    length = *offset;
                }
                if ((offset[0] & 0xC0) != 0xC0)
                {

                    qname[j - 1] = '\0';
                    printf("%s\n", qname);
                    offset++;
                }
                else
                {
                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;

                    offset = response_buffer + index;

                    int length = *offset;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';
                    printf("%s.\n", qname);
                }
            }

            else if (qtype == 6)
            {
                char qname[256];

                offset = response_buffer + (offset - response_buffer);
                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {

                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    if ((offset[0] & 0xC0) == 0xC0)
                        break;

                    length = *offset;
                }
                if ((offset[0] & 0xC0) != 0xC0)
                {

                    qname[j - 1] = '\0';
                    printf("%s. \n", qname);
                    offset++;
                }
                else
                {
                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;

                    offset = response_buffer + index;

                    int length = *offset;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';
                    printf("%s. \n", qname);
                }
            }

            else if (qtype == 15)
            {
                char qname[256];

                offset = response_buffer + (offset - response_buffer);
                int length = *offset;
                int j = 0;

                while (length != 0)
                {
                    offset++;
                    for (int k = 0; k < length; k++)
                    {

                        qname[j] = *offset;
                        offset++;
                        j++;
                    }
                    qname[j] = '.';
                    j++;
                    if ((offset[0] & 0xC0) == 0xC0)
                        break;

                    length = *offset;
                }
                if ((offset[0] & 0xC0) != 0xC0)
                {

                    qname[j - 1] = '\0';
                    printf("%s. \n", qname);
                    offset++;
                }
                else
                {
                    int index = ((offset[0] & 0x3F) << 8) | offset[1];
                    *offset = index;

                    offset = response_buffer + index;

                    int length = *offset;

                    while (length != 0)
                    {
                        offset++;
                        for (int k = 0; k < length; k++)
                        {
                            qname[j] = *offset;
                            offset++;
                            j++;
                        }
                        qname[j] = '.';
                        j++;
                        length = *offset;
                    }
                    qname[j - 1] = '\0';
                    printf("%s. \n", qname);
                }
            }

            total_offset += 10 + rdlength;
        }

        close(sockfd);
    }

    return 0;
}