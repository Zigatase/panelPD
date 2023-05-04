#include "panel.h"

string panelId {"PanelId"};

//
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

    cout << "\n--- Connect To Server ---\n\n";

    send(sock, panelId.c_str(), panelId.size() + 1, 0);

    // Do-while loop to send and receive data
    char buf[4096];

    //
    string userInput;

    while (true)
    {
        ZeroMemory(buf, 4096);

        int bytesReceived = recv(sock, buf, 4096, 0);
        string command = string(buf, 0 , bytesReceived);

        cout << command << endl;

        // Prompt the user for some text
        cout << "Command -> ";
        getline(cin, userInput);

        if (userInput.size() > 0)		// Make sure the user has typed in something
        {
            registerCommands(userInput, sock);
        }

        // Exit Command To Panel
        if (userInput == "-Exit")
        {
            break;
        }
    }

    // Gracefully close down everything
    closesocket(sock);
    WSACleanup();

    cout << "Exit";
}

//

void registerCommands(string userInput, SOCKET sock)
{
    if(userInput == "-Help")
    {
        printf("------------------------------------------------------|\n");
        printf("Example Commands: id + command                        |\n");
        printf("------------------------------------------------------|\n");
        printf("Connection Commands         |   -CCommand             |\n");
        printf("User Interface Commands     |   -UICommands           |\n");
        printf("System Commands             |   -SCommand             |\n");
        printf("File Commands               |   -FCommand             |\n");
        printf("------------------------------------------------------|\n");
    }
    else if(userInput == "-CCommand")
    {
        printf("------------------------------------------------------|\n");
        printf("Example Commands: id + command                        |\n");
        printf("------------------------------------------------------|\n");
        printf("(Connection Commands)                                 |\n");
        printf("                                                      |\n");
        printf("-clients        |  View Connected Clients             |\n");
        printf("[connect <id>]  |   Connect to Client                 |\n");
        printf("[close <id>]    |   Terminate Connection              |\n");
        printf("[delete <id>]   |   Kill Connection & Delete Program  |\n");
        printf("[closeall]      |   Terminates All Connections        |\n");
        printf("------------------------------------------------------|\n");
    }
    else if(userInput == "-UICommands")
    {
        printf("------------------------------------------------------|\n");
        printf("Example Commands: id + command                        |\n");
        printf("------------------------------------------------------|\n");
        printf("(User Interface Command)                              |\n");
        printf("                                                      |\n");
        printf("[clients]       |  View Connected Clients             |\n");
        printf("[connect <id>]  |   Connect to Client                 |\n");
        printf("[close <id>]    |   Terminate Connection              |\n");
        printf("[delete <id>]   |   Kill Connection & Delete Program  |\n");
        printf("[closeall]      |   Terminates All Connections        |\n");
        printf("------------------------------------------------------|\n");
    }
    else if(userInput == "-SCommand")
    {
        printf("------------------------------------------------------|\n");
        printf("Example Commands: id + command                        |\n");
        printf("------------------------------------------------------|\n");
        printf("(System Commands)                                     |\n");
        printf("                                                      |\n");
        printf("[clients]       |  View Connected Clients             |\n");
        printf("[connect <id>]  |   Connect to Client                 |\n");
        printf("[close <id>]    |   Terminate Connection              |\n");
        printf("[delete <id>]   |   Kill Connection & Delete Program  |\n");
        printf("[closeall]      |   Terminates All Connections        |\n");
        printf("------------------------------------------------------|\n");
    }
    else if(userInput == "-FCommand")
    {
        printf("------------------------------------------------------|\n");
        printf("Example Commands: id + command                        |\n");
        printf("------------------------------------------------------|\n");
        printf("(File Commands)                                       |\n");
        printf("                                                      |\n");
        printf("[clients]       |  View Connected Clients             |\n");
        printf("[connect <id>]  |   Connect to Client                 |\n");
        printf("[close <id>]    |   Terminate Connection              |\n");
        printf("[delete <id>]   |   Kill Connection & Delete Program  |\n");
        printf("[closeall]      |   Terminates All Connections        |\n");
        printf("------------------------------------------------------|\n");
    }
    else
    {
        // Send the command
        send(sock, userInput.c_str(), userInput.size() + 1, 0);
    }
}