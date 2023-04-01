#!/usr/bin/env python

import argparse, credentials, openai
from jeff_api import client, server

KEEP_QNA = 9

parser = argparse.ArgumentParser(description="Chat resolver written on top of OpenAI's ChatGPT model, also used in a similar way in ChatGPT.")
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

# WARNING To use credentials, you have to fill it in `credentials-example.py` file and rename it to `credentials.py`.

# if lang == "ru":
#   os.environ["HTTP_PROXY"] = credentials.HTTP_PROXY
#   os.environ["HTTPS_PROXY"] = credentials.HTTPS_PROXY
openai.api_key = credentials.API_KEY


def generate_response(msg):
  try:
    completion = openai.ChatCompletion.create(
      model="gpt-3.5-turbo",
      messages=[
        {"role": "user", "content": msg}
      ]
    )
    message = completion.choices[0].message.content
    return message
  except openai.error.RateLimitError:
    cli.send_error("*[ChatGPT]* Server is overloaded. Try again later." if lang != 'ru' else "*[ChatGPT]* Сервер перегружен. Попробуйте повторить попытку позже.")
  except openai.error.ServiceUnavailableError:
    cli.send_error("*[ChatGPT]* Server is unaccessible. Try again using proxy." if lang != 'ru' else "*[ChatGPT]* Сервер недоступен. Попробуйте использовать прокси.")
  except openai.error.APIConnectionError:
    cli.send_error("*[ChatGPT]* Unable to connect to API. Try to change proxy." if lang != 'ru' else "*[ChatGPT]* Не удалось подключиться к API. Попробуйте сменить прокси.")
  except Exception as e:
    cli.send_error(str(e))
    return str()


def main():
  if verbose: print('ChatGPT activated.' if lang != 'ru' else 'ChatGPT активирован.')
  cli.send_info('[ChatGPT] Extension activated.' if lang != 'ru' else '[ChatGPT] Расширение активировано.')
  while True:
    data = srv.listen()
    if len(data) == 0: continue
    if 'content_type' not in data: continue
    if data['content_type'] not in (1, 2): continue
    if len(data['content']) < 10: continue
    if data['author'] == 1: continue
    text = data['content']
    if verbose: print('*[ChatGPT] Waiting...*' if lang != 'ru' else '*[ChatGPT] Ожидание...*')
    cli.send_msg('*[ChatGPT] Waiting...*' if lang != 'ru' else '*[ChatGPT] Ожидание...*')
    response = generate_response(text)
    if len(response) == 0:
      cli.send_msg('[ChatGPT] Asnwer is not found.' if lang != 'ru' else '[ChatGPT] Ответ не найден.')
    else:
      if not wn: cli.send_msg(response.capitalize())
      else: cli.send_msg('**[ChatGPT]** ' + response.capitalize())
      if verbose: print(response)


try:
  main()
except KeyboardInterrupt:
  print('\nChatGPT disabled.' if lang != 'ru' else '\nChatGPT отключён.')
