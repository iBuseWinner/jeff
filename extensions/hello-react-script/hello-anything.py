#!/usr/bin/env python

# Insert in db this phrase text to start `ret` function:
#
# {
#   "filepath": "extensions/hello-react-script/hello-anything.py",
#   "fn_name": "ret",
#   "required_history_parts": 2,
#   "required_user_input": true,
#   "stype": 1
# }

import json


def ret(args):
  args = json.loads(args)
  print(args)
  return json.dumps({"send": "Hello anything..."}).encode()
