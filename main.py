from flask import Flask, request, jsonify
import random, time, uuid

app = Flask(__name__)

timers = {}

def create_timer(length = 300) -> str:
    """
    Helper function to create a timer. Starts with a default of 300 seconds.

    Parameters:
        length - int
            - the client can provide a length but the default is 300 seconds (5 minutes)

    Returns:
        timer_id - string
    """

    global timers
    timer_id = str(uuid.uuid4())
    timers[timer_id] = time.time()

    return timer_id

@app.post('/timer/set-timer')
def set_timer():
    """
    POST request from client providing a timer length

    Returns a timer ID
    """
    timer_id = create_timer()
    return jsonify({"timer id": timer_id}), 200
