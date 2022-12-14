#!/usr/bin/env python

import transport

# Shutdown flag. Used in main() to stop script.
SHUTDOWN = False
# A list of words that indicate that a person is talking to the bot.
ATTENTION_WORDS = [word for word in 'эй, хэй, хай, ваня'.split(', ')]


def list_subdirs(path: str) -> list:
  """Lists the subfolders of the 'path' folder."""
  from os import listdir
  from os.path import isdir, join
  return [f for f in listdir(path) if isdir(join(path, f))]


def shutdown() -> None:
  """Turns off speech recognition."""
  SHUTDOWN = True


def try_import(t: transport.Transport) -> bool:
  """Checks if required modules are installed and installs otherwise."""
  try:
    import sounddevice, vosk
  except ImportError:
    import subprocess
    try:
      print('Installing modules via "pip"...')
      print(subprocess.run(
        ["pip", "install", "sounddevice", "vosk", "--user"], capture_output=True).stdout.decode()
      )
    except subprocess.CalledProcessError:
      msg = 'Installation failed. Please install "sounddevice" and "vosk" Python modules manually before using vosk-voice-input so that Jeff can hear and listen to you.'
      print(msg)
      t.send_msg(msg)
      return False
  import platform, os.path, webbrowser
  # The binary library for connecting to a microphone for the sounddevice module is downloaded
  # automatically for Windows and Mac OS X. Therefore, we check if you are using Linux.
  if platform.system() == 'Linux':
    if not os.path.exists('/usr/lib/libportaudio.so'):
      msg = 'Your Linux distribution does not have the PortAudio library installed. Install it to use vosk-voice-input. Opening browser...'
      print(msg)
      t.send_msg(msg)
      webbrowser.open('https://pkgs.org/search/?q=portaudio')
      return False
  if not os.path.exists('models') or len(list_subdirs('models')) == 0:
    msg = 'You don\'t have any VOSK models installed. Download the one suitable for you from the site, create a "models" folder and unzip the model into it. Opening browser...'
    print(msg)
    t.send_msg(msg)
    webbrowser.open('https://alphacephei.com/vosk/models')
    return False
  return True


def main(t: transport.Transport) -> None:
  """Starts speech recognition."""
  import sounddevice, vosk, locale, queue, os.path, json
  
  # We select the appropriate model from the list of downloaded ones according to the language used 
  # in the system. If there is no suitable model, we take the first one that comes across. 
  #
  # You can specify a specific model by entering its name below instead of 'searched_folders[0]'.
  lang, _ = locale.getdefaultlocale()
  guess = '-' + lang[:2] + '-'
  searched_folders = [f for f in list_subdirs('models') if guess in f]
  if not searched_folders:
    searched_folders = list_subdirs('models')
  selected_model = searched_folders[0]
  print(f'Selected "{selected_model}".')
  vosk_model = vosk.Model(os.path.join('models', selected_model))
  
  audio_block_queue = queue.Queue()
  def checkout(indata, frames, time, status):
    """Writes recorded audio to queue that handled below."""
    if status:
      print(status, file=sys.stderr)
    audio_block_queue.put(bytes(indata))
  
  # Usually personal computers and laptops are equipped with a maximum of one microphone, so 
  # if there are microphones at all, we will choose the first one that comes across.
  #
  # If you have more than one microphone, you can specify which microphone to use by assigning
  # its name to 'device' kwarg.
  with sounddevice.RawInputStream(blocksize=8000, dtype='int16', channels=1, callback=checkout):
    sample_rate = int(sounddevice.query_devices(sounddevice.default.device, "input")["default_samplerate"])
    vosk_recognizer = vosk.KaldiRecognizer(vosk_model, sample_rate)
    print('Let\'s start recognizing...')
    t.send_msg('Voice input activated.')
    try:
      while True:
        if SHUTDOWN:
          break
        data = audio_block_queue.get()
        if vosk_recognizer.AcceptWaveform(data):
          text = json.loads(vosk_recognizer.Result())["text"]
          for word in text.split():
            if word in ATTENTION_WORDS:
              print(' - ' + text)
              t.send_as_user(text)
              break
    except KeyboardInterrupt:
      print('\nSpeech recognition is off.')


if __name__ == "__main__":
  t = transport.Transport(8005)
  if try_import(t):
    main(t)
