import numpy as np
import cv2

cap = cv2.VideoCapture(0)
x = 0
y = 0
X = 0
Y = 0
i = 0
count = 32*240

while(True):
    ret, frame = cap.read()
    X = 0
    Y = 0
    if i < 3:
        i+=1
    else:
	for X in range(100, 380):
	    for Y in range(100, 540):
	        g = frame.item(X,Y,0)
		b = frame.item(X,Y,1)
		r = frame.item(X,Y,2)     
		if r > 140 and b < 100 and g < 100:
		    x = Y + x
		    y = X + y 
        x = x/count
        y = y/count
	i = 0
	print(x, y)
	#cv2.imshow('frame',frame)
	#if cv2.waitKey(1) & 0xFF == ord('q'):
	#    break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
