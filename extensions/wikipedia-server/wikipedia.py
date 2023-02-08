#!/usr/bin/env python

import argparse, wikipediaapi
from jeff_api import client, server, scenario

import locale
lang, _ = locale.getdefaultlocale()

parser = argparse.ArgumentParser(description="Wikipedia is the Jeff extension that allows you to get brief information about everything from Wikipedia.")
parser.add_argument("extension_port", type=int, help="extension's server port")
parser.add_argument("jeff_port", type=int, help="Jeff port")
parser.add_argument("-v", "--verbose", action="store_true", help="verbose output")

args = parser.parse_args()
extension_port = args.extension_port
jeff_port = args.jeff_port
verbose = args.verbose

srv = server.Server(None, extension_port)
cli = client.Client('localhost', jeff_port)
scn = scenario.Scenario(cli, srv)


def main():
  wiki = wikipediaapi.Wikipedia(lang[:2])
  


try:
  main()
except KeyboardInterrupt:
  print('\nWikipedia Extension is off.' if lang != 'ru_RU' else '\nРасширение Wikipedia выключено.')
