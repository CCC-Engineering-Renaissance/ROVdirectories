import cv2
import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
target = ('192.168.8.161', 5006)

cap = cv2.VideoCapture(4)
while True:
        ret, frame = cap.read()
        frame = cv2.resize(frame, (600, 600))
        _, buffer = cv2.imencode('.jpg', frame, [int(cv2.IMWRITE_JPEG_QUALITY), 40])
        sock.sendto(buffer.tobytes(), target)
