#!/usr/bin/env python

import json, socket


def send_value(k, v):
  data = json.dumps({"store_in_memory": {k: v}}).encode()
  try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
      sock.connect(('localhost', 8005))
      sock.sendall(data)
  except ConnectionRefusedError:
    print('\tСервер отключен.')


def read_value(k):
  data = json.dumps({"memory_cells": [k]}).encode()
  try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
      sock.connect(('localhost', 8005))
      sock.sendall(data)
      data = sock.recv(4096)
      while not len(data):
        data = sock.recv(4096)
  except ConnectionRefusedError:
    print('\tСервер отключен.')
  return json.loads(data.decode())['memory_values'][k]


def main():
  while True:
    option = input('Выберите вариант:\n1. Отправить значение\n2. Принять значение\n')
    if option != '1' and option != '2':
      return
    elif option == '1':
      k = input('\tВведите ключ: ')
      v = input('\tВведите значение: ')
      send_value(k, v)
      print('\tОтправлено.')
    else:
      k = input('\tВведите ключ: ')
      v = read_value(k)
      print(f'\tПолучено: {v}')


try:
  main()
except KeyboardInterrupt:
  print('\nКлиент памяти отключен.')
