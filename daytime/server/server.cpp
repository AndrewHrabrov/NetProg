#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <ctime>
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
	server_address->sin_port = htons(8080); 
	server_address->sin_addr.s_addr = inet_addr("127.0.0.1");

    int rc = bind(sock, reinterpret_cast<const sockaddr*>(server_address.get()), sizeof(sockaddr_in));
    if (rc == -1) {
        cerr << "Ошибка привязки сокета" << endl;
        return 1;
    }

    cout << "Сервер daytime запущен..." << endl;
	
    while (true) {
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        char buffer[5024];
        int work_sock = recvfrom(sock, buffer, sizeof(buffer), 0, (sockaddr*)&client_addr, &client_addr_len);
        if (work_sock == -1) {
            cerr << "Ошибка получения данных" << endl;
            continue;
        }

        time_t now = time(0);
        char* dt = ctime(&now);

        if (sendto(sock, dt, strlen(dt), 0, (sockaddr*)&client_addr, sizeof(client_addr)) == -1) {
            cerr << "Ошибка отправки данных" << endl;
        }
        
        close(work_sock);
        
    }
	
	close(sock);
    return 0;
}
