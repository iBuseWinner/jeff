#!/usr/bin/env python

import aiml, asyncio, json, os.path, server, transport


async def main() -> None:
  t = transport.Transport(8005)
  s = server.Server(8017)
  aiml_kernel = aiml.Kernel()
  aiml_kernel.setTextEncoding(None)
  current_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'files', 'standard')
  aiml_kernel.bootstrap(learnFiles='startup.xml', commands='load aiml b', chdir=current_path)
  # chdir = os.path.join( aiml.__path__[0],'botdata','alice' )
  # kern.bootstrap(learnFiles="startup.xml", commands="load alice", chdir=chdir)
  # kern.saveBrain(args.save)
  # kern.bootstrap(brainFile=args.brain)
  while True:
    data = s.listen()
    if len(data) == 0:
      continue
    if data['author'] == 1: # Jeff
      continue
    if len(data['content']) == 0:
      continue
    msg = aiml_kernel.respond(data['content'])
    if not len(msg):
      continue
    t.send_msg(msg)


if __name__ == "__main__":
  try:
    asyncio.run(main())
  except KeyboardInterrupt:
    print('\nДемон AIML отключен.')
