# Railway Cargo System

A simple C++ console app to manage cargo at a railway yard.

---

## What It Does

- Add, remove, and update cargo
- Search cargo by ID
- Sort by weight or priority
- Filter cargo by destination city
- View yard stats and reset everything

---

## Requirements

- Any C++ compiler (GCC, Clang, MSVC)
- C++11 or newer

---

## How to Run

```bash
g++ -std=c++11 -o railway_cargo railway_cargo.cpp
./railway_cargo
```

On Windows:
```cmd
g++ -std=c++11 -o railway_cargo.exe railway_cargo.cpp
railway_cargo.exe
```

---

## Menu Options

| Option | What it does |
|--------|--------------|
| 1. Load Cargo | Add new cargo (slot is auto-assigned) |
| 2. Dispatch Cargo | Remove cargo by ID |
| 3. Show Manifest | List all cargo |
| 4. Search Cargo | Find cargo by ID |
| 5. Update Cargo | Change destination, weight, or priority |
| 6. Sort by Weight | Lightest first |
| 7. Sort by Priority | Highest priority first |
| 8. Route Filter | Show cargo going to a city |
| 9. Yard Stats | See how many slots are used |
| 10. Reset Yard | Clear everything (asks to confirm) |

---

## Notes

- Max **50 cargo items** at a time
- Data is lost when the program closes (no file saving)
- City names are case-sensitive (`Lahore` ≠ `lahore`)

---

## Author

Faisal khan

C++ data structures project.
