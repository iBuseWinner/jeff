#!/usr/bin/env python

import transport

def try_locate(t: transport.Transport) -> bool:
  """"""
  import os.path
  if not os.path.exists('darknet'):
    # download darknet for linux/macos/windows, but now...
    # or download tensorflow lite for linux & other platforms.
    return False
  return True

def main(t: transport:Transport) -> None:
  """"""
  import subprocess
  path = ""
  if path:
    subprocess.call(["./darknet", "detect", "cfg/yolov4-tiny.cfg", "yolov4-tiny.weights", path])

if __name__ == "__main__":
  t = transport.Transport(8005)
  if try_locate(t):
    main(t)
