#include "helpers.h"
using namespace std;
vector<std::string> helpers::splitString(const std::string& phrase, const std::string& delimiter) {
    vector<string> list;
    string s = string(phrase);
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}


//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString()
{
    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return std::string(); //No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    //Copy the error message into a std::string.
    std::string message(messageBuffer, size);

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}


void helpers::doEOPPipe(std::string& result, int waitSeconds)
{
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;


    hPipe = CreateNamedPipe(L"\\\\.\\pipe\\M2TWEOPStartPipe",
        PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
        1,
        1024 * 16,
        1024 * 16,
        waitSeconds * 1000,
        NULL);
    // while (hPipe != INVALID_HANDLE_VALUE)
  //   {

    OVERLAPPED ol = { 0 };
    ol.hEvent = CreateEvent(
        NULL,
        TRUE,
        FALSE,
        NULL);
    if (ol.hEvent == NULL)
    {
        return;
    }
    bool fConnected = ConnectNamedPipe(
        hPipe,
        &ol);

    if (!fConnected)
    {
        DWORD err = GetLastError();
        string ress = GetLastErrorAsString();
        switch (err)
        {
        case ERROR_PIPE_CONNECTED:
            fConnected = TRUE;
            break;

        case ERROR_IO_PENDING:
            if (WaitForSingleObject(ol.hEvent, waitSeconds * 1000) == WAIT_OBJECT_0)
            {
                DWORD dwIgnore;

                fConnected = GetOverlappedResult(
                    hPipe,
                    &ol,
                    &dwIgnore,
                    FALSE);
            }
            else
            {
                CancelIo(hPipe);
            }
            break;
        }
    }

    CloseHandle(ol.hEvent);

    if (!fConnected)
    {
        CloseHandle(hPipe);
        return;
    }

    while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
    {
        /* add terminating zero */
        buffer[dwRead] = '\0';

        result = buffer;
    }
}