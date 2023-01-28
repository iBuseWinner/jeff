#!/usr/bin/env python

import aiml, asyncio, json, os.path, locale
from jeff_api import client, server

lang, _ = locale.getdefaultlocale()

parser = argparse.ArgumentParser(description="Alice AIML is an extension that integrates the PyAIML processor into Jeff.")
parser.add_argument("extension_port", type=int, help="extension's server port")
parser.add_argument("jeff_port", type=int, help="Jeff port")
parser.add_argument("-v", "--verbose", action="store_true", help="verbose output")

args = parser.parse_args()
extension_port = args.extension_port
jeff_port = args.jeff_port
verbose = args.verbose


def main():
  srv = server.Server(None, extension_port)
  cli = client.Client('localhost', jeff_port)
  aiml_kernel = aiml.Kernel()
  aiml_kernel.setTextEncoding(None)
  current_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'files', 'standard')
  aiml_kernel.bootstrap(learnFiles='startup.xml', commands='load aiml b', chdir=current_path)
  # chdir = os.path.join(aiml.__path__[0], 'botdata', 'alice')
  # kern.bootstrap(learnFiles="startup.xml", commands="load alice", chdir=chdir)
  # kern.saveBrain(args.save)
  # kern.bootstrap(brainFile=args.brain)
  while True:
    data = srv.listen()
    if len(data) == 0:
      continue
    if data['author'] == 1: # Jeff
      continue
    if len(data['content']) == 0:
      continue
    if verbose:
      print('Got message: ' + data['content'])
    msg = aiml_kernel.respond(data['content'])
    if not len(msg):
      continue
    if verbose:
      print('AIML answer: ' + msg)
    cli.send_msg(msg)


try:
  main()
except KeyboardInterrupt:
  print('\nДемон AIML отключен.' if lang != 'ru_RU' else '\nAIML daemon is off.')
