#pragma once

#include <Windows.h>
#include <Shlwapi.h>

class Pipe {
	/*
	"Protocol" for read/write:
	every message starts with a signature identifying the pipe and 4 bytes containing the length of the following message
	the message follows
	*/
public:
	Pipe(LPCSTR pipe_name);
	~Pipe(void);

	/* Called by the server */
	bool listen(void);

	/* Called by the client to connect to the server. Must be called after the server has called listen() */
	bool connect(void);

	/* Checks if the pipe is connected */
	bool connected(void);

	/*
    Reads data from the pipe and writes the data to buffer, returns the number of bytes read. buffer needs to be freed later on
    On error buffer = NULL and the return value is 0
  */
	DWORD read(char **buffer);

	/*
	  Writes length bytes from buffer to the pipe, returns the number of bytes written (+ 1 byte for signature + 4 bytes for message length)
	*/
	DWORD write(char *buffer, DWORD length);
private:
	/*
  	Tries to clear the pipe by searching for the signature. Clears everything read including the signature
  	Returns true if it finds a signature, else false
	*/
	bool debug(void);

	HANDLE
		pipe;	// This value must be initialised either by calling listen() or connect() before any other operations can be used
	LPCSTR
		pipe_name,
		pipe_path;
	static const unsigned int
		pipe_buffer_size_kb = 8,
		pipe_timeout = 10;
};
