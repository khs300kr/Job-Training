#include <boost/asio.hpp>
#include <iostream>

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;


const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;


void main()
{
	io_service io_s;
	tcp::endpoint endpoint(ip::tcp::v4(),PORT_NUMBER);

	tcp::acceptor acceptor(io_s, endpoint);
	tcp:: socket socket(io_s);

	acceptor.accept(socket);
	cout << "클라이언트접속" << endl;

	for (;;)
	{
		array<char, 128> buf;
		buf.assign(0);
		boost::system::error_code error;
		size_t len = socket.read_some(buffer(buf), error);

		if (error)
		{
			if (error == error::eof)
			{
				cout << "클라이언트 연결이 끊어졌습니다" << endl;
			}
			else
			{
				cout << "error No : " << error.value() << "error Message : " << error.message() << endl;
			}
			break;

		}
		cout << "클라이언트에서 받은 메시지 : " << &buf[0] << endl;
		char szMessage[128]{};
		sprintf_s(szMessage, 128 - 1, "Re:%s", &buf[0]);
		int nMsgLen = strnlen(szMessage, 128 - 1);

		boost::system::error_code ignored_error;
		socket.write_some(buffer(szMessage, nMsgLen), ignored_error);
		cout << "클라이언트에 보낸 메시지 : " << szMessage << endl;
	}
	getchar();
		
}