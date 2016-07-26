import RPi.GPIO as GPIO
import time

servoSingalPin = 7
stepSingalPin = 11
stepDirPin = 12
stepSleepPin = 13
limitSwitchPin = 15
irlightPin = 16

GPIO.setmode(GPIO.BOARD)

GPIO.setup(servoSingalPin, GPIO.OUT)
GPIO.setup(stepSingalPin, GPIO.OUT)
GPIO.setup(stepDirPin, GPIO.OUT)
GPIO.setup(stepSleepPin, GPIO.OUT)
GPIO.setup(limitSwitchPin, GPIO.IN)
GPIO.add_event_detect(limitSwitchPin, GPIO.FALLING, callback=limitSwitch, bouncetime=200)
GPIO.setup(irlightPin, GPIO.OUT)

def limitSwitch(channel):  #limitSwitchStart
    print 'Hello'

while True:
	try:

	except KeyboardInterrupt:
		GPIO.cleanup()
GPIO.cleanup()