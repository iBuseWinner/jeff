#!/usr/bin/env python

import argparse, json, os, uuid
from jeff_api import client, server, scenario


parser = argparse.ArgumentParser(description="Alias Maker is an extension that creates aliases for your phrases.")
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

scn_name = 'Alias making' if lang != 'ru' else 'Создание альяса'
scn = scenario.Scenario(cli, srv, scn_name)


def start_alias_maker_scenario(content: str) -> None:
  activator: str = ''
  reagent: str = ''
  if len(content):
    # `@alias Search * in the Internet -> @google *`
    cmds: list[str] = content.split(' -> ')
    if len(cmds) != 2:
      return
    for i in range(len(cmds)):
      cmds[i] = cmds[i].replace('\\->', '->')
    activator = cmds[0]
    reagent = cmds[1]
  else:
    # `@alias`
    scn.send_msg('Enter the alias:' if lang != 'ru' else 'Введите альяс:')
    cmds.append(scn.wait())
    scn.send_msg('Enter the full expression:' if lang != 'ru' else 'Введите полное выражение:')
    cmds.append(scn.wait())
    scn.terminate()
  f = open('alias-template.py', 'r')
  text: str = f.read()
  f.close()
  text = text.replace('<JEFF_PORT>', str(jeff_port))
  text = text.replace('<ALIAS>', reagent)
  filename = str(uuid.uuid4()) + '.py'
  filename = filename.replace('-', '')
  filename = f'aliases/alias_{filename}'
  f = open(filename, 'w')
  f.write(text)
  f.close()
  savepath: str = os.path.abspath(filename)
  react_script: dict = {
    "stype": 1,
    "filepath": savepath,
    "fn_name": "main",
    "required_user_input": True
  }
  cli.send_json({
    "add_expr": [
      {
        "activator_text": activator,
        "reagent_text": json.dumps(react_script),
        "exec": True,
        "properties": {}
      }
    ]
  })


def main() -> None:
  cli.send_info('[Alias] Extension started.' if lang != 'ru' else '[Alias] Расширение запущено.')
  if not os.path.isdir('aliases'): os.mkdir('aliases')
  while True:
    data: dict = srv.listen()
    if len(data) == 0: continue
    if data['author'] != 0: continue
    received: str = data['content']
    if verbose: print(f'Got message: {received}')
    start_alias_maker_scenario(received)


try:
  main()
except KeyboardInterrupt:
  print('\nAlias Maker is off.' if lang != 'ru' else '\nСоздатель альясов отключен.')
