import json, socket

class Client:
  def __init__(self, ip, port):
    self.ip = ip
    self.socket_port = port
  
  def _send(self, data):
    try:
      with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((self.ip, self.socket_port))
        sock.sendall(data)
    except ConnectionRefusedError:
      print('Jeff\'s socket is disabled.')
  
  def _accept(self, data, buffer_size):
    try:
      with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((self.ip, self.socket_port))
        sock.sendall(data)
        data = sock.recv(buffer_size)
    except ConnectionRefusedError:
      print('Jeff\'s socket is disabled.')
    return data
  
  def _encode_json(j):
    return json.dumps(j).encode()
  
  def _decode_json(b):
    return json.loads(b.decode())
  
  def send_msg(self, msg):
    j = {"send": msg}
    self._send(Client._encode_json(j))

  def send_json(self, j):
    self._send(Client._encode_json(j))

  def send_as_user(self, msg):
    j = {"send_as_user": msg}
    self._send(Client._encode_json(j))
