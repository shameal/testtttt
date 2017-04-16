import numpy as np
import cv2
class main():
   while(True):
      cameraFrames = cv2.VideoCapture(0)
      ret, frame = cameraFrames.read()
   	  #frame = cv2.threshold(frame,140,255,cv2.THRESH_BINARY)
      cv2.imshow('frame',frame)
      if cv2.waitKey(30) & 0xFF == ord('q'):
         break
		
			