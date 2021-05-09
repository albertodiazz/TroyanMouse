import socketio
import  asyncio
import win32api, win32con
import json

sio = socketio.AsyncClient()
data_read = open('setup_ip_puertos.json')
data = json.load(data_read)

@sio.event
async def connect():
    print("Conenxion establecida")

@sio.on('botones_output')
async def on_message(data):
    print(data)
    if data['boton_derecho'] == 'UP':
        win32api.mouse_event(win32con.MOUSEEVENTF_LEFTDOWN, win32con.MOUSEEVENTF_ABSOLUTE,win32con.MOUSEEVENTF_ABSOLUTE,0,0)
        print('<<<>>>')     
    if data['boton_derecho'] == 'DOWN' and  data['boton_izquierdo'] == 'UP':
        win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP, win32con.MOUSEEVENTF_ABSOLUTE,win32con.MOUSEEVENTF_ABSOLUTE,0,0)  
    if data['boton_izquierdo'] == 'DOWN':
        win32api.mouse_event(win32con.MOUSEEVENTF_RIGHTUP, win32con.MOUSEEVENTF_ABSOLUTE,win32con.MOUSEEVENTF_ABSOLUTE,0,0)
    #if data['boton_izquierdo'] == 'DOWN' and  data['boton_derecho'] == 'UP':
    #    win32api.mouse_event(win32con.MOUSEEVENTF_RIGHTUP, win32con.MOUSEEVENTF_ABSOLUTE,win32con.MOUSEEVENTF_ABSOLUTE,0,0)
    #else:
       # win32api.mouse_event(win32con.MOUSEEVENTF_RIGHTDOWN, win32con.MOUSEEVENTF_ABSOLUTE,win32con.MOUSEEVENTF_ABSOLUTE,0,0)

@sio.on('wheel_output')
async def on_message(data):
    print(data)
    if int(data['wheel_Scroll']) > 0:
        win32api.mouse_event(win32con.MOUSEEVENTF_WHEEL,win32con.MOUSEEVENTF_ABSOLUTE,win32con.MOUSEEVENTF_ABSOLUTE,(int(data['wheel_Scroll'])*120),0)
    else:
        win32api.mouse_event(win32con.MOUSEEVENTF_WHEEL,win32con.MOUSEEVENTF_ABSOLUTE,win32con.MOUSEEVENTF_ABSOLUTE,(abs(int(data['wheel_Scroll']))*-120),0)
    
async def start_server():
    print()
    await sio.connect(data['botones'][0]['IP'] + ':' + data['botones'][0]['PORT'])
    await sio.wait()

if __name__ == '__main__':
    asyncio.run(start_server())    
    data_read.close()