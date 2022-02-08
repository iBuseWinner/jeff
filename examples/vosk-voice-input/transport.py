import json, socket

class Transport:
  def __init__(self, port):
    self.socket_port = port
  
  def _send(self, data):
    try:
      with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(('localhost', self.socket_port))
        sock.sendall(data)
    except ConnectionRefusedError:
      print('\tСервер отключен.')
  
  def _accept(self, data, buffer_size):
    try:
      with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(('localhost', self.socket_port))
        sock.sendall(data)
        data = sock.recv(buffer_size)
    except ConnectionRefusedError:
      print('\tСервер отключен.')
    return data
  
  def _encode_json(j):
    return json.dumps(j).encode()
  
  def _decode_json(b):
    return json.loads(b.decode())
  
  def send_msg(self, msg):
    j = {"send": msg}
    self._send(Transport._encode_json(j))
