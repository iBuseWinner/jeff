#!/usr/bin/env python

import credentials, argparse, wolframalpha
from jeff_api import client, server

# WARNING To use credentials, you have to fill it in `credentials-example.py` file and rename it to `credentials.py`.
app_id = credentials.APP_ID

parser = argparse.ArgumentParser(description="An extension that uses the Wolfram Alpha API to solve problems and answer questions.")
parser.add_argument("extension_port", type=int, help="extension's server port")
parser.add_argument("jeff_port", type=int, help="Jeff port")
parser.add_argument("-v", "--verbose", action="store_true", help="verbose output")

args = parser.parse_args()
extension_port = args.extension_port
jeff_port = args.jeff_port
verbose = args.verbose

srv = server.Server(None, extension_port)
cli = client.Client('localhost', jeff_port)
lang = cli.read_cells(['jeff-lang'])['jeff-lang']

wa_cli = wolframalpha.Client(app_id)

def main():
  cli.send_info('[Wolfram] Extension started.' if lang != 'ru' else '[Wolfram] Расширение запущено.')
  while True:
    data = srv.listen()
    if len(data) == 0: continue
    if 'author' not in data: continue
    if data['author'] == 1: continue
    if data['content_type'] not in (1, 2): continue
    if not data['content'].startswith('/w '): continue
    if len(data['content']) <= 3: continue
    text = data['content'][3 : ]
    res = wa_cli.query(text)
    answer = next(res.results).text
    cli.send_msg(answer)


try:
  main()
except KeyboardInterrupt:
  print('\nWolfram is off.' if lang != 'ru' else '\nWolfram выключен.')
