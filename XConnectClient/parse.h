/*
This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once


extern void SetParsingFileName(const char *path);
extern const char* GetParsingFileName();

extern int GetParsingLineNumber();

extern token_type ReadToken(FILE *fp);

extern int TokenIs(const char *s1);
extern int TokenTypeIs(token_type ty);

extern token_type SkipBehindToken(FILE *fp, token_type ty);
extern token_type SkipToToken(FILE *fp, token_type ty);

extern const char* GetTokenBuffer();

extern void Writeln(FILE *str, int offset);
extern float stringToFloat(const char *s);
extern char* TrimRight(char *szSource );
extern char* TrimLeft(char *szSource );

extern time_t getFileCreationTime(const char *path);
extern time_t getFileModificationTime(const char *path);
extern long getFileSize(const char *path);
