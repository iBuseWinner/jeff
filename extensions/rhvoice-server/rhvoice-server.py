#!/usr/bin/env python

from jeff_api import client, server
import argparse, speechd, subprocess

parser = argparse.ArgumentParser(description="RHVoice' TTS server for Jeff.")
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
  tts_d = speechd.SSIPClient('test')
  tts_d.set_output_module('rhvoice')
  tts_d.set_rate(10)
  voices = tts_d.list_synthesis_voices()
  for voice in voices:
    if voice[1] == lang:
      tts_d.set_synthesis_voice(voice[0])
  tts_d.set_punctuation(speechd.PunctuationMode.NONE)
  cli.send_info('[RHVoice] Extension activated.' if lang != 'ru' else '[RHVoice] Расширение активировано.')
  while True:
    data = srv.listen()
    if len(data) == 0: continue
    if not 'content_type' in data: continue
    if data['content_type'] not in (1, 2): continue
    if data['author'] != 1: continue
    text = data['content']
    tts_d.speak(text)


try: main()
except KeyboardInterrupt: print('\nRHVoice disabled.' if lang != 'ru' else '\nRHVoice отключён.')
