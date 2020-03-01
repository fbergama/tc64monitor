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
digit    [0-9]
alpha    [a-fA-F]
hextail  ({digit}|{alpha}){1,8}
hexaddr1 (0[xX]){hextail}
hexaddr2 ($){hextail}
hexbyte  ({digit}|{alpha}){2}
decimaln (\+{digit}*)

%top{
#include "tc64monitor.h"

token t;
}


%%

^M          {t.type = COMMAND_PEEK; t.str=yytext; t.len=yyleng; return 1;}
^:          {t.type = COMMAND_POKE; t.str=yytext; t.len=yyleng; return 1;}
^\?         {t.type = COMMAND_HELP; t.str=yytext; t.len=yyleng; return 1;}
^q          {t.type = COMMAND_QUIT; t.str=yytext; t.len=yyleng; return 1;}
{hexaddr1}  {t.type = HEXNUM; t.str=yytext+2; t.len=yyleng-2; return 1;}
{hexaddr2}  {t.type = HEXNUM; t.str=yytext+1; t.len=yyleng-1; return 1;}
{hexbyte}   {t.type = HEXBYTE; t.str=yytext; t.len=yyleng; return 1;}
{decimaln}  {t.type = INTEGER; t.str=yytext; t.len=yyleng; return 1;}
[\n\r]      {t.type = ENDCOMMAND; return 1;}
[" "\r\t]   {}
.           {t.type = UNKNOWN; t.str=yytext; t.len=yyleng; return 1; }
%%

