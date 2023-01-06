#!/usr/bin/env python

import json, socket, time, uuid
from jeff_api import client

cli = client.Client('localhost', 8005)

def send_time(msg_id):
  cli.send_status(msg_id, f'Текущее время: {time.strftime("%X")}.')


def main():
  msg_id = str(uuid.uuid4())
  while True:
    send_time(msg_id)
    time.sleep(5)


if __name__ == '__main__':
  try:
    main()
  except KeyboardInterrupt:
    print('\nЧасы отключены.')
