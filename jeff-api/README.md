# jeff-api

Библиотека Python для взаимодействия с Джеффом. Поддерживает как `jeff-qt`, так и `jeff-core`.

## Установка из публичного репозитория PyPi.org

```bash
pip install jeff-api
```

## Использование

```python
from jeff_api import server, client

srv = server.Server(None, port)
cli = client.Client('localhost', 8005)

data = srv.listen()
cli.send_msg(data)
```

## Сборка

```bash
cd jeff-api
python -m pip install --upgrade build
python -m build
```
