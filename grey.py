import numpy as np
import cv2

cap = cv2.VideoCapture(0)
x = 0
y = 0
X = 0
Y = 0
i = 0
count = 320*240

while(True):
    ret, frame = cap.read()
    frame.set(3,320)
    frame.set(4,240)
    X = 0
    Y = 0
    if i < 3:
        i+=1
    else:
	for X in range(0, 240):
	    for Y in range(0, 320):
	        g = frame.item(X,Y,0)
		b = frame.item(X,Y,1)
		r = frame.item(X,Y,2)     
		if r > 150 and b < 100 and g < 100:
		    x = Y + x
		    y = X + y 
        x = x/count
        y = y/count
	i = 0
	cv2.imshow('frame',frame)
	if cv2.waitKey(1) & 0xFF == ord('q'):
	    break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
