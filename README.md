# timer-microservice

 A Flask based microservice for tracking timers.

 ## Features
 
 - Can set timers based on client length request, or use a default length
 - Tracks timers logically
 - Can track multiple timers at once
 - Allows client to pause, resume, and delete timers
 - Requires clients to poll the microservice for timer updates
 - Validates timer expiration during API requests
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
   Service runs on `http://localhost:8000`

2. **Run tests:**

## UML Diagram
<img width="1849" height="7080" alt="Timer Microservice" src="https://github.com/user-attachments/assets/c97e69d2-1c7c-43b8-bd22-08fcadfc0373" />




## Example Calls

**Set Timer:** `POST /timer/set-timer`
- The POST Request must be sent with a JSON body
- The JSON body must contain a duration field
- The JSON body must contain a count field

**Request:** 
```
auto response = client.Post("/timer/set-timer", "{"count":2,"duration":5}", "application/json");
```

**Response:**
```
({"timer_id": "f455ed55-7f08-3w2q-077r49e81513"})
```

**Pause Timer:** `POST /timer/<timer_id>/pause`
-The POST Request must have a timer_id in the URL

**Request:**
```
auto response = client.Post("/timer/{timer ID}/pause");
```

**Response:**
```
({"message": "Timer Paused"})
```

**Resume Timer:** `POST /timer/<timer_id>/resume`
-The POST Request must have a timer_id in the URL

**Request:**
```
auto response = client.Post("/timer/{timer ID}/resume");
```

**Response:**
```
({"message": "Timer Resumed"})
```

**Delete Timer:** `DELETE /timer/<timer_id>/delete`
-The POST Request must have a timer_id in the URL

**Request:**
```
auto response = client.Delete("/timer/{timer ID}/delete");
```

**Response:**
```
({"message": "Timer Deleted"})
```

**Get Details:** `GET /timer/<timer_id>/details`
-The GET Request must have a timer_id in the URL

**Request:**
```
auto response = client.Get("/timer/{timer ID}/details");
```

**Response:**
```
({"timer_id": "f455ed55-7f08-3w2q-077r49e81513",
  "start": 1722940867.3386405,
  "state": "active",
  "count": 0,
  "time_remaining": 4.6613595
  "end": 1722940872.0
})
```

## Files:
- `main.py`
- `timer_test.cpp`

## Return Codes

- `200` - Success
- `400` - Bad Request
- `404` - Not Found
- `409` - Conflict

