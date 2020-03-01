/*
    tc64monitor
    Copyright (C) 2020  Filippo Bergamasco

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <errno.h>
#include "tc64monitor.h"

#undef VERBOSE
#define VERSION "0.1"

void* c64mem = 0;
int pid = 0;


unsigned long int str2hex( char* str, int strlen ) {

    unsigned long int val=0;

    while( *str && strlen-- ) {

        val *= 16;
        if( *str >= 'A' && *str<='F' ) {
            val += (*str - 'A')+10;
        }
        else if( *str >= 'a' && *str<='f' ) {
            val += (*str - 'a')+10;
        }
        else if( *str >= '0' && *str<='9' ) {
            val += (*str - '0');
        }
        else {
            printf("Invalid HEX: %c\n", *str );
            return 0;
        }

        ++str;
    }

    return val;
}


void dopoke()
{
    yylex();
    if( t.type != HEXNUM )
    {
        printf("Error: address expected\n");
        return;
    }
    unsigned long int addr = str2hex( t.str, t.len );

    void* c64memaddr = c64mem + addr;
#ifdef VERBOSE
    printf("POKE at addr $%04x (0x%lx)\n", addr,c64memaddr );
#endif

    unsigned char buff[256]={0};
    size_t bufflen=0;

    yylex();

    do
    {
        if( t.type != HEXBYTE )
        {
            printf("Error: byte expected\n");
            return;
        }
        buff[bufflen++] = (unsigned char)str2hex( t.str, t.len );
        yylex();

    } while( t.type != ENDCOMMAND && bufflen<256 );

#ifdef VERBOSE
    printf("Poking %s\n", buff);
#endif

    // Build iovec structs
    struct iovec local[1];
    local[0].iov_base = buff;
    local[0].iov_len = bufflen;

    struct iovec remote[1];
    remote[0].iov_base = c64memaddr;
    remote[0].iov_len = bufflen;

    // Call process_vm_readv - handle any error codes
    ssize_t nwrite = process_vm_writev(pid, local, 1, remote, 1, 0);
    if (nwrite < 0) {
        switch (errno) {
            case EINVAL:
              printf("ERROR: INVALID ARGUMENTS.\n");
              break;
            case EFAULT:
              printf("ERROR: UNABLE TO ACCESS TARGET MEMORY ADDRESS.\n");
              break;
            case ENOMEM:
              printf("ERROR: UNABLE TO ALLOCATE MEMORY.\n");
              break;
            case EPERM:
              printf("ERROR: INSUFFICIENT PRIVILEGES TO TARGET PROCESS.\n");
              break;
            case ESRCH:
              printf("ERROR: PROCESS DOES NOT EXIST.\n");
              break;
            default:
              printf("ERROR: AN UNKNOWN ERROR HAS OCCURRED.\n");
        }

        return;
    }
}


void dopeek()
{
    yylex();
    if( t.type != HEXNUM )
    {
        printf("Error: address expected\n");
        return;
    }
    unsigned long int addr = str2hex( t.str, t.len );

    void* c64memaddr = c64mem + addr;
#ifdef VERBOSE
    printf("PEEK at addr $%04x (0x%lx)\n", addr,c64memaddr );
#endif

    yylex();
    if( t.type != INTEGER )
    {
        printf("Error: num bytes expected after address\n");
        return;
    }

    int nbytes = atoi( t.str );
#ifdef VERBOSE
    printf("Want to read %d bytes\n",nbytes);
#endif

    do
    {
        yylex();
    } while( t.type != ENDCOMMAND );


    // Build iovec structs
    struct iovec local[1];
    local[0].iov_base = calloc(nbytes, sizeof(char));
    local[0].iov_len = nbytes;

    struct iovec remote[1];
    remote[0].iov_base = c64memaddr;
    remote[0].iov_len = nbytes;

    ssize_t nread = process_vm_readv(pid, local, 1, remote, 1, 0);
    if (nread < 0) {
        switch (errno) {
            case EINVAL:
              printf("ERROR: INVALID ARGUMENTS.\n");
              break;
            case EFAULT:
              printf("ERROR: UNABLE TO ACCESS TARGET MEMORY ADDRESS.\n");
              break;
            case ENOMEM:
              printf("ERROR: UNABLE TO ALLOCATE MEMORY.\n");
              break;
            case EPERM:
              printf("ERROR: INSUFFICIENT PRIVILEGES TO TARGET PROCESS.\n");
              break;
            case ESRCH:
              printf("ERROR: PROCESS DOES NOT EXIST.\n");
              break;
            default:
              printf("ERROR: AN UNKNOWN ERROR HAS OCCURRED.\n");
        }

        free( local[0].iov_base );
        return;
    }

    unsigned char* data = local[0].iov_base;
    int ii;
    for( ii=0; ii<nbytes; ++ii )
    {
        printf("%02x%c",data[ii],ii==nbytes-1?'\n':' ');
    }

    free( local[0].iov_base );
}



int main( int argc, char* argv[] )
{
    printf("\n");
    printf("*****  tc64monitor ***  v %s \n",VERSION);
    printf("***************************** \n");
    printf(" Filippo Bergamasco 2020      \n\n");

    if( argc != 5 )
    {
        printf("Usage: \n");
        printf("%s <pid> <base_address> <offset> <c64addr>\n",argv[0]);
        return 0;
    }

    pid = atoi( argv[1] );
    printf("      PID: %d\n",pid);

    void* base_address = (void*)str2hex( argv[2], strlen(argv[2]));
    printf("BASE ADDR: 0x%lx\n", (unsigned long int)base_address );

    unsigned long int offset = str2hex( argv[3], strlen(argv[3]));
    printf("   OFFSET: 0x%lx\n", (unsigned long int)offset );

    unsigned long int c64addr = str2hex( argv[4], strlen(argv[4]));
    printf("  C64ADDR: 0x%lx\n", (unsigned long int)c64addr );

    c64mem = base_address + offset - c64addr;
    printf("   C64MEM: 0x%lx\n", (unsigned long int)c64mem );


    printf("Type ? for help.\n");


    while( 1 )
    {
        printf("> ");
        fflush(0);
        yylex();

        switch( t.type ) {
            case COMMAND_PEEK:
                {
                    dopeek();
                    break;
                }
            case COMMAND_POKE:
                {
                    dopoke();
                    break;
                }
            case COMMAND_HELP:
                {
                    printf("Available commands:\n");
                    printf("  M <addr> +n                                PEEK n bytes from C64 memory starting at <addr>\n");
                    printf("  : <addr> <byte1> <byte2> ... <byten>       POKE byte1..byten to C64 memory starting at <addr>\n");
                    printf("  ?                                          Show this help\n");
                    printf("  q                                          Quit\n");
                    printf("\n");
                    printf("<addr> is a 16bit hex address (in C64 memory space) in the form 0x0000..0xFFFF or $0000..$FFFF\n");
                    printf("<byte> is an HEX number of the form 00,01,..,FF\n");
                    break;
                }
            case COMMAND_QUIT:
                {
                    printf("Bye.\n");
                    exit(0);
                }
            case ENDCOMMAND:
                {
                    break;
                }

            default: printf("Error: command expected. Type ? for help\n");
        }


    } // main loop

    return 0;
}

