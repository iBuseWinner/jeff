#!/usr/bin/env python

import argparse, shlex, subprocess
from time import sleep
from jeff_api import client, server, scenario

parser = argparse.ArgumentParser(description="Simple Terminal Extension.")
parser.add_argument("extension_port", type=int, help="extension's server port")
parser.add_argument("jeff_port", type=int, help="Jeff port")
parser.add_argument("-v", "--verbose", action="store_true", help="verbose output")

args = parser.parse_args()
extension_port = args.extension_port
jeff_port = args.jeff_port
verbose = args.verbose

srv = server.Server(None, extension_port)
cli = client.Client('localhost', jeff_port)

cells = cli.read_cells(['jeff-lang', 'jeff-bundle-dir'])
lang, bundle_dir = cells['jeff-lang'], cells['jeff-bundle-dir']


class Process:
  def __init__(self, cmd):
    self.args = ['konsole', '-e', 'zsh', '-c', f'{cmd}; zsh -i']
    if verbose: print(self.args)
    self.process = subprocess.Popen(
      self.args,
      stdin=subprocess.PIPE,
      stdout=subprocess.PIPE,
      start_new_session=True,
      cwd=bundle_dir
    )

  def poll(self):
    return self.process.poll()


def exec_cmd(cmd):
  try:
    process = Process(cmd)
    if verbose: print('Process is started.')
    while process.poll() is None:
      sleep(0.01)
  except Exception as e:
    if str(e):
      print(str(e))
      cli.send_error(err_msg + str(e))
  if verbose: print('Process finished')


def main():
  cli.send_info('[STE] Extension started.' if lang != 'ru' else '[STE] Расширение запущено.')
  while True:
    data = srv.listen()
    if len(data) == 0: continue
    if not 'content_type' in data: continue
    if data['content_type'] not in (1, 2): continue
    if 'author' not in data: continue
    if data['author'] != 0: continue
    if not 'content' in data: continue
    cc = data['content']
    if verbose: print(f'Got: $ {cc}')
    exec_cmd(cc)


try:
  main()
except KeyboardInterrupt:
  print('\nCommand Line\' Extension is off.' if lang != 'ru' else '\nРасширение Командная строка выключено.')
