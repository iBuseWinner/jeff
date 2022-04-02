#!/usr/bin/env python

import aiml, asyncio, json, os.path, server, transport


async def main() -> None:
  t = transport.Transport(8005)
  s = server.Server(8017)
  aiml_kernel = aiml.Kernel()
  aiml_kernel.setTextEncoding(None)
  current_path = os.path.abspath(os.path.dirname(__file__))
  while True:
    data = s.listen()
    if len(data) == 0:
      continue


if __name__ == "__main__":
  try:
    asyncio.run(main())
  except KeyboardInterrupt:
    print('\nЧасы отключены.')
