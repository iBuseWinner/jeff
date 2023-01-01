#!/usr/bin/env python

import json, socket, time, uuid


def send_time(msg_id) -> None:
  data = json.dumps({"send_status": {"id": msg_id,
                                     "msg": f'Текущее время: {time.strftime("%X")}.'}}).encode()
  try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
      sock.connect(('localhost', 8005))
      sock.sendall(data)
  except ConnectionRefusedError:
    print('Сервер отключен.')


def main() -> None:
  msg_id = str(uuid.uuid4())
  while True:
    send_time(msg_id)
    time.sleep(5)


if __name__ == '__main__':
  try:
    main()
  except KeyboardInterrupt:
    print('\nЧасы отключены.')
