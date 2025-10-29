# cap = cv2.VideoCapture(2)
# cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
# cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
# while True:
    # _, frame = cap.read()
	# hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
	# height, width, _ = frame.shape
    # cx = int(width / 2)
    # cy = int(height / 2)
    # Pick pixel value
    # pixel_center = hsv_frame[cy, cx]
    # hue_value = pixel_center[0]


# import cv2
# import numpy as np

# def nop(x):
    # pass
# cv2.namedWindow("frame")
# cv2.createTrackbar("h", "frame", 0,179, nop)
# cv2.createTrackbar("s", "frame", 255,255, nop)
# cv2.createTrackbar("v", "frame", 255,255, nop)

# while True:
    # h = cv2.getTrackbarPos("h", "frame")
    # s = cv2.getTrackbarPos("s", "frame")
    # v = cv2.getTrackbarPos("v", "frame")

    # img_hsv = np.full((480, 640, 3), (h, s, v), dtype=np.uint8)
    # img = cv2.cvtColor(img_hsv, cv2.COLOR_HSV2BGR)

    # cv2.imshow("frame", img)
    # if cv2.waitKey(1)==27:
        # break

# cv2.destroyAllWindows()



import cv2
import numpy as np

# Load image
img = cv2.imread("shapes_and_colors.png")
img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

#  Choose target hue (for example: red = 0, green = 60, blue = 120)
target_hue = 5  # green

# Define hue tolerance (range)
lower = np.array([7, 100, 100])
upper = np.array([20, 255, 255])
print(lower)
print(upper)

#  Create mask based on hue range
mask = cv2.inRange(img_hsv, lower, upper)

#  Apply mask 
result = cv2.bitwise_and(img, img, mask=mask)

# Show results
cv2.imshow("original", img)
cv2.imshow("mask", mask)
cv2.imshow("filtered", result)
cv2.waitKey(0)
cv2.destroyAllWindows()
