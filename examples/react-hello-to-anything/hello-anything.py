#!/usr/bin/env python

import json


def ret(args):
  args = json.loads(args)
  print(args)
  return json.dumps({"send": "Hello anything..."}).encode()
