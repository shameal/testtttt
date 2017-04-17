import numpy as np
import cv2

cap = cv2.VideoCapture(0)
x = 0
y = 0
X = 0
Y = 0
count = 0

while(True):
    
    ret, frame = cap.read()

    for X in range(0, 480):
	    for Y in range(0, 640):
	  
	        g = frame.item(X,Y,0)
	        b = frame.item(X,Y,1)
	        r = frame.item(X,Y,2)
	                
	        if r > 150 and b < 100 and g < 100:
	 
	            frame.itemset((X,Y,0),255)
	            frame.itemset((X,Y,1),255)
	            frame.itemset((X,Y,2),255)
	            x = Y + x;
	            y = X + y; 
	            count+=1;
	                
	        else:

	            frame.itemset((X,Y,0),0)
	            frame.itemset((X,Y,1),0)
	            frame.itemset((X,Y,2),0)

    cv2.imshow('frame',frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()