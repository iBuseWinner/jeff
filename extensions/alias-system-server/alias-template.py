from jeff_api import client


def main(_):
  client.Client('localhost', '<JEFF_PORT>').send_as_user('<ALIAS>')
  return '{"ignore": true}'
