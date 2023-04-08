#!/usr/bin/env python

import argparse, shlex, subprocess
from time import sleep
from jeff_api import client, server

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

cells = cli.read_cells(['jeff-lang', 'jeff-bundle-dir', 'jeff-os'])
lang, bundle_dir, target_os = cells['jeff-lang'], cells['jeff-bundle-dir'], cells['jeff-os']


class Process:
  def __init__(self, cmd):
    if target_os == 'Linux':
      self.args = ['konsole', '-e', 'zsh', '-c', f'{cmd}; zsh -i']
    else:
      self.args = [cmd]  # TBD
    if verbose: print(self.args)

  def run(self):
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
    process.run()
    if verbose: print('Process is started.')
    while process.poll() is None:
      sleep(0.01)
  except Exception as e:
    if str(e):
      print(str(e))
      cli.send_error(str(e))
  if verbose: print('Process finished')


def exec_simple(cmd):
  try:
    process = Process(cmd)
    process.args = shlex.split(cmd)
    process.run()
    if verbose: print('Process is started.')
    while process.poll() is None:
      sleep(0.01)
  except Exception as e:
    if str(e):
      print(str(e))
      cli.send_error(str(e))
  if verbose: print('Process finished')


def main():
  cli.send_info('[STE] Extension started.' if lang != 'ru' else '[STE] Расширение запущено.')
  while True:
    data = srv.listen()
    if len(data) == 0: continue
    if 'content_type' not in data: continue
    if data['content_type'] not in (1, 2): continue
    if 'author' not in data: continue
    if data['author'] != 0: continue
    if 'content' not in data: continue
    cc = data['content']
    if verbose: print(f'Got: $ {cc}')
    if '/simple ' in cc:
      exec_simple(cc.replace('/simple ', ''))
    else:
      exec_cmd(cc)


try:
  main()
except KeyboardInterrupt:
  print('\nCommand Line\' Extension is off.' if lang != 'ru' else '\nРасширение Командная строка выключено.')
