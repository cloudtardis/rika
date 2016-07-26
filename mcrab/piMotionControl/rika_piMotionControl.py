import RPi.GPIO as GPIO
import time

# pinNumber
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
GPIO.add_event_detect(limitSwitchPin, GPIO.FALLING, callback=limitSwitch, bouncetime=100)
GPIO.setup(irlightPin, GPIO.OUT)

# Variable
alpha = 0
beta = 0
irlight = 2

while True:
	try:

	except KeyboardInterrupt:
		GPIO.cleanup()
GPIO.cleanup()

def rx(rxBuff):
	buff[] = rxBuff.split()
	for i in range(0,10,2):
		if buff[i] == a_rel:
			alpha += float(buff[i + 1])
			if alpha < 0:
				alpha += 360
			elif alpha > 360
				alpha -= 360
		elif buff[i] == a_abs:
			alpha = float(buff[i + 1])
		elif buff[i] == b_rel:
			beta += float(buff[i + 1])
			if beta < 0:
				beta += 360
			elif beta > 360
				beta -= 360
		elif buff[i] == b_abs:
			beta = float(buff[i + 1])
		elif buff[i] == irlight:
			if buff[i + 1] == 0:
				pass
			elif buff[i + 1] == 1:
				pass
		elif buff[i] == cal:
			pass

def tx():

def motorCompute():
	

# limitSwitchStart
def limitSwitch(channel):  
    print 'Hello'


'''
def saveMethod(self):
    if self.e == None:
        self.displayText["text"] = "No Encrypt object can save!!"
    else:
        f = open('./code.txt', 'w')
        f.write(self.e.getCode())
        f.closed
        self.displayText["text"] = "The code is saved."
'''