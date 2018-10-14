# Wrapper for low level socket I/O

## Usage

Example of simple client/server usage

```c
void server()
{
	sio::socket_init();
	sio::socket_t sock = sio::socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	sio::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	sio::sock_addr4 serv_addr(INADDR_ANY, 8888);

	sio::bind(sock, serv_addr, sizeof(serv_addr));
	sio::listen(sock, 3);

	int addrlen = sizeof(serv_addr);
	sio::socket_t new_sock = sio::accept(sock, serv_addr, &addrlen);

	char buffer[1024] = { 0 };
	int nbytes = sio::recv(new_sock, buffer, 1024, 0);
	std::printf("%s\n", buffer);

	const char *hello = "Hello from server";
	sio::send(new_sock, hello, std::strlen(hello), 0);

	sio::closesocket(new_sock);
	sio::closesocket(sock);
	sio::socket_term();
}
```

```c
static void client()
{
	sio::socket_init();
	sio::socket_t sock = sio::socket(AF_INET, SOCK_STREAM, 0);

	sio::sock_addr4 serv_addr;
	serv_addr.sin_port = sio::htons(8888);

	sio::inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

	sio::connect(sock, serv_addr, sizeof(serv_addr));

	const char *hello = "Hello from client";
	sio::send(sock, hello, std::strlen(hello), 0);

	char buffer[1024] = { 0 };
	int nbytes = sio::recv(sock, buffer, 1024, 0);
	std::printf("%s\n", buffer);

	sio::closesocket(sock);
	sio::socket_term();
}
```
