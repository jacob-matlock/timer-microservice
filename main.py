from flask import Flask, request, jsonify
import random, time, uuid

app = Flask(__name__)

timers = {}

def start_timer(length = 300) -> str:
    """
    Helper function to start a timer. Starts with a default of 300 seconds.

    Parameters:
        length - int
            - the client can provide a length but the default is 300 seconds (5 minutes)

    Returns:
        timer_id - string
    """

    global timers
    timer_id = str(uuid.uuid4())
    timers[timer_id] = {
        "start": time.time(),
        "duration": length,
        "end": time.time()+length,
        "state": "active"
    }

    return timer_id

@app.post('/timer/set-timer')
def set_timer():
    """
    This is an endpoint that sets a timer for the client. If the client's request contains a JSON body with a duration,
    the timer will be set to that length, otherwise, a default duration is used.

    Returns a timer ID
    """

    data = request.get_json()
    duration = None

    if data is not None:
        duration = data.get("duration")

    if duration is None:
        timer_id = start_timer()
    else:
        if (isinstance(duration, int) is not int or duration <= 0):
            return jsonify({"error": "Invalid Duration"}), 400

        timer_id = start_timer(duration)

    return jsonify({"timer id": timer_id}), 200

@app.post('timer/<timer_id>/pause')
def pause_timer(timer_id):
    """
    This is an endpoint that pauses the timer with the timer_id provided by the client. If such timer does not exist,
    is already paused, or is inactive, the appropriate codes are returned. Otherwise, the timer is paused and its state
    is changed to paused.
    """

    timer = timers[timer_id]

    if timer is None:
        return jsonify("No Timer Associated With That ID", 404)

    if timer["state"] == "paused":
        return jsonify("Already Paused", 304)

    if timer["state"] == "inactive":
        return jsonify("Timer Inactive", 409)

    timer["state"] = "paused"
    time_remaining = timer["end"] - time.time()
    timer["time remaining"] = time_remaining

@app.post('timer/<timer_id>/resume')
def resume_timer(timer_id):
    """
    This is  an endpoint that resumes the timer with the timer_id provided. If such timer does not exist, is already
    active, or is inactive, the appropriate codes will be returned. Otherwise, the timer will be resumed and the state
    will be changed from paused to active.
    """