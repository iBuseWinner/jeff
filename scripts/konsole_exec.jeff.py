#!/usr/bin/env python

import os, json

def action_provider():
  return json.dumps({'/kexec': 'Executes something in Konsole', '/krun': 'Runs someapp'})

def go_action(inp):
  if '/kexec' in inp:
    return konsole_exec(inp[6:])
  elif '/krun' in inp:
    return run(inp[6:])

def konsole_exec(command):
  stream = os.popen('konsole --hold -e {}'.format(command))
  output = stream.readlines()
  return json.dumps({'send': output})

def run(command):
  stream = os.popen(command)
  output = stream.readlines()
  return json.dumps({'send': output})
