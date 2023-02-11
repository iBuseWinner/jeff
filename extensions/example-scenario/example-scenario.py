#!/usr/bin/env python

from jeff_api import client, server, scenario

srv = server.Server(None, 30003)
cli = client.Client('localhost', 8005)
scn = scenario.Scenario(cli, srv)

try:
  scn.send_msg('Hello! You\'re in the scenario now!')
  msg = scn.wait()
  print(msg)
  scn.send_msg('2nd message')
  msg = scn.wait()
  print(msg)
  scn.send_msg('Bye!', last=True)
except scenario.ScenarioTerminatedException:
  print('terminated')
except scenario.ScenarioNotStartedException:
  print('not started')
except Exception as e:
  print(e) 
