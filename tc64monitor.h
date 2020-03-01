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

#ifndef _TC64MONITOR_H_
#define  _TC64MONITOR_H_


typedef enum {
    COMMAND_PEEK,
    COMMAND_POKE,
    COMMAND_HELP,
    COMMAND_QUIT,
    HEXNUM,
    HEXBYTE,
    INTEGER,
    ENDCOMMAND,
    BLANK,
    UNKNOWN
} token_type;

typedef struct token {
    token_type type;
    char* str;
    int len;
} token;

extern token t;
extern int yylex();

#endif
