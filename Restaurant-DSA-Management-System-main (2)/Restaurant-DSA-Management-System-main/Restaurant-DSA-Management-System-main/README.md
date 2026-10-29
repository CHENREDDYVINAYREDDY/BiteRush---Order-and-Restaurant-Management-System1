# BiteRush- Order and Restaurant Management System

A console-based restaurant management application written in C (C11) to demonstrate Data Structures and Algorithms.

## Features

- Display restaurant menu
- Search food items using linear search
- Sort food items by price using bubble sort
- Place customer orders and calculate bills
- Manage pending orders using a FIFO queue
- Keep completed order history in a linked list
- Save completed orders to `orders.txt` and reload them at startup
- Validate numeric input

## Files

- `restaurant.c` — complete source code
- `README.md` — project documentation
- `.gitignore` — excludes compiled binaries
- `orders.txt` — created automatically when completed orders are saved

## Requirements

A C compiler supporting the C11 standard, such as GCC.

## Compile

Open a terminal in this folder and run:

```bash
gcc -std=c11 -Wall -Wextra -pedantic restaurant.c -o restaurant
```

## Run

Linux/macOS:

```bash
./restaurant
```

Windows:

```powershell
.\restaurant.exe
```

## DSA Concepts

| Feature | Concept | Complexity |
|---|---|---|
| Find a menu item | Linear search | O(n) |
| Sort menu | Bubble sort | O(n^2) |
| Enqueue order | Queue | O(1) |
| Dequeue order | Queue | O(1) |
| Append history | Linked list | O(n) |
| Display history | Linked list traversal | O(n) |

## Notes

Pending orders are kept in memory and are lost when the program closes. Completed orders are saved in `orders.txt`. This is an educational project, not a production payment system.

## GitHub

Create a repository, then run:

```bash
git init
git add restaurant.c README.md .gitignore
git commit -m "Add Restaurant DSA Management System in C"
git branch -M main
git remote add origin https://github.com/YOUR-USERNAME/Restaurant-DSA-Project.git
git push -u origin main
```

Replace `YOUR-USERNAME` with your GitHub username.
