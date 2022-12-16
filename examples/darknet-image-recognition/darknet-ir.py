#!/usr/bin/env python

import json, subprocess, socket, os, uuid, locale

lang, _ = locale.getdefaultlocale()


def try_locate():
  """"""
  if not os.path.exists('darknet'):
    # download darknet for linux/macos/windows, but now...
    # or download tensorflow lite for linux & other platforms.
    return ""
  return "darknet"


def recognize(incoming, savepath):
  """"""
  r = incoming.recv(1024).strip()
  incoming.close()
  if not r:
    return
  request = json.loads(r.decode('utf-8'))
  if request["author"] == 1:
    return
  if not request["content"].endswith('.jpg'):
    return
  path = request["content"]
  response = {"send": f'Got a file {path}.' if lang != 'ru_RU' else f'Получен файл {path}.'}
  outgoing = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  outgoing.connect(('localhost', 8005))
  outgoing.send(json.dumps(response).encode())
  outgoing.close()
  subprocess.call(["./darknet",
                   "detect",
                   "cfg/yolov4-tiny.cfg",
                   "yolov4-tiny.weights",
                   path.rstrip()])
  filename = f'{savepath}/{str(uuid.uuid4())}.jpg'
  os.rename('predictions.jpg', filename)
  response = {"send": os.path.abspath(filename)}
  outgoing = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  outgoing.connect(('localhost', 8005))
  outgoing.send(json.dumps(response).encode())
  outgoing.close()


def main():
  """"""
  path = try_locate()
  if not path:
    return
  if not os.path.isdir('recognized'):
    os.mkdir('recognized')
  savepath = os.path.abspath('recognized')
  port = 15203
  os.chdir(path)
  server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  server.bind(('localhost', port))
  server.listen()
  print(f'Darknet enabled on port {str(port)}.' if lang != 'ru_RU' \
        else f'Darknet слушает на порту {str(port)}.')
  while True:
    incoming, _ = server.accept()
    recognize(incoming, savepath)


if __name__ == "__main__":
  try:
    main()
  except KeyboardInterrupt:
    print('\nDarknet disabled.' if lang != 'ru_RU' else '\nDarknet отключён.')
