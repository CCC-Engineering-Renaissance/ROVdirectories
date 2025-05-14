import cv2
import threading
from flask import Flask, render_template, Response

cam = cv2.VideoCapture(-1, cv2.CAP_V4L2)
app = Flask(__name__)
	
frame = None
def test():
	global frame
	while True:
		thing = cam.read()[1]
		ret, buffer = cv2.imencode('.jpg', thing)
		frame = buffer.tobytes()

def gen_frames():
	while True:
		print(frame)
		yield(b'--frame\r\n'b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/stream')
def stream():
    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/')
def findex():
    return render_template('index.html')

if __name__ == "__main__":
	t = threading.Thread(target=test)
	t.start()
	app.run(host="0.0.0.0",port=81,debug=True, use_reloader=False)
