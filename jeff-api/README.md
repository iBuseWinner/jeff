# jeff-api

Python library for Jeff interaction. Supports both `jeff-qt` and `jeff-core`.

## Installing from PyPi.org

```bash
pip install jeff-api
```

## Usage

```python
from jeff_api import server, client

srv = server.Server(None, port)
cli = client.Client('localhost', 8005)

data = srv.listen()
cli.send_msg(data)
```

## Building

```bash
cd jeff-api
python -m pip install --upgrade build
python -m build
```
