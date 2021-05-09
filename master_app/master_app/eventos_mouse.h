#pragma once
#include <iostream>
#include <Windows.h>
#include <windowsx.h>
#include <WinUser.h>
#include <atlstr.h>

#define _WIN32_WINNT 0x0500
#include "socketio_setup.h";

#include "nlohmann/json.hpp"
#include <fstream>;

using namespace std;
static socket::ptr current_socket_mouse;
static socket::ptr current_socket_posiciones;
static int test_mensajes = 0;
using namespace nlohmann;


static ifstream ifs("src/data_out.json");
static json msg;

class eventos_mouse
{
private:

public:

	struct info
	{
		int pX;
		int pY;
		int datos_wheel;
		string boton_presionado;
	};

	static sio::message::ptr createObject(json o)
	{
		sio::message::ptr object = object_message::create();

		for (json::iterator it = o.begin(); it != o.end(); ++it)
		{
			auto key = it.key();
			auto v = it.value();

			if (v.is_boolean())
			{
				object->get_map()[key] = bool_message::create(v.get<bool>());
			}
			else if (v.is_number_integer())
			{
				object->get_map()[key] = int_message::create(v.get<int>());
			}
			else if (v.is_string())
			{
				object->get_map()[key] = string_message::create(v.get<std::string>());
			}
			else if (v.is_array())
			{
				json childObject = v;
				object->get_map()[key] = createArray(v);
			}
			else if (v.is_object())
			{
				json childObject = v;
				object->get_map()[key] = createObject(childObject);
			}
		}
		return object;
	}
	static sio::message::ptr createArray(json o)
	{
		sio::message::ptr array = array_message::create();

		for (json::iterator it = o.begin(); it != o.end(); ++it) {

			auto v = it.value();
			if (v.is_boolean())
			{
				array->get_vector().push_back(bool_message::create(v.get<bool>()));
			}
			else if (v.is_number_integer())
			{
				array->get_vector().push_back(int_message::create(v.get<int>()));
			}
			else if (v.is_string())
			{
				array->get_vector().push_back(string_message::create(v.get<string>()));
			}
			else if (v.is_array())
			{
				array->get_vector().push_back(createArray(v));
			}
			else if (v.is_object())
			{
				array->get_vector().push_back(createObject(v));
			}
		}
		return array;
	}
	void json_l() {
		try
		{
			json estructura = {
			{"pX", 0},
			{"pY" , 0},
			{"wheel_Scroll" , 0},
			{"boton_derecho" , "UP"},
			{"boton_izquierdo" , "UP"}
			};
			msg = estructura;
		}
		catch (const std::exception&)
		{

		}
	
	}


	static LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {

		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);

		MSLLHOOKSTRUCT *pMhs = (MSLLHOOKSTRUCT *)lParam;
		MSLLHOOKSTRUCT* mWheelDStruct = (MSLLHOOKSTRUCT*)lParam;
		POINT pt = pMhs->pt;
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		int delta = pMhs->mouseData;
		int deltas = GET_WHEEL_DELTA_WPARAM(wParam);
		short zDelta = HIWORD(pMhs->mouseData);


		sio::message::ptr object = createObject(msg);

		/////Posicion de la rueda del raton
		if (wParam == WM_MOUSEWHEEL) {
			if (zDelta > 0)
			{
				//click by scroll
				if (test_mensajes * 120 >= 120) {
					test_mensajes = 1;
				}
				else {
					test_mensajes++;
				}
				msg["wheel_Scroll"] = test_mensajes;
				current_socket_mouse->emit("wheel_input", object);
			}
			else
			{
				if (test_mensajes * 120 >= 120) {
					test_mensajes = 1;
				}
				else {
					test_mensajes++;
				}
				msg["wheel_Scroll"] = test_mensajes * -1;
				current_socket_mouse->emit("wheel_input", object);
			}

		}
		////////////Posicion de mouse
		/*
		POINT mouse_position;
		if (wParam == WM_MOUSEMOVE) {
			if (GetCursorPos(&mouse_position))
			{
				msg["pX"] = mouse_position.x;
				msg["pY"] = mouse_position.y;
				//current_socket_mouse->emit("posiciones_input", object);
				//current_socket_mouse->off_all();
			}
		}*/

		string clicks_;
		switch (wParam)
		{
		case WM_LBUTTONUP:
			msg["boton_derecho"] = "UP";
			current_socket_mouse->emit("envio_datos", object);
			break;
		case WM_LBUTTONDOWN:
			msg["boton_derecho"] = "DOWN";
			current_socket_mouse->emit("envio_datos", object);
			break;
		case WM_RBUTTONDOWN:
			msg["boton_izquierdo"] = "DOWN";
			current_socket_mouse->emit("envio_datos", object);
			break;
		case WM_RBUTTONUP:
			msg["boton_izquierdo"] = "UP";
			current_socket_mouse->emit("envio_datos", object);
			break;
		}


		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	static LRESULT CALLBACK keyboardHookProc_(int nCode, WPARAM wParam, LPARAM lParam) {

		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);

		MSLLHOOKSTRUCT *pMhs = (MSLLHOOKSTRUCT *)lParam;
		MSLLHOOKSTRUCT* mWheelDStruct = (MSLLHOOKSTRUCT*)lParam;
		POINT pt = pMhs->pt;
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		int delta = pMhs->mouseData;
		int deltas = GET_WHEEL_DELTA_WPARAM(wParam);
		short zDelta = HIWORD(pMhs->mouseData);


		sio::message::ptr object = createObject(msg);

		////////////Posicion de mouse
		POINT mouse_position;
		if (wParam == WM_MOUSEMOVE) {
			if (GetCursorPos(&mouse_position))
			{
				msg["pX"] = mouse_position.x;
				msg["pY"] = mouse_position.y;
				current_socket_posiciones->emit("posiciones_input", object);

				//current_socket_mouse->off_all();
			}
		}

		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	eventos_mouse();

};

eventos_mouse::eventos_mouse()
{

}



