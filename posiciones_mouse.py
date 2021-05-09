import socketio
import  asyncio
import win32api
import json

sio = socketio.AsyncClient()
data_read = open('setup_ip_puertos.json')
data = json.load(data_read)

@sio.event
async def connect():
    print("Conenxion establecida")

@sio.on('posiciones_output')
async def on_message(data):
    x = int(data["pX"])
    y = int(data["pY"])
    win32api.SetCursorPos((x,y))

async def start_server():
    await sio.connect(data['posicion'][0]['IP'] + ':' + data['posicion'][0]['PORT'])
    await sio.wait()

if __name__ == '__main__':
    asyncio.run(start_server())    
    data_read.close()