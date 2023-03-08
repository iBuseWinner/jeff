#!/usr/bin/env python

import argparse, credentials, locale, openai, os
from jeff_api import client, server

KEEP_QNA = 9

parser = argparse.ArgumentParser(description="Chat resolver written on top of OpenAI's GPT-3 model, also used in a similar way in ChatGPT.")
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

prompt = "The following is a conversation with an AI assistant called Jeff. The assistant is helpful, clever and very friendly.\n\nHuman: Hello, who are you?\nAI: I am Jeff, automation tool. I use an AI model created by OpenAI. How can I help you today?" #if lang != 'ru' else "Ниже приведена переписка с ИИ-ассистентом. Ассистент полезный, умный и очень дружелюбный.\n\nЧеловек: Привет, кто ты?\nИИ: Я - Джефф, средство автоматизации. Я использую ИИ-модель, созданную OpenAI. Как я могу вам помочь?"

stop_keys = ["Human:", "AI:"] #if lang != 'ru' else ["Человек:", "ИИ:"]

ai_sequence = "\nAI: " #if lang != 'ru' else "\nИИ: " 
human_sequence = "\nHuman: " #if lang != 'ru' else "\nЧеловек: "

# WARNING To use credentials, you have to fill it in `credentials-example.py` file and rename it to `credentials.py`.

# if lang == "ru":
#   os.environ["HTTP_PROXY"] = credentials.HTTP_PROXY
#   os.environ["HTTPS_PROXY"] = credentials.HTTPS_PROXY
openai.api_key = credentials.API_KEY


def generate_response(prompt):
  try:
    completions = openai.Completion.create(
      model="text-davinci-003",
      prompt=prompt,
      temperature=0.9,
      max_tokens=1024,
      top_p=1,
      frequency_penalty=0,
      presence_penalty=0.6,
      stop=stop_keys
    )
    message = completions.choices[0].text
    return message
  except openai.error.RateLimitError:
    cli.send_error("*[GPT-3]* Server is overloaded. Try again later." if lang != 'ru' else "*[GPT-3]* Сервер перегружен. Попробуйте повторить попытку позже.")
  except openai.error.ServiceUnavailableError:
    cli.send_error("*[GPT-3]* Server is unaccessible. Try again using proxy." if lang != 'ru' else "*[GPT-3]* Сервер недоступен. Попробуйте использовать прокси.")
  except openai.error.APIConnectionError:
    cli.send_error("*[GPT-3]* Unable to connect to API. Try to change proxy." if lang != 'ru' else "*[GPT-3]* Не удалось подключиться к API. Попробуйте сменить прокси.")
  except Exception as e:
    cli.send_error(str(e))
    return str()


def history_reducer(history):
  if len(history) > KEEP_QNA >= 3:
    del history[2]
    del history[1]
  return history


def make_prompt(history):
  return ''.join(history) + ai_sequence


def main():
  history = [prompt]
  if verbose: print('GPT-3 activated.' if lang != 'ru' else 'GPT-3 активирован.')
  cli.send_info('[GPT-3] Extension activated.' if lang != 'ru' else '[GPT-3] Расширение активировано.')
  while True:
    data = srv.listen()
    if len(data) == 0: continue
    if not 'content_type' in data: continue
    if data['content_type'] not in (1, 2): continue
    if len(data['content']) < 10: continue
    text = data['content']
    if data['author'] == 1: continue
    if verbose: print('*[GPT-3] Waiting...*' if lang != 'ru' else '*[GPT-3] Ожидание...*')
    cli.send_status('*[GPT-3] Waiting...*' if lang != 'ru' else '*[GPT-3] Ожидание...*')
    history.append(human_sequence + text)
    response = generate_response(make_prompt(history)).strip()
    if len(response) == 0:
      cli.send_msg('[GPT-3] Asnwer is not found.' if lang != 'ru' else '[GPT-3] Ответ не найден.')
    else:
      history.append(response)
      if not wn: cli.send_msg(response.replace(ai_sequence, '').capitalize())
      else: cli.send_msg('**[GPT-3]** ' + response.replace(ai_sequence, '').capitalize())
      if verbose: print(response.replace(ai_sequence, ''))
      history = history_reducer(history)


try:
  main()
except KeyboardInterrupt:
  print('\nGPT-3 disabled.' if lang != 'ru' else '\nGPT-3 отключён.')
