#!/usr/bin/env python

import os

def provide_action():
    return [
        ('/konsole_exec', 'Executes something in Konsole'),
        ('/run', 'Runs someapp')
        ]

def konsole_exec(command):
    stream = os.popen('konsole --hold -e {}'.format(command))
    output = stream.readlines()
    return output

def run(command):
    stream = os.popen(command)
    output = stream.readlines()
    return output

print(run(input()))
