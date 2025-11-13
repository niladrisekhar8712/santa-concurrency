# 🎅 Santa Claus Concurrency Problem

This project is a C++ simulation of the classic **Santa Claus Problem**, a well-known synchronization and concurrency exercise.  
It models the behavior of Santa, nine reindeer, and ten elves using **threads** and **mutexes** to manage synchronization.

---

## 🧩 Problem Description

Santa repeatedly sleeps until wakened by either:

- **All nine reindeer**, back from their holidays, or
- **A group of three elves** needing help with toy R&D.

When awakened:

- If **reindeer** wake Santa:
    - Santa harnesses them, delivers toys, and then unharnesses them (sending them back on holiday).
- If **three elves** wake Santa:
    - Santa consults with them in his study and then sends them back to work.

Santa must **give priority to the reindeer** whenever both groups are waiting.

---

## ⚙️ Implementation Overview

This implementation uses:

- **`std::thread`** for concurrent execution.
- **`std::mutex`** for safe access to shared resources.
- Shared counters (`reindeer_count`, `elf_count`) to track how many reindeer or elves are ready.
- Simple logic prioritizing reindeer over elves in Santa’s decision flow.

Each function simulates a specific role:

| Function | Description |
|-----------|-------------|
| `reindeer_awake(int i)` | Marks a reindeer as returned from holiday. |
| `reindeer_sleep(int i)` | Puts a reindeer back on holiday after delivery. |
| `elf_awake(int i)` | Elf requests help from Santa (max 3 at once). |
| `elf_sleep(int i)` | Elf finishes consultation and returns to work. |
| `santa_task()` | Santa checks who’s waiting and responds with priority to reindeer. |
| `init()` | Initializes all state variables. |

---

## 🧠 Design Notes

- **Thread Safety:**  
  Each shared resource (elves and reindeer) has its own mutex (`m1`, `m2`) to avoid race conditions.  
  Santa uses a separate mutex (`m3`) to check the global state safely.

- **Priority Handling:**  
  Reindeer always take precedence.  
  Santa only helps elves if no reindeer are waiting.

- **Simplified Model:**  
  This version executes only once — Santa checks the current state and responds.  
  It’s designed for clarity rather than continuous looping behavior.

...
🚀 Possible Improvements
Add condition variables or semaphores for more realistic continuous Santa behavior.

Allow Santa to wake dynamically whenever elves or reindeer trigger events.

Improve output synchronization for cleaner terminal output.

📚 References
Hoare, C. A. R. “Communicating Sequential Processes”, 1978.


The Santa Claus concurrency problem is a classic synchronization exercise used in operating systems and concurrent programming education.
