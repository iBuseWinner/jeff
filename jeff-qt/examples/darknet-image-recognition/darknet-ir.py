#!/usr/bin/env python

from jeff_api import client, server
import json, subprocess, socket, os, uuid, locale

lang, _ = locale.getdefaultlocale()


def try_locate():
  """"""
  if not os.path.exists('darknet'):
    # download darknet for linux/macos/windows, but now...
    # or download tensorflow lite for linux & other platforms.
    return ""
  return "darknet"


def recognize(cli, request, savepath):
  """"""
  if request["author"] == 1:
    return
  if not request["content"].endswith('.jpg'):
    return
  path = request["content"]
  cli.send_msg(f'Got a file {path}.' if lang != 'ru_RU' else f'Получен файл {path}.')
  subprocess.call(["./darknet",
                   "detect",
                   "cfg/yolov4-tiny.cfg",
                   "yolov4-tiny.weights",
                   path.rstrip()])
  filename = f'{savepath}/{str(uuid.uuid4())}.jpg'
  os.rename('predictions.jpg', filename)
  cli.send_msg(os.path.abspath(filename))


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
  srv = server.Server(None, port)
  cli = client.Client('localhost', 8005)
  print(f'Darknet enabled on port {str(port)}.' if lang != 'ru_RU' \
        else f'Darknet слушает на порту {str(port)}.')
  while True:
    j = srv.listen()
    recognize(cli, j, savepath)


if __name__ == "__main__":
  try:
    main()
  except KeyboardInterrupt:
    print('\nDarknet disabled.' if lang != 'ru_RU' else '\nDarknet отключён.')
