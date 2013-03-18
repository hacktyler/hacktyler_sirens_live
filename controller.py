#!/usr/bin/env python

import serial
import sys
import time

import pusherclient

DEVICES = ['/dev/tty.usbmodem1411', '/dev/tty.usbmodem1421']   

def new_active_call(data):
    print 'new_active_call'
    arduino.write('a')

def connection_established(data):
    print 'connection_established'
    channel = pusher.subscribe('active-calls')
    channel.bind('new_active_call', new_active_call)

arduino = None

for device in DEVICES:
    try:
        arduino = serial.Serial(device, 9600)
        print 'Connected to device %s' % device
        break
    except:
        continue
else:
    print 'Could not connect to Arduino!'
    sys.exit()

pusher = pusherclient.Pusher('d20fddb74c58823cd05d')
pusher.connection.bind('pusher:connection_established', connection_established)

while True:
    time.sleep(1)
