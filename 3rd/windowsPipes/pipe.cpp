#include <Windows.h>
#include <Shlwapi.h>

#ifdef DEBUG
#include <stdio.h>	// DEBUG
#endif

#include "Pipe.h"

Pipe::Pipe(LPCSTR pipe_name)
{
	/*
		Initialises variables
	*/
	this->pipe_name = (LPCSTR)new char[strlen(pipe_name) + 1];	// Remember the pipe file path for later use
	this->pipe_path = (LPCSTR)new char[strlen(pipe_name) + 10];	// The path passed to CreateNamedPipe / CreateFile / etc.

	strcpy((char *)this->pipe_name, pipe_name);
	strcpy((char *)this->pipe_path, "\\\\.\\pipe\\");
	strcpy((char *)(this->pipe_path + 9), pipe_name);

	this->pipe = INVALID_HANDLE_VALUE;

	return;
}

Pipe::~Pipe()
{
	/*
		Frees all previously allocated memory and closes the pipe handle
	*/
	delete[] this->pipe_name;
	delete[] this->pipe_path;

	if (!this->connected())	// The pipe needs to be connected in order to read from it
		CloseHandle(this->pipe);

	return;
}

bool Pipe::debug(void)
{
	/*
		Reads incoming pipe traffic until reaching the 'signature byte'.
		Private function
	*/
	if (!this->connected())	// Check that the pipe is connected
	{
#ifdef DEBUG
		printf("Tried to debug non-connected pipe '%s'\n", this->pipe_name);
#endif
		return NULL;
	}

	DWORD bytes_received;
	char byte;

	for (;;)
	{
		ReadFile(this->pipe, (char *)&byte, sizeof(byte), &bytes_received, NULL);
		if (byte == *this->pipe_name)
			return true;
	}
	return false;	// will never reach here .. program will be stuck until it reads a signature
}


bool Pipe::listen(void)
{
	/*
		Listens on the pipe for an incoming connection.
	*/
	this->pipe = CreateNamedPipe(	// The stuff specified here must be set on any handle used to connect. Else an error will occur according to the msdn documentation
		this->pipe_path,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		1,
		this->pipe_buffer_size_kb * 1024,
		this->pipe_buffer_size_kb * 1024,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);

	if (this->pipe == INVALID_HANDLE_VALUE)
	{
#ifdef DEBUG
		printf("Failed to create named pipe '%s': %d\n", this->pipe_name, GetLastError());
#endif
		return false;
	}

#ifdef DEBUG
	printf("[D] Listening on pipe %s\n", this->pipe_path);
#endif
	if (!ConnectNamedPipe(this->pipe, NULL))
	{
#ifdef DEBUG
		printf("Failed to listen on pipe '%s': %d\n", this->pipe_name, GetLastError());
#endif
		return false;
	}

#ifdef DEBUG
	printf("[D] A process connected on '%s'\n", this->pipe_name);
#endif

	return true;
}


