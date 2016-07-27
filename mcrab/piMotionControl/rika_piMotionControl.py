import RPi.GPIO as GPIO
import time
import threading

# pinNumber
stepSingalPin = 11
stepDirPin = 12
stepSleepPin = 13
limitSwitchPin = 15
irlightPin = 16

GPIO.setmode(GPIO.BOARD)

GPIO.setup(stepSingalPin, GPIO.OUT)
GPIO.setup(stepDirPin, GPIO.OUT)
GPIO.setup(stepSleepPin, GPIO.OUT)
GPIO.setup(limitSwitchPin, GPIO.IN)
GPIO.add_event_detect(limitSwitchPin, GPIO.FALLING, callback=limitSwitch, bouncetime=100)
GPIO.setup(irlightPin, GPIO.OUT)

# Variable
alpha = [180, 0]	# [rightNowAngle, leftAngle]
irlight = 2			# 0=off 1=on 2=auto

# alphaMotorVariable
stepAngle = 0.45
stepSpeed = 0.0008	# us
# stepGeer = 0
threadSwitch = 0

# motorThread
motorThread = threading.Thread(target=motorTd,name='motorThread')

def motorTd(alphaStep):
	GPIO.output(stepSleepPin, GPIO.HIGH)
	while alphaStep >= 0 && threadSwitch >= 1:
		GPIO.output(stepSingalPin, GPIO.LOW)
		time.sleep(stepSpeed)
		GPIO.output(stepSingalPin, GPIO.HIGH)
		time.sleep(stepSpeed)
		alpha[0] += stepAngle
		alphaStep -= 1
	GPIO.output(stepSleepPin, GPIO.LOW)

def receive(rxBuff):
	buff[] = rxBuff.split()
	for i in range(0,len(buff),2):
		if threadSwitch != 2
			if buff[i] == a_rel:	# relative
				threadSwitch = 0
				alphaBuff = float(buff[i + 1])
				motorCompute(alphaBuff, 1)
			elif buff[i] == a_abs:	# absolut
				threadSwitch = 0
				alphaBuff = float(buff[i + 1]) - alpha[0]
				motorCompute(alphaBuff, 1)
			elif buff[i] == irlight:
				irlight = buff[i + 1]
			elif buff[i] == cal:
				threadSwitch = 0
				alpha[0] = 360
				motorCompute(-360, 2)

def transmit():

def motorCompute(alphaBuff, TdBuff):
	if alphaBuff + alpha[0] < 0:
		alphaBuff += 360
	elif alphaBuff + alpha[0] > 360
		alphaBuff -= 360

	if(alphaBuff > 0):
		GPIO.output(stepDirlPin, GPIO.HIGH)
	else:
		GPIO.output(stepDirlPin, GPIO.LOW)
	time.sleep(0.0017)
	threadSwitch = TdBuff
	motorThread.start(abs(int(alphaBuff/stepAngle)))


# limitSwitchStart
def limitSwitch(channel):  
    threadSwitch = 0
    while GPIO.input(limitSwitchPin) == GPIO.LOW:
    	motorCompute(1, 2)
    	time.sleep(0.01)
    alpha[0] = 0
    alpha[1] = 0
    threadSwitch = 0