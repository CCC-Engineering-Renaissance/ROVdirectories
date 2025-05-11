import cv2
from flask import Flask, render_template, Response

for i in range(42):
	try:
		cam = cv2.VideoCapture(i, cv2.CAP_V4L2)
	except:
		pass
app = Flask(__name__)

def gen_frames():
    while True:
        thing = cam.read()[1]
        ret, buffer = cv2.imencode('.jpg', thing)
        frame = buffer.tobytes()

        yield(b'--frame\r\n'b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/stream')
def stream():
    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/')
def index():
    return render_template('index.html')

if __name__ == "__main__":
	print("hi")
    #app.run(host="0.0.0.0",port=81,debug=True, use_reloader=False)