bool Pipe::connect(void)
{
	/*
		Connects the pipe to the pipe-server.
		The server must be waiting for an incoming connection by having
		called Pipe::listen() previously.
	*/
#ifdef DEBUG
	printf("[D] Connecting to pipe '%s'\n", this->pipe_path);
#endif
	DWORD pipe_mode;

	for (;;)
	{
		this->pipe = CreateFile(
			this->pipe_path,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		if (this->pipe != INVALID_HANDLE_VALUE)
			break;
		else if (GetLastError() != ERROR_PIPE_BUSY)
		{
#ifdef DEBUG
			printf("Failed to connect to pipe '%s': %d\n", this->pipe_name, GetLastError());
#endif
			return false;
		}
		else {	// this->pipe == INVALID_HANDLE_VALUE && GetLastError == ERROR_PIPE_BUSY
			if (!WaitNamedPipe(this->pipe_path, this->pipe_timeout * 1000))
			{
#ifdef DEBUG
				printf("Pipe '%s' is busy and connection request timed out after %d seconds\n", this->pipe_name, this->pipe_timeout);
#endif
				return false;
			}
		}
	}

	pipe_mode = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_NOWAIT;	// Set the pipe handle state according to the stuff defined in listen()
	if (!SetNamedPipeHandleState(this->pipe, &pipe_mode, NULL, NULL))
	{
#ifdef DEBUG
		printf("Failed to set pipe '%s' handle state: %d\n", this->pipe_name, GetLastError());
#endif
		CloseHandle(this->pipe);
		return false;
	}

#ifdef DEBUG
	printf("[D] '%s' Connected", this->pipe_name);
#endif

	return true;
}


bool Pipe::connected(void)
{
	/*
		Checks if the pipe is connected
	*/
	return (this->pipe != NULL && this->pipe != INVALID_HANDLE_VALUE);
}


DWORD Pipe::read(char **return_buffer)
{
	/*
		Reads a message from the pipe
	*/
	DWORD	message_length,
		bytes_received, total_bytes_received;
	char	signature_byte, *buffer;

	if (!this->connected())	// Check that the pipe is connected
	{
#ifdef DEBUG
		printf("Tried to read from non-connected pipe '%s'\n", this->pipe_name);
#endif
		return NULL;
	}

	if (!ReadFile(this->pipe, (LPVOID)&signature_byte, sizeof(char), &bytes_received, NULL))
	{
#ifdef DEBUG
		printf("Could not read message signature from pipe '%s': %d\n", this->pipe_name, GetLastError());
#endif
		return NULL;
	}

	if (signature_byte != *this->pipe_name)
	{
#ifdef DEBUG
		printf("Pipe '%s' is bugged.. debugging\n", this->pipe_name);
#endif
		if (!this->debug())
		{
#ifdef DEBUG
			printf("Failed to debug '%s'\n", this->pipe_name);
#endif
			return NULL;
		}
	}

	if (!ReadFile(this->pipe, (LPVOID)&message_length, sizeof(DWORD), &bytes_received, NULL))
	{
#ifdef DEBUG
		printf("Could not read message length from pipe '%s': %d\n", this->pipe_name, GetLastError());
#endif
		return NULL;
	}

	buffer = new char[message_length + 1];
	total_bytes_received = 0;
	while (total_bytes_received < message_length)
	{
		if (!ReadFile(this->pipe, buffer + total_bytes_received, message_length - total_bytes_received, &bytes_received, NULL))
		{
			free(buffer);
#ifdef DEBUG
			printf("Failed to read message of length %d from pipe '%s': %d\n", message_length, this->pipe_name, GetLastError());
#endif
			return NULL;
		}

		total_bytes_received += bytes_received;
	}
	buffer[message_length] = '\0';

	*return_buffer = buffer;

	return total_bytes_received;
}

DWORD Pipe::write(char *buffer, DWORD length)
{
	/*
		Writes a message to the pipe
	*/
	char *message_buffer, *ptr;
	DWORD i, message_length, bytes_written;

	if (!this->connected())	// Check that the pipe is connected
	{
#ifdef DEBUG
		printf("Tried to write to non-connected pipe '%s'\n", this->pipe_name);
#endif
		return NULL;
	}

	message_length = length + sizeof(DWORD) + 1;	// Construct the message "signature + message_length + message"
	message_buffer = new char[message_length];

	*message_buffer = *this->pipe_name;				// Write signature

	ptr = (char *)&length;							// Write message length in binary
	for (i = 0; i < sizeof(DWORD); i++)
		message_buffer[i + 1] = ptr[i];

	for (i = 0; i < length; i++)					// Write message
		message_buffer[i + sizeof(DWORD) + 1] = buffer[i];

	if (!WriteFile(this->pipe, message_buffer, message_length, &bytes_written, NULL))
	{
#ifdef DEBUG
		printf("Failed to write to pipe '%s': %d\n", this->pipe_name, GetLastError());
#endif
		return NULL;
	}

	delete[] message_buffer;

	return bytes_written;
}
