#include <iostream>
#include "./Infra/Network/Server.h"
#include <string>
#include <thread>
#include <Windows.h>

int main()
{
	try {

		Eternal::Infra::Server AccountServer("127.0.0.1", 55099);
		std::thread worker_thread{ std::bind(&Eternal::Infra::Server::run, &AccountServer) };

		while (!(GetAsyncKeyState(VK_NUMPAD0) & 1))
		{
			Sleep(100);
		}

		AccountServer.get_iocontext()->stop();

		std::cout << "Server stopped\n";
		worker_thread.join();

	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << '\n';
	}

	
}