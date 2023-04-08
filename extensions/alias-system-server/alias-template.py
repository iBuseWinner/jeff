from jeff_api import client
import json, re


def handle_asterisks(activator, reagent) -> str:
  reagent = reagent.replace('\\*', '<used>').replace('*', '(.*?)').replace('<used>', '\\*')
  length = len(re.findall("(\\(.*?\\))", reagent))
  match = re.search(reagent, activator)
  for i in range(length):
    reagent = reagent.replace('(.*?)', match.group(i + 1), 1)
  return reagent


def main(args: str) -> str:
  args: dict = json.loads(args)
  activator: str = args['user_input']
  reagent = '<ALIAS>'
  modified_reagent: str = handle_asterisks(activator, reagent)
  client.Client('localhost', <JEFF_PORT>).send_as_user(modified_reagent)
  return '{"ignore": true}'.encode()
