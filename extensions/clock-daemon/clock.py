#!/usr/bin/env python

import json, socket, time, uuid, argparse, locale
from jeff_api import client

desc = "Jeff Extension: clock that sends time to Jeff every 5 seconds."
port_help = "Jeff port"
verbose_help = "verbose output"

parser = argparse.ArgumentParser(description=desc)
parser.add_argument("-p", "--port", type=int, help=port_help)
parser.add_argument("-v", "--verbose", action="store_true", help=verbose_help)

args = parser.parse_args()
port = args.port
verbose = args.verbose

cli = client.Client('localhost', port)
lang, _ = locale.getdefaultlocale()

try:
  msg_id = str(uuid.uuid4())
  while True:
    curr_time = 'Current time is ' if lang != 'ru_RU' else 'Текущее время: '
    curr_time += f'{time.strftime("%X")}' + '.'
    if verbose:
      print('[5-Sec Precision Clock] ' + curr_time)
    cli.send_status(msg_id, curr_time)
    time.sleep(5)
except KeyboardInterrupt:
  pass
finally:
  print('\nClock is off.')
