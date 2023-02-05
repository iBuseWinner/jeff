from . import client, server
import json, socket

class ScenarioTerminatedException(Exception):
  "The scenario is ended by the user forcibly."
  pass

class ScenarioNotStartedException(Exception):
  "The scenario is ended by the user forcibly."
  pass

class Scenario:
  def __init__(self, cli, srv):
    self.cli = cli
    self.srv = srv
    self.init = False
    self.token = None
    self.nis = False

  def _encode_json(j):
    return json.dumps(j).encode()

  def _decode_json(b):
    return json.loads(b.decode())

  def _init_scenario(self, j):
    self.srv.server_socket.settimeout(5)
    j |= {"sready": True, "saddr": self.srv.host, "sport": self.srv.port}
    self.cli._send(Scenario._encode_json(j))
    res = Scenario._decode_json(self.srv._waits_for())
    self.srv.server_socket.settimeout(None)
    if "stoken" not in res:
      raise ScenarioNotStartedException
    self.token = res["stoken"]
    self.init = True

  def _continue_scenario(self, j):
    j |= {"stoken": self.token, "scontinue": True}
    self.cli._send(Scenario._encode_json(j))

  def _terminate_scenario(self, j):
    j |= {"stoken": self.token, "sfinish": True}
    self.token = None
    self.init = False
    self.cli._send(Scenario._encode_json(j))

  def _decide(self, j):
    if self.init:
      self._continue_scenario(j)
    elif self.nis:
      self._terminate_scenario(j)
    else:
      self._init_scenario(j)

  def send_msg(self, msg):
    j = {"send": msg}
    self._decide(j)

  def send_as_user(self, msg):
    j = {"send_as_user": msg}
    self._decide(j)

  def send_status(self, msg_id, msg):
    j = {"send_status": {"id": msg_id, "msg": msg}}
    self._decide(j)

  def send_error(self, msg):
    j = {"send_warning": msg}
    self._decide(j)

  def store_cells(self, values_dict):
    j = {"store_in_memory": values_dict}
    self._decide(j)

  def read_cells(self, keys_arr, buffer_size=8192):
    if not self.init or self.nis:
      return None
    j = {"memory_cells": keys_arr}
    j = Scenario._decode_json(self._accept(Scenario._encode_json(j), buffer_size))
    if "memory_values" not in j:
      return None
    else:
      return j

  def listen(self):
    if not self.init or self.nis:
      raise ScenarioNotStartedException
    j = Scenario._decode_json(self.srv._waits_for())
    if "sfinish" in j:
      if j["sfinish"] is True:
        raise ScenarioTerminatedException
    return j

  def next_is_last(self):
    self.nis = True

  def wait(self):
    while True:
      msg = self.listen()
      if len(msg) == 0:
        continue
      if msg['author'] == 1:
        continue
      return msg
