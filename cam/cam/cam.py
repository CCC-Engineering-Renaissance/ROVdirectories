import cv2
import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
target = ('192.168.8.161', 5005)
target2 = ('192.168.8.161', 5006)
target3 = ('192.168.8.161', 5007)

desired_width = 640 # Example width
desired_height = 480 # Example height


cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, desired_width)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, desired_height)
cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*'MJPG'))

#cap1 = cv2.VideoCapture(4, cv2.CAP_V4L2)
#cap1.set(cv2.CAP_PROP_FRAME_WIDTH, desired_width)
#cap1.set(cv2.CAP_PROP_FRAME_HEIGHT, desired_height)
#cap1.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*'MJPG'))

#cap2 = cv2.VideoCapture(8, cv2.CAP_V4L2)
#cap2.set(cv2.CAP_PROP_FRAME_WIDTH, desired_width)
#cap2.set(cv2.CAP_PROP_FRAME_HEIGHT, desired_height)
#cap2.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*'MJPG'))


while True:
        ret, frame = cap.read()
        frame = cv2.rotate(frame, cv2.ROTATE_180)
        _, buffer = cv2.imencode('.jpg', frame, [int(cv2.IMWRITE_JPEG_QUALITY), 50])
        sock.sendto(buffer.tobytes(), target)

#        ret, frame = cap1.read()
#        frame = cv2.rotate(frame, cv2.ROTATE_180)
#        _, buffer = cv2.imencode('.jpg', frame, [int(cv2.IMWRITE_JPEG_QUALITY), 50])
#        sock.sendto(buffer.tobytes(), target2)

#        ret, frame = cap2.read()
#        frame = cv2.rotate(frame, cv2.ROTATE_90_CLOCKWISE)
#        _, buffer = cv2.imencode('.jpg', frame, [int(cv2.IMWRITE_JPEG_QUALITY), 20])
#        sock.sendto(buffer.tobytes(), target3)
