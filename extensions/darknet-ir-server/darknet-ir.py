#!/usr/bin/env python

from jeff_api import client, server
import json, subprocess, socket, os, uuid, locale, argparse

parser = argparse.ArgumentParser(description="Darknet & YOLOv7 Objects-on-Image Recognition Jeff's extension.")
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

def try_locate():
  if not os.path.exists('darknet'):
    err_msg = 'Darknet is not installed. Please install it from <a href=\"https://github.com/AlexeyAB/darknet\">this link</a> before using the extension.' if lang != 'ru' else 'Darknet не установлен. Пожалуйста, установите его по <a href=\"https://github.com/AlexeyAB/darknet\">ссылке</a>, прежде чем использовать расширение.'
    if verbose:
      print(err_msg)
    cli.send_error('[Darknet] ' + err_msg)
    return False
  if not (
    os.path.exists(os.path.join('darknet', 'darknet')) or
    os.path.exists(os.path.join('darknet', 'darknet.exe'))
  ):
    err_msg = 'Darknet is not compiled yet (darknet/darknet[.exe]). Please build it before using the extension (read darknet/README.md for building instructions).' if lang != 'ru' else 'Darknet не скомпилирован (darknet/darknet[.exe]). Пожалуйста, соберите его, прежде чем использовать расширение (прочтите darknet/README.md для инструкций по сборке).'
    if verbose: print(err_msg)
    cli.send_error('[Darknet] ' + err_msg)
    return False
  if not os.path.exists(os.path.join('darknet', 'cfg', 'yolov7-tiny.cfg')):
    err_msg = 'YOLOv7 model configuration file (darknet/cfg/yolov7-tiny.cfg) not found. Please download it from <a href=\"https://github.com/AlexeyAB/darknet\">this link</a> before using the extension.' if lang != 'ru' else 'Файл конфигурации модели YOLOv7 (darknet/cfg/yolov7-tiny.cfg) не найден. Пожалуйста, скачайте его по <a href=\"https://github.com/AlexeyAB/darknet\">ссылке</a>, прежде чем использовать расширение.'
    if verbose: print(err_msg)
    cli.send_error('[Darknet] ' + err_msg)
    return False
  if not os.path.exists(os.path.join('darknet', 'yolov7-tiny.weights')):
    err_msg = 'YOLOv7 model weights file (darknet/yolov7-tiny.weights) not found. Please download it from <a href=\"https://github.com/WongKinYiu/yolov7/releases/download/v0.1/yolov7-tiny.weights\">this link</a> before using the extension.' if lang != 'ru' else 'Файл весов модели YOLOv7 (darknet/yolov7-tiny.weights) не найден. Пожалуйста, скачайте его по <a href=\"https://github.com/WongKinYiu/yolov7/releases/download/v0.1/yolov7-tiny.weights\">ссылке</a>, прежде чем использовать расширение.'
    if verbose:
      print(err_msg)
    cli.send_error('[Darknet] ' + err_msg)
    return False
  return True


def recognize(request, savepath):
  if request["author"] == 1: return
  if not (request["content"].endswith('.jpg') or request["content"].endswith('.png')): return
  path = request["content"]
  file_ext = path[-4:]
  gotcha_msg = f'Got a file {path}.' if lang != 'ru' else f'Получен файл {path}.'
  if verbose: print(gotcha_msg)
  cli.send_info(gotcha_msg)
  subprocess.call(['./darknet' if os.path.exists("darknet") else 'darknet.exe',
                   "detect",
                   os.path.join("cfg", "yolov7-tiny.cfg"),
                   "yolov7-tiny.weights",
                   path])
  filename = f'{savepath}/{str(uuid.uuid4())}{file_ext}'
  os.rename('predictions.jpg', filename)
  filename = os.path.abspath(filename)
  if verbose: print(filename)
  cli.send_msg(filename)


def main():
  if not try_locate(): return
  if not os.path.isdir('recognized'): os.mkdir('recognized')
  savepath = os.path.abspath('recognized')
  os.chdir('darknet')
  print(f'Darknet enabled on port {str(extension_port)}.' if lang != 'ru' \
        else f'Darknet слушает на порту {str(extension_port)}.')
  while True:
    j = srv.listen()
    recognize(j, savepath)


try: main()
except KeyboardInterrupt: pass
finally: print('\nDarknet disabled.' if lang != 'ru' else '\nDarknet отключён.')
