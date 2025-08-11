#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <atomic>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "ws2_32.lib")
  using socklen_t = int;
  #define CLOSESOCKET closesocket
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #define INVALID_SOCKET (-1)
  #define SOCKET_ERROR (-1)
  using SOCKET = int;
  #define CLOSESOCKET ::close
#endif

static void print_last_error(const char* where)
{
#ifdef _WIN32
    std::cerr << where << " WSA error: " << WSAGetLastError() << std::endl;
#else
    perror(where);
#endif
}

void handle_client(SOCKET client)
{
    char buf[1024]{};
    int received = recv(client, buf, sizeof(buf), 0);
    if (received > 0)
    {
        std::cout << "Клиент отправил: " << std::string(buf, buf + received) << std::endl;
        const std::string reply = "Привет от сервера!\n";
        send(client, reply.c_str(), static_cast<int>(reply.size()), 0);
    }
    else
    {
        std::cout << "Клиент отключился или ошибка приема." << std::endl;
    }

    CLOSESOCKET(client);
}

int main()
{
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    const int port = 8080;

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET)
    {
        print_last_error("socket");
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // Позволяет быстро перезапускать сервер
    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(server, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
    {
        print_last_error("bind");
        CLOSESOCKET(server);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    if (listen(server, 10) == SOCKET_ERROR)
    {
        print_last_error("listen");
        CLOSESOCKET(server);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    std::cout << "Сервер запущен на порту " << port << std::endl;

    while (true)
    {
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);
        SOCKET client = accept(server, reinterpret_cast<sockaddr*>(&client_addr), &len);
        if (client == INVALID_SOCKET)
        {
            print_last_error("accept");
            continue;
        }

        std::thread t(handle_client, client);
        t.detach();
    }

    CLOSESOCKET(server);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}