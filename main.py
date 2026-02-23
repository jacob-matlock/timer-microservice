from flask import Flask, request, jsonify
import time, uuid

app = Flask(__name__)

timers = {}

def refresh_timer_state(timer):
    """
    This is a helper function to be called at the beginning of each endpoint. It checks if the timer should be set
    to inactive before any more action is taken

    Parameters:
        timer - dictionary

    Returns:
         True if timer state is changing from active to inactive
         False otherwise
    """

    starting_state = timer["state"]
    if timer["state"] == "active" and time.time() >= timer['end']:
        if "repeat" in timer and timer["repeat"] > 0: #repeat management
            timer["repeat"] -= 1
            timer["start"] = time.time()
            timer["end"] = time.time() + timer["duration"]
        else:
            timer["state"] = "inactive"
        return True #timer expired during refresh
    else:
        return False

def start_timer(length, num_repeat) -> str:
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
        "timer_id": timer_id,
        "start": time.time(),
        "duration": length,
        "end": time.time()+length,
        "state": "active"

    }

    if num_repeat > 0:
        timers[timer_id]["repeat"] = num_repeat

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


    if not data or "duration" not in data:
        return jsonify({"error": "Duration is a Required Field"}), 400

    duration = data["duration"]

    if "repeat" in data:
        repeat = data["repeat"]

    if not isinstance(duration, int) or duration <= 0:
        return jsonify({"error": "Invalid Duration"}), 400

    if not isinstance(repeat, int) or repeat < 0:
        return jsonify({"error": "Repeat Field Must be a Positive Integer"}), 400

    timer_id = start_timer(duration, repeat)

    return jsonify({"timer id": timer_id}), 200

@app.post('/timer/<timer_id>/pause')
def pause_timer(timer_id):
    """
    This is an endpoint that pauses the timer with the timer_id provided by the client. If such timer does not exist,
    is already paused, or is inactive, the appropriate codes are returned. Otherwise, the timer is paused and its state
    is changed to paused.
    """

    timer = timers.get(timer_id)

    if timer is None:
        return jsonify({"error": "No Timer Associated With That ID"}), 404

    expired = refresh_timer_state(timer)
    if expired:
        return jsonify({"error": f"Timer Already Expired At {timer['end']}"}), 409

    if timer["state"] == "paused":
        return jsonify({"error": "Already Paused"}), 409

    if timer["state"] == "inactive":
        return jsonify({"error": "Timer Inactive"}), 409

    timer["state"] = "paused"
    time_remaining = timer["end"] - time.time()
    timer["time_remaining"] = time_remaining
    return jsonify({"message": "Timer Paused"}), 200

@app.post('/timer/<timer_id>/resume')
def resume_timer(timer_id):
    """
    This is an endpoint that resumes the timer with the timer_id provided. If such timer does not exist, is already
    active, or is inactive, the appropriate codes will be returned. Otherwise, the timer will be resumed and the state
    will be changed from paused to active.
    """

    timer = timers.get(timer_id)

    if timer is None:
        return jsonify({"error": "No Timer Associated With That ID"}), 404

    expired = refresh_timer_state(timer)
    if expired:
        return jsonify({"error": f"Timer Already Expired At {timer['end']}"}), 409

    if timer["state"] == "active":
        return jsonify({"error": "Already Active"}), 409

    if timer["state"] == "inactive":
        return jsonify ({"error": "Timer Inactive"}),  409

    timer["state"] = "active"
    timer["end"] = time.time() + timer["time_remaining"]
    return jsonify({"message": "Timer Resumed"}), 200

@app.delete('/timer/<timer_id>/delete')
def delete_timer(timer_id):
    """
    This is an endpoint that deletes the timer with the timer_id provided. If such timer does not exist, the appropriate
    code is returned. Otherwise, the timer is deleted and a success code is returned.
    """

    timer = timers.get(timer_id)

    if timer is None:
        return jsonify({"error":"No Timer Associated With That ID"}), 404

    del timers[timer_id]
    return jsonify({"message": "Timer Deleted"}), 200

@app.get('/timer/<timer_id>/details')
def get_details(timer_id):
    """
    This is an endpoint that finds the timer with the timer_id provided and returns the following fields to the client:
    timer_id, start, end, state. If the timer is paused, timer_remaining will be returned in place of end. The
    appropriate codes will be returned upon success or failure.
    """

    timer = timers.get(timer_id)

    if timer is None:
        return jsonify({"error": "No Timer Associated With That ID"}), 404

    refresh_timer_state(timer)

    response = {
        "timer_id": timer["timer_id"],
        "start": timer["start"],
        "state": timer["state"]
    }

    if timer["state"] != "paused":
        response["time_remaining"] = timer["time_remaining"]
    else:
        response["end"] = timer["end"]

    return jsonify(response), 200

if __name__ == "__main__":
    app.run(port=5000)