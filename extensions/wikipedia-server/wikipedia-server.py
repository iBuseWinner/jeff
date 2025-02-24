#!/usr/bin/env python

import argparse, string
import wikipedia as wiki
from jeff_api import client, server, scenario

ATTENTION_WORDS = ['что такое', 'кто такой', 'кто такая', 'what is the', "what's the", "who is"]

parser = argparse.ArgumentParser(description="Wikipedia is the Jeff extension that allows you to get brief information about everything from Wikipedia.")
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

scn_name = 'Summaries from Wikipedia' if lang != 'ru' else 'Выжимки из Википедии'
scn = scenario.Scenario(cli, srv, scn_name)


def try_summary(query):
  try:
    msg = wiki.summary(query)
    scn.send_msg(msg)
  except Exception as e:
    scn.send_error(str(e))
    scn.terminate()
    return


def summary_or_select(query):
  try:
    msg = wiki.summary(query)
    if not wn: scn.send_msg(msg)
    else: scn.send_msg('**[Wiki]** ' + msg)
  except wiki.DisambiguationError:
    if verbose: print('Several options')
    variants = wiki.search(query, results=10, suggestion=True)
    if verbose: print(variants[1])
    variants = [v for v in variants[0]]
    ep = 'Select one of these articles:' if lang != 'ru' else 'Выберите одну из этих статей:'
    variants = [ep] + variants
    msg = '  \n'.join([variants[0]] + ['. '.join([str(i), variants[i]]) for i in range(1, len(variants))])
    scn.send_msg(msg)
    op = scn.wait()
    if verbose: print(f'"{op}"')
    if not op.isdigit():
      scn.terminate()
      return
    op = int(op)
    if op > len(variants) or op == 0:
      scn.terminate()
      return
    query = variants[op]
    try_summary(query)
  scn.send_msg('Want to know more? **(y/[n])**' if lang != 'ru' else 'Хотите узнать больше? **(y/[n])**')
  op = scn.wait()
  if verbose: print(op)
  if 'y' not in op:
    if verbose: print('Not wanted to know more')
    scn.terminate()
    return
  page = wiki.page(query)
  msg = page.content
  if len(msg) > 8000:
    for i in range((len(msg) // 8000) - 1):
      tmp = msg[:8000]
      scn.send_msg(tmp)
      msg = msg[8000:]
  scn.send_msg(msg, last=True)


def wiki_navigator(query, at_words):
  query = query[(query.find(at_words) + len(at_words) + 1):]
  query = query.translate(str.maketrans('', '', string.punctuation)).capitalize()
  try:
    summary_or_select(query)
  except scenario.ScenarioTerminatedException:
    pass
  except scenario.ScenarioNotStartedException:
    pass
  except wiki.exceptions.PageError:
    pass


def main():
  wiki.set_lang(lang)
  cli.send_info('[Wiki] Extension started.' if lang != 'ru' else '[Wiki] Расширение запущено.')
  while True:
    data = srv.listen()
    if len(data) == 0: continue
    if 'content' not in data: continue
    cc = data['content'].lower()
    for at_words in ATTENTION_WORDS:
      if at_words in cc:
        if verbose: print(f'Found "{at_words}"')
        wiki_navigator(data['content'], at_words)


try:
  main()
except KeyboardInterrupt:
  print('\nWikipedia Extension is off.' if lang != 'ru' else '\nРасширение Wikipedia выключено.')
