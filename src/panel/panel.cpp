#include "panel.h"

void Panel()
{
    const string ipAddress = "127.0.0.1";			// IP Address of the server
    const int port = 13254;						// Listening port # on the server

    // Initialize WinSock
    WSAData data{};
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        cerr << "Can't start Winsock, Err #" << wsResult << endl;
        return;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
        WSACleanup();
        return;
    }

    // Fill in a hint structure
    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    // Do-while loop to send and receive data
    char buf[4096];
    string userInput;

    do
    {
        // Prompt the user for some text
        cout << "Command> ";
        getline(cin, userInput);

        if (userInput.size() > 0)		// Make sure the user has typed in something
        {
            // Send the text
            send(sock, userInput.c_str(), userInput.size() + 1, 0);
        }

    } while (userInput.size() > 0);

    // Gracefully close down everything
    closesocket(sock);
    WSACleanup();
}