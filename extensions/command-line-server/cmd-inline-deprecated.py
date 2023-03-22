#!/usr/bin/env python

import argparse, shlex, subprocess
from time import sleep
from jeff_api import client, server, scenario

parser = argparse.ArgumentParser(description="")
parser.add_argument("extension_port", type=int, help="extension's server port")
parser.add_argument("jeff_port", type=int, help="Jeff port")
parser.add_argument("-v", "--verbose", action="store_true", help="verbose output")

args = parser.parse_args()
extension_port = args.extension_port
jeff_port = args.jeff_port
verbose = args.verbose

srv = server.Server(None, extension_port)
cli = client.Client('localhost', jeff_port)
lang, bundle_dir = (
  lambda **k: (k[list(k.keys())[1]], k[list(k.keys())[0]])
)(
  **(cli.read_cells(['jeff-lang', 'jeff-bundle-dir']))
)

scn_name = 'Command Line' if lang != 'ru' else 'Командная строка'
scn = scenario.Scenario(cli, srv, scn_name)


class Process:
  def __init__(self, cmd):
    self.args = shlex.split(cmd)
    if verbose: print(self.args)
    self.process = subprocess.Popen(
      self.args,
      stdin=subprocess.PIPE,
      stdout=subprocess.PIPE,
      start_new_session=True
    )

  def poll_and_read(self, buffer_size=4000):
    try:
      return self.process.poll(), self.process.stdout.read1(buffer_size).decode('utf-8')
    except Exception as e:
      if verbose: print(str(e))
    finally:
      self.process.terminate()

  def write_and_flush(self, data):
    try:
      self.process.stdin.write(data + '\n')
      self.process.stdin.flush()
    except Exception as e:
      if verbose: print(str(e))
    finally:
      self.terminate()

  def mk_msg(msg):
    return '[cmd] `' + msg + '`'

  def terminate(self):
    self.process.terminate()


def pna(process):
  try:
    sleep(0.001)
    poll, out = process.poll_and_read()
    if poll is not None and out:
      cli.send_msg(Process.mk_msg(out))
      _, out = process.poll_and_read()
      while out:
        cli.send_msg(Process.mk_msg(out))
        _, out = process.poll_and_read()
    else:
      if verbose: print('Long-time process')
      if out: scn.send_msg(Process.mk_msg(out))
      poll, out = process.poll_and_read()
      while poll is None:
        if verbose: print('poll is None')
        sleep(0.01)
        if out: cli.send_msg(Process.mk_msg(out))
        in_cmd = scn.listen()
        if 'content' not in in_cmd: continue
        if not len(in_cmd['content']): continue
        process.write_and_flush(in_cmd['content'])
        poll, out = process.poll_and_read()
      while out:
        scn.send_msg(Process.mk_msg(out))
        _, out = process.poll_and_read()
      scn.terminate()
  except Exception as e:
    if str(e):
      print(str(e))
      err_msg = 'Got an error: ' if lang != 'ru' else 'Произошла ошибка: '
      scn.terminate()
      cli.send_error(err_msg + str(e))
  finally:
    process.terminate()


def exec_cmd(cmd):
  try:
    process = Process(cmd)
    if verbose: print('Process is started.')
    pna(process)
  except Exception as e:
    if str(e):
      print(str(e))
      scn.terminate()
      cli.send_error(str(e))
  if verbose: print('Process finished')


def main():
  cli.send_info('[cmd] Extension started.' if lang != 'ru' else '[cmd] Расширение запущено.')
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
    exec_cmd(cc)


try:
  main()
except KeyboardInterrupt:
  print('\nCommand Line\' Extension is off.' if lang != 'ru' else '\nРасширение Командная строка выключено.')
