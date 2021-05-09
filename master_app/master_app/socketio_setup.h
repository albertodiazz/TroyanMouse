#pragma once
#include "sio_client.h"
#include <Windows.h>
#include <windowsx.h>
#include <WinUser.h>
#include <atlstr.h>

#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#ifdef WIN32
#define HIGHLIGHT(__O__) std::cout<<__O__<<std::endl
#define EM(__O__) std::cout<<__O__<<std::endl

#include <stdio.h>
#include <tchar.h>
#define MAIN_FUNC int _tmain(int argc, _TCHAR* argv[])
#else
#define HIGHLIGHT(__O__) std::cout<<"\e[1;31m"<<__O__<<"\e[0m"<<std::endl
#define EM(__O__) std::cout<<"\e[1;30;1m"<<__O__<<"\e[0m"<<std::endl

#define MAIN_FUNC int main(int argc ,const char* args[])
#endif

using namespace sio;
using namespace std;


static std::mutex _lock;
static std::condition_variable_any _cond;
static bool connect_finish = false;

class socketio_setup
{
private:


public:

	sio::client &handler;

	socketio_setup(sio::client& h) :
		handler(h)
	{
	}

	void on_connected()
	{
		_lock.lock();
		_cond.notify_all();
		connect_finish = true;
		_lock.unlock();
	}
	void on_close(client::close_reason const& reason)
	{
		std::cout << "sio closed " << std::endl;
		exit(0);
	}

	void on_fail()
	{
		std::cout << "sio failed " << std::endl;
		exit(0);
	}

};

class socketio_main
{
public:
	static LRESULT CALLBACK execute_socketio(int nCode, WPARAM wParam, LPARAM lParam);
	sio::client h;
	socket::ptr current_socket;

	int * buffer;
};



