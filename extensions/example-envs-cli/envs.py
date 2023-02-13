#!/usr/bin/env python

import argparse, os
from jeff_api import client

parser = argparse.ArgumentParser(description="The example extension with some enviromental variables.")
parser.add_argument("jeff_port", type=int, help="Jeff port")
parser.add_argument("-v", "--verbose", action="store_true", help="verbose output")

args = parser.parse_args()
jeff_port = args.jeff_port
verbose = args.verbose

cli = client.Client('localhost', jeff_port)

cli.send_msg(f'{os.environ["VAR1"]}, {os.environ["VAR2"]}')
