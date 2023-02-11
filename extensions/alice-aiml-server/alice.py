#!/usr/bin/env python

import aiml, asyncio, json, os.path, argparse, signal, sys, uuid
from jeff_api import client, server

parser = argparse.ArgumentParser(description="Alice AIML is an extension that integrates the PyAIML processor into Jeff.")
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

def main():
  aiml_kernel = aiml.Kernel()
  aiml_kernel.setTextEncoding(None)
  load_status_id = str(uuid.uuid4())
  cli.send_status(load_status_id, '*[Alice] Waiting...*' if lang != 'ru' else '*[Alice] Ожидание...*')
  if os.path.isfile('brain.brn') and os.path.isfile('sessions.brn'):
    aiml_kernel.bootstrap(brainFile='brain.brn', sessionsFile='sessions.brn')
  else:
    current_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'files', 'alice')
    aiml_kernel.bootstrap(learnFiles="startup.xml", commands="load alice", chdir=current_path)
    current_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'files', 'standard')
    aiml_kernel.bootstrap(learnFiles='startup.xml', commands='load aiml b', chdir=current_path)
  cli.send_status(load_status_id, '[Alice] Kernel is ready to work.' if lang != 'ru' else '[Alice] Ядро готово к работе.')
  
  class Responder:
    def __init__(self, aiml_kernel):
      self.aiml_kernel = aiml_kernel
      signal.signal(signal.SIGINT, self.exit_gracefully)
      signal.signal(signal.SIGTERM, self.exit_gracefully)

    def exit_gracefully(self, *args):
      self.aiml_kernel.saveBrain('brain.brn', 'sessions.brn')
      sys.exit(0)
      
    def run_loop(self, cli, srv):
      while True:
        data = srv.listen()
        if len(data) == 0: continue
        if data['author'] == 1: continue
        if len(data['content']) == 0: continue
        if verbose: print('Got message: ' + data['content'])
        msg = self.aiml_kernel.respond(data['content'])
        if not len(msg): continue
        if verbose: print('AIML answer: ' + msg)
        cli.send_msg(msg)
        
  Responder(aiml_kernel).run_loop(cli, srv)


try:
  main()
except KeyboardInterrupt:
  print('\nДемон AIML отключен.' if lang != 'ru' else '\nAIML daemon is off.')
