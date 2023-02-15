#!/usr/bin/env python

import argparse, uuid, os
import google.auth
from jeff_api import client, server
from google.cloud import texttospeech
from playsound import playsound

parser = argparse.ArgumentParser(description="The extension voices Jeff's output.")
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

credentials, project = google.auth.default()
tts = texttospeech.TextToSpeechClient()
voice = texttospeech.VoiceSelectionParams(language_code=lang, ssml_gender=texttospeech.SsmlVoiceGender.NEUTRAL)
audio_config = texttospeech.AudioConfig(audio_encoding=texttospeech.AudioEncoding.MP3)


def main():
  while True:
    data = srv.listen()
    if len(data) == 0: continue
    if 'author' not in data: continue
    if data['author'] == 1: continue
    if data['content_type'] not in (1, 2): continue
    synthesis_input = texttospeech.SynthesisInput(text=data['content'])
    response = tts.synthesize_speech(input=synthesis_input, voice=voice, audio_config=audio_config)
    if verbose: print('Got response' if lang != 'ru' else 'Получен ответ')
    fn = f'speech/{str(uuid.uuid4())}.mp3'
    with open(fn, "wb") as out:
      out.write(response.audio_content)
      if verbose: print('Audio content written to file {fn}' if lang != 'ru' else 'Звук записан в файл {fn}')
    playsound(fn)


try:
  if not os.path.isdir('speech'): os.mkdir('speech')
  cli.send_info('[GTTS] TTS engine is ready.' if lang != 'ru' else '[GTTS] Движок TTS готов к работе.')
  main()
except KeyboardInterrupt:
  print('\nGoogle TTS is off.' if lang != 'ru' else '\nGoogle TTS выключен.')
