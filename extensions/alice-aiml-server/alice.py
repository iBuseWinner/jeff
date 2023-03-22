#!/usr/bin/env python

import aiml, os.path, argparse, signal, sys
import argostranslate.package, argostranslate.translate
from jeff_api import client, server

parser = argparse.ArgumentParser(description="Alice AIML is an extension that integrates the PyAIML processor into Jeff.")
parser.add_argument("extension_port", type=int, help="extension's server port")
parser.add_argument("jeff_port", type=int, help="Jeff port")
parser.add_argument("-v", "--verbose", action="store_true", help="verbose output")
parser.add_argument("-w", "--with_name", action="store_true", help="send messages with identifier")

args = parser.parse_args()
extension_port = args.extension_port
jeff_port = args.jeff_port
verbose = args.verbose
wn = args.with_name

srv = server.Server(None, extension_port)
cli = client.Client('localhost', jeff_port)
lang = cli.read_cells(['jeff-lang'])['jeff-lang']


class Argos2Alice:
  def __init__(self, alice_lang, jeff_lang):
    self.alice_lang = alice_lang
    self.jeff_lang = jeff_lang
    if self.alice_lang != self.jeff_lang:
      cells = cli.read_cells(['argos2alice-downloaded'])
      if 'argos2alice-downloaded' not in cells:
        self.download()
      elif cells['argos2alice-downloaded'] != f'{self.jeff_lang}-{self.alice_lang}':
        self.download()
      try:
        installed_languages = argostranslate.translate.get_installed_languages()
        from_lang = list(filter(lambda x: x.code == self.jeff_lang, installed_languages))[0]
        to_lang = list(filter(lambda x: x.code == self.alice_lang, installed_languages))[0]
        self.j2al = from_lang.get_translation(to_lang)
        self.al2j = to_lang.get_translation(from_lang)
      except Exception as e:
        self.download()
        installed_languages = argostranslate.translate.get_installed_languages()
        from_lang = list(filter(lambda x: x.code == self.jeff_lang, installed_languages))[0]
        to_lang = list(filter(lambda x: x.code == self.alice_lang, installed_languages))[0]
        self.j2al = from_lang.get_translation(to_lang)
        self.al2j = to_lang.get_translation(from_lang)
      try:
        text = self.al2j.translate("Hello!")
        self.j2al.translate(text)
      except:
        cli.send_error('[Argos] Unable to get translator.' if lang != 'ru' else '[Argos] Невозможно воспользоваться переводчиком.')

  def download(self):
    if verbose: print('Installing translations')
    cli.send_info('[Argos] Translation models are downloading. This may take some time....' if lang != 'ru' else '[Argos] Скачиваются модели перевода. Это может занять какое-то время...')
    argostranslate.package.update_package_index()
    available_packages = argostranslate.package.get_available_packages()
    package_to_install = next(
      filter(lambda x: x.from_code == self.alice_lang and x.to_code == self.jeff_lang, available_packages)
    )
    if verbose: print(f'Found from {self.alice_lang} to {self.jeff_lang}')
    argostranslate.package.install_from_path(package_to_install.download())
    package_to_install = next(
      filter(lambda x: x.from_code == self.jeff_lang and x.to_code == self.alice_lang, available_packages)
    )
    if verbose: print(f'Found from {self.jeff_lang} to {self.alice_lang}')
    argostranslate.package.install_from_path(package_to_install.download())
    if verbose: print('Downloaded translations')
    cli.store_cells({'argos2alice-downloaded': f'{self.jeff_lang}-{self.alice_lang}'})
    cli.send_info('[Argos] Translations downloaded.' if lang != 'ru' else '[Argos] Переводы скачаны.')

  def to_aiml(self, text):
    try:
      if self.alice_lang == self.jeff_lang: return text
      return self.j2al.translate(text)
    except Exception:
      return text

  def from_aiml(self, text):
    try:
      if self.alice_lang == self.jeff_lang: return text
      return self.al2j.translate(text)
    except Exception:
        return text


class AliceResponder:
  def __init__(self, aiml_kernel):
    self.aiml_kernel = aiml_kernel
    self.tr = None

  def with_tr(self, tr):
    self.tr = tr
    return self

  def run_loop(self, cli, srv):
    while True:
      data = srv.listen()
      if len(data) == 0: continue
      if len(data['content']) == 0: continue
      if data['content'].startswith('/'): continue
      received = data['content']
      if verbose: print(f'Got message: {received}')
      if self.tr:
        received = self.tr.to_aiml(received)
        if verbose: print(f'Translated to: {received}')
      msg = self.aiml_kernel.respond(received)
      if not len(msg): continue
      if verbose: print('AIML answer: ' + msg)
      if self.tr:
        msg = self.tr.from_aiml(msg)
        if verbose: print(f'Translated back to: {msg}')
      if not wn: cli.send_msg(msg)
      else: cli.send_msg('**[Alice]** ' + msg)


def main():
  aiml_kernel = aiml.Kernel()
  aiml_kernel.setTextEncoding(None)
  if verbose: print('*[Alice] Waiting...*' if lang != 'ru' else '*[Alice] Ожидание...*')
  if os.path.isfile('brain.brn') and os.path.isfile('sessions.brn'):
    aiml_kernel.bootstrap(brainFile='brain.brn', sessionsFile='sessions.brn')
  else:
    current_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'files', 'alice')
    aiml_kernel.bootstrap(learnFiles="startup.xml", commands="load alice", chdir=current_path)
    current_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'files', 'standard')
    aiml_kernel.bootstrap(learnFiles='startup.xml', commands='load aiml b', chdir=current_path)
  translator = Argos2Alice('en', lang)
  alice = AliceResponder(aiml_kernel).with_tr(translator)

  def exit_gracefully(*args):
    aiml_kernel.saveBrain('brain.brn', 'sessions.brn')
    global translator
    del translator  # you MUST do this before (!) process terminating because of ctranslate2 side effects
    sys.exit(0)

  cli.send_info('[Alice] Kernel is ready to work.' if lang != 'ru' else '[Alice] Ядро готово к работе.')
  signal.signal(signal.SIGINT, exit_gracefully)
  signal.signal(signal.SIGTERM, exit_gracefully)
  alice.run_loop(cli, srv)


try:
  main()
except KeyboardInterrupt:
  print('\nAIML daemon is off.' if lang != 'ru' else '\nДемон AIML отключен.')
