from flask import Flask, render_template, session, request
from flask_socketio import SocketIO, emit
 
app = Flask(__name__)
app.config['SECRET_KEY'] = 'TCC_UNISAL'
socketio = SocketIO(app)

@app.route('/')
def index(): 
    return render_template("index.html")

@app.errorhandler(404)
def page_not_found(e):
    return render_template("404.html"), 404 


if __name__ == '__main__':
    app.run(app, debug=True)
