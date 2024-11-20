#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <memory>

using namespace std;

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Ошибка создания сокета" << endl;
        return 1;
    }

    std::unique_ptr <sockaddr_in> server_address(new sockaddr_in);
    server_address->sin_family = AF_INET;
    server_address->sin_port = htons(7777); 
    server_address->sin_addr.s_addr = inet_addr("127.0.0.1");

    int rc = bind(sock, reinterpret_cast<const sockaddr*>(server_address.get()), sizeof(sockaddr_in));
	if (rc == -1) {
        cerr << "Ошибка привязки адреса" << endl;
        return 1;
    }
    
    if (listen(sock, 5) == -1) {
        cerr << "Ошибка прослушивания" << endl;
        return 1;
    }

    cout << "Сервер echo запущен..." << endl;

    while (true) {
        sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_sock = accept(sock, (sockaddr*)&client_address, &client_address_len);
        if (client_sock == -1) {
            cerr << "Ошибка принятия соединения" << endl;
            continue;
        }

        char buffer[5024];
        int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            cerr << "Ошибка получения данных" << endl;
            close(client_sock);
            continue;
        }

        if (send(client_sock, buffer, bytes_received, 0) == -1) {
            cerr << "Ошибка отправки данных" << endl;
        }

        close(client_sock);
    }

    close(sock);
    return 0;
}
