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


#define XC_CALL_CONNECTION			"FSASMLIB:IPC"
#define XC_ACTION_READ				(1)
#define XC_ACTION_WRITE				(2)
#define XC_ACTION_READTRUE			(3)
#define XC_ACTION_WRITETRUE			(4)
#define XC_ACTION_READTOKEN			(5)
#define XC_ACTION_WRITETOKEN		(6)
#define XC_ACTION_SEARCH			(7)
#define XC_RETURN_FAILURE			(0)
#define XC_RETURN_SUCCESS			(1)

typedef struct tagXC_ACTION_READ_HDR {
	DWORD action;
	DWORD offset;
	DWORD size;
	DWORD data;
} XC_ACTION_READ_HDR;

typedef struct tagXC_ACTION_WRITE_HDR {
	DWORD action;
	DWORD offset;
	DWORD size;
} XC_ACTION_WRITE_HDR;

typedef struct tagXC_ACTION_READTOKEN_HDR {
	DWORD action;
	DWORD token;
	DWORD data;
} XC_ACTION_READTOKEN_HDR;

typedef struct tagXC_ACTION_WRITETOKEN_HDR {
	DWORD action;
	DWORD token;
} XC_ACTION_WRITETOKEN_HDR;

typedef struct tagXC_ACTION_SEARCH_HDR {
	DWORD action;
	DWORD offset_from;
	DWORD offset_to;
	DWORD size;
} XC_ACTION_SEARCH_HDR;

enum OperationType {
	WaitingT,
	WritingT,
	ReadingT,
	SearchingT
};

extern void module_init(HINSTANCE dllInstance);
extern void module_deinit();