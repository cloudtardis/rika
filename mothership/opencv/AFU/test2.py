import numpy
import cv2

rL = (-10,100,100)
rU = (10,255,255)
bL = (110,50,50)
bU = (130,255,255)
gL = (50,100,100)
gU = (70,255,255)
wL = (0,0,200)
wU = (179,255,255)

im = cv2.imread("Test\\500.jpg")

hsv = cv2.cvtColor(im, cv2.COLOR_BGR2HSV)
mask = cv2.inRange(hsv, rL, rU) + cv2.inRange(hsv, bL, bU) + cv2.inRange(hsv, wL, wU)
mask = cv2.erode(mask, None, iterations=1)
mask = cv2.dilate(mask, None, iterations=1)
res = cv2.bitwise_and(im, im, mask= mask)

cv2.imshow("QQ",res)
cv2.imwrite("QQ2.jpg",res)
cv2.waitKey(0)