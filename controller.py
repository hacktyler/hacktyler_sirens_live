#!/usr/bin/env python

import serial
import time

import pusherclient

def new_active_call(data):
    print 'new_active_call'
    arduino.write('a')

def connection_established(data):
    print 'connection_established'
    channel = pusher.subscribe('active-calls')
    channel.bind('new_active_call', new_active_call)

arduino = serial.Serial('/dev/tty.usbmodem1421', 9600)

pusher = pusherclient.Pusher('d20fddb74c58823cd05d')
pusher.connection.bind('pusher:connection_established', connection_established)

while True:
    time.sleep(1)
