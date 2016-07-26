from picamera import PiCamera, Color
from time import sleep

camera = PiCamera()

#setting camera paramaters
camera.resolution = (2592, 1944)
camera.framerate = 15
camera.vflip = True

camera.start_preview()
for i in range(5):
	camera.annotate_background = Color('white')
	camera.annotate_text = "ready to capture in 1 sec"
	sleep(1)
	camera.annotate_background = Color('red')
	camera.annotate_text = "CAPTURE!"
	camera.capture('/home/pi/Desktop/images%s.jpg' % i)


camera.resolution = (1920,1080)
camera.annotate_background = Color('green')
camera.annotate_text = "Begin Video Capture!!!"
camera.start_recording('/home/pi/Desktop/video.h264')
sleep(10)
camera.annotate_text = "ok... shutting down, good bye :)"
camera.stop_recording()
camera.stop_preview()

print 'capture complete'


