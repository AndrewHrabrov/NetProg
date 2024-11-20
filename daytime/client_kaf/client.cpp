#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <memory>

using namespace std;

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        cerr << "Ошибка создания сокета" << endl;
        return 1;
    }

    std::unique_ptr <sockaddr_in> server_address(new sockaddr_in);
    server_address->sin_family = AF_INET;
    server_address->sin_port = htons(13); 
    server_address->sin_addr.s_addr = inet_addr("172.16.40.1"); 

	if (sendto(sock, "", 0, 0, reinterpret_cast<const sockaddr*>(server_address.get()), sizeof(sockaddr_in)) == -1) {
        cerr << "Ошибка отправки данных" << endl;
        return 1;
    }

    char buffer[5024];
    sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int bytes_received = recvfrom(sock, buffer, sizeof(buffer), 0, (sockaddr*)&client_address, &client_address_len);
    if (bytes_received == -1) {
        cerr << "Ошибка получения данных" << endl;
        return 1;
    }

    cout << "Текущее время: " << buffer << endl;

    close(sock);
    return 0;
}
