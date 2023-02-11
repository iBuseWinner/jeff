import json, socket

class Server:
  def __init__(self, host, port):
    self.host = host
    self.port = port
    self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    self.server_socket.bind((host if host is not None else socket.gethostname(), port))
    self.server_socket.listen()
  
  def _waits_for(self, buffer_size=8192):
    (socket, address) = self.server_socket.accept()
    try:
      data = socket.recv(buffer_size)
      return data
    except ConnectionRefusedError:
      print("Connection refused.")
      return b"{}"
    except json.decoder.JSONDecodeError:
      print('JSON decode error.')
      return b"{}"
  
  def _encode_json(j):
    return json.dumps(j).encode()
  
  def _decode_json(b):
    return json.loads(b.decode())
  
  def listen(self):
    return Server._decode_json(self._waits_for())
