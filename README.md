# timer-microservice

 A Flask based microservice for tracking timers.

 ## Features

 - Can set timers based on client length request, or use a default length
 - Can track multiple timers at once
 - Allows client to pause, resume, and delete timers
 - RESTFUL API interface

## Requirements

```bash
pip install flask
```

## Quick Start

1. **Start the  service:**
   ```bash
   python main.py
   ```
   Service runs on `http://localhost:5000`

2. **Run tests:**

## UML Diagram

## Example Calls

**Set Timer:** `POST \timer


**

This microservice tracks timers logically by validating expiration during API requests rather than running continuous real-time timer processes.
The microservice does not run a persistent background process to actively track timers. Clients are expected to poll the microservice to retrieve timer updates.

