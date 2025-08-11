#include <iostream>
#include <string>

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

    const char* server_ip = "127.0.0.1";
    const int port = 8080;

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        print_last_error("socket");
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
#ifdef _WIN32
    inet_pton(AF_INET, server_ip, &addr.sin_addr);
#else
    if (inet_aton(server_ip, &addr.sin_addr) == 0)
    {
        std::cerr << "Неверный IP адрес\n";
        return 1;
    }
#endif

    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
    {
        print_last_error("connect");
        CLOSESOCKET(sock);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    const std::string msg = "Привет, сервер!\n";
    int sent = send(sock, msg.c_str(), static_cast<int>(msg.size()), 0);
    if (sent <= 0)
    {
        print_last_error("send");
    }

    char buf[1024]{};
    int received = recv(sock, buf, sizeof(buf), 0);
    if (received > 0)
    {
        std::cout << "Ответ сервера: " << std::string(buf, buf + received) << std::endl;
    }
    else
    {
        std::cout << "Сервер не прислал ответ или произошла ошибка." << std::endl;
    }

    CLOSESOCKET(sock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}