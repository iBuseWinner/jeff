#!/usr/bin/env python

import asyncio, json, socket


async def send_value(k, v) -> None:
  data = json.dumps({"store_values": [{"key": k, "value": v}]}).encode()
  try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
      sock.connect(('localhost', 8005))
      sock.sendall(data)
  except ConnectionRefusedError:
    print('\tСервер отключен.')


async def read_value(k) -> str:
  data = json.dumps({"need_values": [k]}).encode()
  try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
      sock.connect(('localhost', 8005))
      sock.sendall(data)
      data = sock.recv(4096)
  except ConnectionRefusedError:
    print('\tСервер отключен.')
  return json.loads(data.decode())['values'][k]


async def main() -> None:
  #msg_id = str(uuid.uuid4())
  while True:
    option = input('Выберите вариант:\n1. Отправить значение\n2. Принять значение\n')
    #await send_time(msg_id)
    #await asyncio.sleep(5)
    if option != '1' and option != '2':
      return
    elif option == '1':
      k = input('\tВведите ключ: ')
      v = input('\tВведите значение: ')
      await send_value(k, v)
      print('\tОтправлено.')
    else:
      k = input('\tВведите ключ: ')
      v = await read_value(k)
      print(f'\tПолучено: {v}')


if __name__ == '__main__':
  try:
    asyncio.run(main())
  except KeyboardInterrupt:
    print('\nЧасы отключены.')
