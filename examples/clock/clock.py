#!/usr/bin/env python

import asyncio, json, socket, time, uuid


async def send_time(msg_id) -> None:
  data = json.dumps({"send_status": {"id": msg_id, "msg": time.strftime('%X')}}).encode()
  try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
      sock.connect(('localhost', 8005))
      sock.sendall(data)
  except ConnectionRefusedError:
    print('Сервер отключен.')


async def main() -> None:
  msg_id = str(uuid.uuid4())
  while True:
    await send_time(msg_id)
    await asyncio.sleep(5)


if __name__ == '__main__':
  try:
    asyncio.run(main())
  except KeyboardInterrupt:
    print('\nЧасы отключены.')
