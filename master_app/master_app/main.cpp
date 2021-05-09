#include <iostream>
#include <Windows.h>
#include <windowsx.h>
#include <WinUser.h>
#include <atlstr.h>
#include<fstream>

#include "eventos_mouse.h"
#include "socketio_setup.h"

using namespace std;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	eventos_mouse hook;
	socketio_main socketio;
	socketio_setup l(socketio.h);

	socketio_main socketio_posiciones;
	socketio_setup l_(socketio.h);
	
	hook.json_l();

	socketio.h.set_open_listener(std::bind(&socketio_setup::on_connected, &l));
	socketio.h.set_close_listener(std::bind(&socketio_setup::on_close, &l, std::placeholders::_1));
	socketio.h.set_fail_listener(std::bind(&socketio_setup::on_fail, &l));
	socketio.h.connect("http://127.0.0.1:3000");


	socketio_posiciones.h.set_open_listener(std::bind(&socketio_setup::on_connected, &l_));
	socketio_posiciones.h.set_close_listener(std::bind(&socketio_setup::on_close, &l_, std::placeholders::_1));
	socketio_posiciones.h.set_fail_listener(std::bind(&socketio_setup::on_fail, &l_));
	socketio_posiciones.h.connect("http://127.0.0.1:3001");

	_lock.lock();


	_lock.unlock();
	socketio.current_socket = socketio.h.socket();
	socketio_posiciones.current_socket = socketio_posiciones.h.socket();

	HHOOK keyboardHooks = SetWindowsHookEx(
		WH_MOUSE_LL,
		hook.keyboardHookProc,
		hInstance,
		0);

	HHOOK keyboardHooks_ = SetWindowsHookEx(
		WH_MOUSE_LL,
		hook.keyboardHookProc_,
		hInstance,
		0);

	current_socket_mouse = socketio.current_socket;
	current_socket_posiciones = socketio_posiciones.current_socket;
	//socketio.execute_socketio();
	MessageBox(NULL, "Presiona para salir", "APP_MASTER", (MB_ICONEXCLAMATION | MB_OK));

	string msg = "Auf Wiedersehen Bitch";
	socketio.current_socket->emit("chat_message", msg);
	socketio.h.sync_close();
	socketio.h.clear_con_listeners();

	socketio_posiciones.current_socket->emit("chat_message", msg);
	socketio_posiciones.h.sync_close();
	socketio_posiciones.h.clear_con_listeners();
	//out.close();

	
	//out.close();
	return 0;
}
