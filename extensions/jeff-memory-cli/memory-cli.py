#!/usr/bin/env python

import json, socket


def send_value(k, v):
  data = json.dumps({"store_in_memory": {k: v}}).encode()
  try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
      sock.connect(('localhost', 8005))
      sock.sendall(data)
  except ConnectionRefusedError:
    print('\tServer is off.')


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
    print('\tServer is off.')
  return json.loads(data.decode())['memory_values'][k]


def main():
  while True:
    option = input('Select an option:\n1. Send value\n2. Get value\n')
    if option != '1' and option != '2':
      return
    elif option == '1':
      k = input('\tEnter the key: ')
      v = input('\tEnter the value: ')
      send_value(k, v)
      print('\tSent.')
    else:
      k = input('\tEnter the key: ')
      v = read_value(k)
      print(f'\tGot: {v}')


try:
  main()
except KeyboardInterrupt:
  print('\nMemory client is off.')
