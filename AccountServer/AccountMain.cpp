#include <iostream>
#include "./Infra/Network/Server.h"
#include "./Infra/Network/Encryption/TqCipher.h"
#include "./Infra/Network/Encryption/Rc5.h"
#include "./Msg/MsgAccount.h"
#include <string>
#include <thread>
#include <Windows.h>

int main()
{
	try {
		Eternal::Infra::Server AccountServer("127.0.0.1", 55099);
		AccountServer._on_receive = [](uint8_t * data, size_t bytes_received) {
			Eternal::Encryption::TqCipher cipher;
			cipher.generate_iv();
			cipher.decrypt(data, bytes_received);
			Eternal::Msg::MsgAccount msg_account{ data, bytes_received };
			Eternal::Encryption::Rc5 rc5;
			rc5.decrypt((uint8_t*)msg_account.get_password(), msg_account.get_password_len());

			std::cout << msg_account.stringfy() << '\n';

			// account server processing logic goes here.
			// check db, etc ...
		};
		//AccountServer.take_over();
		AccountServer.set_threads(3).run();

		while (!(GetAsyncKeyState(VK_NUMPAD0) & 1))
		{
			Sleep(100);
		}

		AccountServer.shutdown();
		std::cout << "Server stopped\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << '\n';
	}

	
}