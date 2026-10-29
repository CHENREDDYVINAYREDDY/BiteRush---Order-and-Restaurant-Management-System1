# BiteRush---Order-and-Restaurant-Management-System1
A menu-driven C project that simulates a restaurant ordering application.
## Features
- Highly ordered restaurants and top-rated restaurants
- Search restaurants and filter by cuisine
- View menu, popular food, search food, and filter by category
- Add items to cart, set quantities, and remove items
- Apply sample coupons: BITE10 and FEAST50
- Place orders and calculate final bills
- Cancel eligible orders
- Track order status through a simulated delivery process
- Rate an order after delivery
- Save order history in orders.txt

## Requirements
- GCC compiler (MinGW-w64 on Windows is suitable)
- Visual Studio Code (optional, but recommended)

## Compile and run on Windows PowerShell
Open a terminal in this folder and run:

powershell
gcc restaurant.c -o restaurant.exe
.\restaurant.exe


## Files
- restaurant.c — complete C source code
- orders.txt — saved order history; initially empty
- .gitignore — ignores the compiled executable
- README.md — project documentation

## DSA concepts used
- Arrays and structures to store restaurants, food, cart items, and orders
- Bubble sort to rank restaurants and popular food
- Linear search for restaurants, food, and order IDs
- File handling to preserve order history

## Notes
This is an educational simulation. Restaurant information, ratings, popularity, coupons, and delivery status are sample data. It does not connect to the real Zomato service or provide live delivery tracking.
