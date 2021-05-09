#include "socketio_setup.h"


int participants = -1;
socket::ptr current_socket;


void bind_events()
{

	current_socket->on("new message", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck, message::list &ack_resp)
	{

		_lock.lock();
		string user = data->get_map()["username"]->get_string();
		string message = data->get_map()["message"]->get_string();
		EM(user << ":" << message);
		_lock.unlock();
	}));

	current_socket->on("user joined", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck, message::list &ack_resp)
	{
		_lock.lock();
		string user = data->get_map()["username"]->get_string();
		participants = data->get_map()["numUsers"]->get_int();
		bool plural = participants != 1;

		//     abc "
		HIGHLIGHT(user << " joined" << "\nthere" << (plural ? " are " : "'s ") << participants << (plural ? " participants" : " participant"));
		_lock.unlock();
	}));
	current_socket->on("user left", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck, message::list &ack_resp)
	{
		_lock.lock();
		string user = data->get_map()["username"]->get_string();
		participants = data->get_map()["numUsers"]->get_int();
		bool plural = participants != 1;
		HIGHLIGHT(user << " left" << "\nthere" << (plural ? " are " : "'s ") << participants << (plural ? " participants" : " participant"));
		_lock.unlock();
	}));
}


LRESULT CALLBACK socketio_main::execute_socketio(int nCode, WPARAM wParam, LPARAM lParam)
{
	//sio::client h;
	socketio_main cliente;

	if (cliente.buffer != NULL) {

		MessageBox(NULL, ":::::::::::::", "Information", MB_OK);

	}

	//socketio_setup l(cliente.h);

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}
