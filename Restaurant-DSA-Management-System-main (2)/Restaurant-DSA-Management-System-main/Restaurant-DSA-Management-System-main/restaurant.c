#include <stdio.h>
#include <string.h>

#define RESTAURANT_COUNT 5
#define FOOD_COUNT 15
#define MAX_ORDERS 200

#define PENDING 0
#define PREPARING 1
#define OUT_FOR_DELIVERY 2
#define DELIVERED 3
#define CANCELLED 4

typedef struct {
    int id;
    char name[40];
    char cuisine[25];
    float rating;
    int orderCount;
} Restaurant;

typedef struct {
    int id;
    int restaurantId;
    char name[40];
    char category[20];
    float price;
    int popularity;
} Food;

typedef struct {
    int id;
    int restaurantId;
    float bill;
    int status;
    int rating; /* 0 means not rated */
} Order;

Restaurant restaurants[RESTAURANT_COUNT] = {
    {1, "Pista House", "Indian", 4.8f, 1200},
    {2, "KFC", "Italian", 4.3f, 720},
    {3, "Hyderabad House", "Indian", 4.6f, 950},
    {4, "Burger King", "Fast Food", 4.1f, 680},
    {5, "Karam Food Court", "South Indian", 4.5f, 830}
};

Food foods[FOOD_COUNT] = {
    {1, 1, "Chicken Biryani", "Main", 220.0f, 100},
    {2, 1, "Veg Biryani", "Main", 160.0f, 85},
    {3, 1, "Gulab Jamun", "Dessert", 60.0f, 70},
    {4, 2, "Margherita Pizza", "Main", 220.0f, 100},
    {5, 2, "Garlic Bread", "Starter", 100.0f, 80},
    {6, 2, "Brownie", "Dessert", 90.0f, 65},
    {7, 3, "Paneer Curry", "Main", 190.0f, 95},
    {8, 3, "Veg Biryani", "Main", 160.0f, 85},
    {9, 3, "Spring Rolls", "Starter", 120.0f, 70},
    {10, 4, "Cheese Burger", "Main", 150.0f, 100},
    {11, 4, "French Fries", "Starter", 90.0f, 75},
    {12, 4, "Chocolate Shake", "Drink", 110.0f, 60},
    {13, 5, "Masala Dosa", "Main", 90.0f, 100},
    {14, 5, "Idli", "Main", 50.0f, 80},
    {15, 5, "Filter Coffee", "Drink", 40.0f, 65}
};

Order orders[MAX_ORDERS];
int orderCount = 0;
int nextOrderId = 101;
int cart[FOOD_COUNT] = {0};
int selectedRestaurant = -1;
int cartRestaurantId = 0;

/* Function declarations */
void loadOrders(void);
void saveOrders(void);
void chooseRestaurant(void);
void restaurantMenu(void);
void showRestaurants(int mode);
void showFood(int mode);
void showCart(void);
void editCart(int removeItem);
void checkout(void);
void manageOrder(int index);
void orderHistory(void);
int cartIsEmpty(void);
float calculateBill(void);
int findRestaurant(int id);
int findOrder(int id);
const char *statusName(int status);
void clearCart(void);

int findRestaurant(int id) {
    int i;
    for (i = 0; i < RESTAURANT_COUNT; i++) {
        if (restaurants[i].id == id) {
            return i;
        }
    }
    return -1;
}

int findOrder(int id) {
    int i;
    for (i = 0; i < orderCount; i++) {
        if (orders[i].id == id) {
            return i;
        }
    }
    return -1;
}

const char *statusName(int status) {
    switch (status) {
        case PENDING: return "Pending";
        case PREPARING: return "Preparing";
        case OUT_FOR_DELIVERY: return "Out for Delivery";
        case DELIVERED: return "Delivered";
        case CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

void clearCart(void) {
    int i;
    for (i = 0; i < FOOD_COUNT; i++) {
        cart[i] = 0;
    }
    cartRestaurantId = 0;
}

void saveOrders(void) {
    FILE *fp = fopen("orders.txt", "w");
    int i;

    if (fp == NULL) {
        printf("Warning: Could not save orders.txt. Check folder permissions.\n");
        return;
    }

    for (i = 0; i < orderCount; i++) {
        fprintf(fp, "%d %d %.2f %d %d\n",
                orders[i].id,
                orders[i].restaurantId,
                orders[i].bill,
                orders[i].status,
                orders[i].rating);
    }
    fclose(fp);
}

void loadOrders(void) {
    FILE *fp = fopen("orders.txt", "r");
    Order temp;

    if (fp == NULL) {
        /* File will be created the first time orders are saved. */
        return;
    }

    while (orderCount < MAX_ORDERS &&
           fscanf(fp, "%d %d %f %d %d",
                  &temp.id, &temp.restaurantId, &temp.bill,
                  &temp.status, &temp.rating) == 5) {
        if (findRestaurant(temp.restaurantId) == -1 ||
            temp.status < PENDING || temp.status > CANCELLED ||
            temp.rating < 0 || temp.rating > 5) {
            continue;
        }

        orders[orderCount++] = temp;
        if (temp.id >= nextOrderId) {
            nextOrderId = temp.id + 1;
        }
    }
    fclose(fp);
}

void showRestaurants(int mode) {
    int index[RESTAURANT_COUNT];
    int i, j, temp, shouldSwap;

    for (i = 0; i < RESTAURANT_COUNT; i++) {
        index[i] = i;
    }

    /* Bubble sort the index list; the original restaurant data stays unchanged. */
    if (mode == 1 || mode == 2) {
        for (i = 0; i < RESTAURANT_COUNT - 1; i++) {
            for (j = 0; j < RESTAURANT_COUNT - i - 1; j++) {
                shouldSwap = 0;
                if (mode == 1 &&
                    restaurants[index[j]].orderCount <
                    restaurants[index[j + 1]].orderCount) {
                    shouldSwap = 1;
                } else if (mode == 2 &&
                           restaurants[index[j]].rating <
                           restaurants[index[j + 1]].rating) {
                    shouldSwap = 1;
                }

                if (shouldSwap) {
                    temp = index[j];
                    index[j] = index[j + 1];
                    index[j + 1] = temp;
                }
            }
        }
    }

    printf("\n%-4s %-22s %-16s %-8s %s\n",
           "ID", "Restaurant", "Cuisine", "Rating", "Orders");
    printf("---------------------------------------------------------------------\n");

    for (i = 0; i < RESTAURANT_COUNT; i++) {
        Restaurant *p = &restaurants[index[i]];
        printf("%-4d %-22s %-16s %-8.1f %d\n",
               p->id, p->name, p->cuisine, p->rating, p->orderCount);
    }
}

void chooseRestaurant(void) {
    int choice, id, i, found;
    char text[40];

    while (1) {
        printf("\n===== CHOOSE A RESTAURANT =====\n");
        printf("1. Highly Ordered Restaurants\n");
        printf("2. Search Restaurant\n");
        printf("3. Top Rated Restaurants\n");
        printf("4. Restaurants by Cuisine\n");
        printf("5. View All Restaurants\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Restart the program and enter numbers for menu choices.\n");
            return;
        }

        switch (choice) {
            case 1:
                showRestaurants(1);
                break;
            case 2:
                printf("Enter restaurant name or part of its name: ");
                scanf(" %39[^\n]", text);
                found = 0;
                for (i = 0; i < RESTAURANT_COUNT; i++) {
                    if (strstr(restaurants[i].name, text) != NULL) {
                        printf("%d. %s | %s | Rating %.1f\n",
                               restaurants[i].id, restaurants[i].name,
                               restaurants[i].cuisine, restaurants[i].rating);
                        found = 1;
                    }
                }
                if (!found) {
                    printf("Restaurant not found. Search is case-sensitive.\n");
                }
                break;
            case 3:
                showRestaurants(2);
                break;
            case 4:
                printf("Enter cuisine exactly (Indian, Italian, Fast Food, South Indian): ");
                scanf(" %39[^\n]", text);
                found = 0;
                for (i = 0; i < RESTAURANT_COUNT; i++) {
                    if (strcmp(restaurants[i].cuisine, text) == 0) {
                        printf("%d. %s | Rating %.1f\n",
                               restaurants[i].id, restaurants[i].name,
                               restaurants[i].rating);
                        found = 1;
                    }
                }
                if (!found) {
                    printf("No restaurants found for that cuisine.\n");
                }
                break;
            case 5:
                showRestaurants(0);
                break;
            case 0:
                return;
            default:
                printf("Invalid choice. Enter 0 to 5.\n");
                continue;
        }

        printf("\nEnter restaurant ID to select, or 0 to browse again: ");
        if (scanf("%d", &id) != 1) {
            printf("Invalid input.\n");
            return;
        }

        if (id == 0) {
            continue;
        }

        selectedRestaurant = findRestaurant(id);
        if (selectedRestaurant != -1) {
            printf("Selected: %s\n", restaurants[selectedRestaurant].name);
            return;
        }
        printf("Invalid restaurant ID.\n");
    }
}

void showFood(int mode) {
    int index[FOOD_COUNT];
    int n = 0, i, j, temp;

    if (selectedRestaurant < 0 || selectedRestaurant >= RESTAURANT_COUNT) {
        printf("Please select a restaurant first.\n");
        return;
    }

    for (i = 0; i < FOOD_COUNT; i++) {
        if (foods[i].restaurantId == restaurants[selectedRestaurant].id) {
            index[n++] = i;
        }
    }

    if (mode == 1) {
        for (i = 0; i < n - 1; i++) {
            for (j = 0; j < n - i - 1; j++) {
                if (foods[index[j]].popularity < foods[index[j + 1]].popularity) {
                    temp = index[j];
                    index[j] = index[j + 1];
                    index[j + 1] = temp;
                }
            }
        }
    }

    printf("\n%-4s %-24s %-12s %s\n", "ID", "Food", "Category", "Price");
    printf("---------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        Food *p = &foods[index[i]];
        printf("%-4d %-24s %-12s Rs. %.2f\n",
               p->id, p->name, p->category, p->price);
    }
}

int cartIsEmpty(void) {
    int i;
    for (i = 0; i < FOOD_COUNT; i++) {
        if (cart[i] > 0) {
            return 0;
        }
    }
    return 1;
}

float calculateBill(void) {
    int i;
    float total = 0.0f;
    for (i = 0; i < FOOD_COUNT; i++) {
        total += cart[i] * foods[i].price;
    }
    return total;
}

void showCart(void) {
    int i, found = 0;

    printf("\n===== YOUR CART =====\n");
    for (i = 0; i < FOOD_COUNT; i++) {
        if (cart[i] > 0) {
            printf("%-24s x %d = Rs. %.2f\n",
                   foods[i].name, cart[i], foods[i].price * cart[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("Your cart is empty.\n");
    } else {
        printf("Subtotal: Rs. %.2f\n", calculateBill());
    }
}

void editCart(int removeItem) {
    int id, qty, i, found = 0;
    int currentRestaurantId;

    showFood(0);
    printf("Enter food ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        return;
    }

    for (i = 0; i < FOOD_COUNT; i++) {
        if (foods[i].id == id &&
            foods[i].restaurantId == restaurants[selectedRestaurant].id) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Invalid food ID for this restaurant.\n");
        return;
    }

    currentRestaurantId = restaurants[selectedRestaurant].id;

    if (!removeItem && !cartIsEmpty() && cartRestaurantId != currentRestaurantId) {
        printf("Your cart already contains food from another restaurant.\n");
        printf("Remove the existing items or finish that order first.\n");
        return;
    }

    if (removeItem) {
        if (cart[i] == 0) {
            printf("This food is not in your cart.\n");
            return;
        }

        printf("Quantity to remove (1 to %d): ", cart[i]);
        if (scanf("%d", &qty) != 1 || qty <= 0 || qty > cart[i]) {
            printf("Invalid quantity.\n");
            return;
        }

        cart[i] -= qty;
        if (cartIsEmpty()) {
            cartRestaurantId = 0;
        }
        printf("Cart updated.\n");
    } else {
        printf("Quantity to add: ");
        if (scanf("%d", &qty) != 1 || qty <= 0 || cart[i] + qty > 100) {
            printf("Invalid quantity. Maximum quantity per food is 100.\n");
            return;
        }

        if (cartIsEmpty()) {
            cartRestaurantId = currentRestaurantId;
        }
        cart[i] += qty;
        printf("%s added to cart.\n", foods[i].name);
    }
}

void orderHistory(void) {
    int i, restaurantIndex;

    printf("\n===== ORDER HISTORY =====\n");
    if (orderCount == 0) {
        printf("No orders yet.\n");
        return;
    }

    for (i = 0; i < orderCount; i++) {
        restaurantIndex = findRestaurant(orders[i].restaurantId);
        printf("ID: %d | Restaurant: %s | Bill: Rs. %.2f | Status: %s | Rating: %d/5\n",
               orders[i].id,
               restaurantIndex >= 0 ? restaurants[restaurantIndex].name : "Unknown",
               orders[i].bill, statusName(orders[i].status), orders[i].rating);
    }
}

void checkout(void) {
    int couponChoice, confirm, i;
    char code[20];
    float subtotal, discount = 0.0f, delivery = 30.0f;

    if (cartIsEmpty()) {
        printf("Your cart is empty. Add food first.\n");
        return;
    }

    if (selectedRestaurant < 0 || selectedRestaurant >= RESTAURANT_COUNT ||
        cartRestaurantId != restaurants[selectedRestaurant].id) {
        printf("Your cart belongs to a different restaurant.\n");
        printf("Choose the restaurant matching your cart, or remove its items first.\n");
        return;
    }

    if (orderCount >= MAX_ORDERS) {
        printf("Order storage is full. Cannot accept more orders.\n");
        return;
    }

    subtotal = calculateBill();
    printf("\n===== CHECKOUT =====\n");
    showCart();

    printf("\nAvailable coupons:\n");
    printf("BITE10  - 10%% off on bills of Rs. 100 or more (maximum discount Rs. 100)\n");
    printf("FEAST50 - Rs. 50 off on bills of Rs. 300 or more\n");
    printf("1. Apply coupon\n");
    printf("2. Continue without coupon\n");
    printf("Enter choice: ");

    if (scanf("%d", &couponChoice) != 1) {
        printf("Invalid input. Checkout stopped; cart preserved.\n");
        return;
    }

    if (couponChoice == 1) {
        printf("Enter coupon code: ");
        scanf("%19s", code);

        if (strcmp(code, "BITE10") == 0 && subtotal >= 100.0f) {
            discount = subtotal * 0.10f;
            if (discount > 100.0f) {
                discount = 100.0f;
            }
            printf("BITE10 applied successfully.\n");
        } else if (strcmp(code, "FEAST50") == 0 && subtotal >= 300.0f) {
            discount = 50.0f;
            printf("FEAST50 applied successfully.\n");
        } else {
            printf("Coupon invalid or minimum bill not met. No discount applied.\n");
        }
    } else if (couponChoice != 2) {
        printf("Invalid coupon choice. No discount applied.\n");
    }

    printf("\nSubtotal: Rs. %.2f\n", subtotal);
    printf("Discount: Rs. %.2f\n", discount);
    printf("Delivery fee: Rs. %.2f\n", delivery);
    printf("Final bill: Rs. %.2f\n", subtotal - discount + delivery);

    printf("1. Confirm Order\n");
    printf("0. Cancel Checkout\n");
    printf("Enter choice: ");
    if (scanf("%d", &confirm) != 1 || confirm != 1) {
        printf("Checkout cancelled. Your cart is preserved.\n");
        return;
    }

    orders[orderCount].id = nextOrderId++;
    orders[orderCount].restaurantId = restaurants[selectedRestaurant].id;
    orders[orderCount].bill = subtotal - discount + delivery;
    orders[orderCount].status = PENDING;
    orders[orderCount].rating = 0;

    printf("\nORDER PLACED SUCCESSFULLY!\n");
    printf("Order ID: %d\n", orders[orderCount].id);
    printf("Restaurant: %s\n", restaurants[selectedRestaurant].name);
    printf("Final bill: Rs. %.2f\n", orders[orderCount].bill);

    restaurants[selectedRestaurant].orderCount++;
    orderCount++;
    saveOrders();

    for (i = 0; i < FOOD_COUNT; i++) {
        cart[i] = 0;
    }
    cartRestaurantId = 0;

    manageOrder(orderCount - 1);
}

void manageOrder(int index) {
    int choice, rating;
    Order *o;

    if (index < 0 || index >= orderCount) {
        printf("Invalid order.\n");
        return;
    }
    o = &orders[index];

    while (1) {
        printf("\n===== ORDER MANAGEMENT =====\n");
        printf("Order ID: %d | Status: %s\n", o->id, statusName(o->status));
        printf("1. Track Order Status\n");
        printf("2. Cancel Order\n");
        printf("3. Update to Preparing\n");
        printf("4. Update to Out for Delivery\n");
        printf("5. Confirm Delivered (Simulation)\n");
        printf("6. Rate Restaurant\n");
        printf("7. View Order Summary\n");
        printf("0. Return to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return;
        }

        switch (choice) {
            case 1:
                printf("Order status: %s\n", statusName(o->status));
                break;

            case 2:
                if (o->status == PENDING || o->status == PREPARING) {
                    o->status = CANCELLED;
                    saveOrders();
                    printf("Order cancelled successfully.\n");
                } else {
                    printf("Cancellation is not allowed at this stage.\n");
                }
                break;

            case 3:
                if (o->status == PENDING) {
                    o->status = PREPARING;
                    saveOrders();
                    printf("Order is now Preparing.\n");
                } else {
                    printf("Only a Pending order can move to Preparing.\n");
                }
                break;

            case 4:
                if (o->status == PREPARING) {
                    o->status = OUT_FOR_DELIVERY;
                    saveOrders();
                    printf("Order is Out for Delivery.\n");
                } else {
                    printf("Order must be Preparing first.\n");
                }
                break;

            case 5:
                if (o->status == OUT_FOR_DELIVERY) {
                    o->status = DELIVERED;
                    saveOrders();
                    printf("Order Delivered Successfully! (Simulation)\n");
                } else if (o->status == DELIVERED) {
                    printf("Order is already delivered.\n");
                } else {
                    printf("Order must be Out for Delivery first.\n");
                }
                break;

            case 6:
                if (o->status != DELIVERED) {
                    printf("You can rate only after delivery.\n");
                } else if (o->rating != 0) {
                    printf("This order has already been rated.\n");
                } else {
                    printf("Enter rating (1 to 5): ");
                    if (scanf("%d", &rating) != 1 || rating < 1 || rating > 5) {
                        printf("Rating must be between 1 and 5.\n");
                    } else {
                        o->rating = rating;
                        saveOrders();
                        printf("Thank you for your rating!\n");
                    }
                }
                break;

            case 7: {
                int rIndex = findRestaurant(o->restaurantId);
                printf("Order ID: %d\n", o->id);
                printf("Restaurant: %s\n",
                       rIndex >= 0 ? restaurants[rIndex].name : "Unknown");
                printf("Bill: Rs. %.2f\n", o->bill);
                printf("Status: %s\n", statusName(o->status));
                printf("Rating: %d/5 (0 means not rated)\n", o->rating);
                break;
            }

            case 0:
                return;

            default:
                printf("Invalid choice. Enter 0 to 7.\n");
        }
    }
}

void restaurantMenu(void) {
    int choice, i, found;
    char text[40];

    while (1) {
        printf("\n===== %s =====\n", restaurants[selectedRestaurant].name);
        printf("1. View Full Menu\n");
        printf("2. Highly Ordered Food\n");
        printf("3. Search Food\n");
        printf("4. Filter Food by Category\n");
        printf("5. View Restaurant Details\n");
        printf("6. Add Food to Cart\n");
        printf("7. View Cart\n");
        printf("8. Remove Food from Cart\n");
        printf("9. Proceed to Checkout\n");
        printf("0. Choose Another Restaurant\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return;
        }

        switch (choice) {
            case 1:
                showFood(0);
                break;

            case 2:
                showFood(1);
                break;

            case 3:
                printf("Enter food name or part of name: ");
                scanf(" %39[^\n]", text);
                found = 0;
                for (i = 0; i < FOOD_COUNT; i++) {
                    if (foods[i].restaurantId == restaurants[selectedRestaurant].id &&
                        strstr(foods[i].name, text) != NULL) {
                        printf("%d. %s - Rs. %.2f\n",
                               foods[i].id, foods[i].name, foods[i].price);
                        found = 1;
                    }
                }
                if (!found) {
                    printf("Food not found. Search is case-sensitive.\n");
                }
                break;

            case 4:
                printf("Category (Main, Starter, Dessert, Drink): ");
                scanf(" %39[^\n]", text);
                found = 0;
                for (i = 0; i < FOOD_COUNT; i++) {
                    if (foods[i].restaurantId == restaurants[selectedRestaurant].id &&
                        strcmp(foods[i].category, text) == 0) {
                        printf("%d. %s - Rs. %.2f\n",
                               foods[i].id, foods[i].name, foods[i].price);
                        found = 1;
                    }
                }
                if (!found) {
                    printf("No food found in that category.\n");
                }
                break;

            case 5:
                printf("Name: %s\n", restaurants[selectedRestaurant].name);
                printf("Cuisine: %s\n", restaurants[selectedRestaurant].cuisine);
                printf("Rating: %.1f/5\n", restaurants[selectedRestaurant].rating);
                printf("Sample order count: %d\n",
                       restaurants[selectedRestaurant].orderCount);
                break;

            case 6:
                editCart(0);
                break;

            case 7:
                showCart();
                break;

            case 8:
                editCart(1);
                break;

            case 9:
                checkout();
                if (cartIsEmpty()) {
                    return;
                }
                break;

            case 0:
                return;

            default:
                printf("Invalid choice. Enter 0 to 9.\n");
        }
    }
}

int main(void) {
    int choice, id, index;

    loadOrders();

    while (1) {
        printf("\n========== BITERUSH ==========\n");
        printf("1. Choose Restaurant and Order Food\n");
        printf("2. View All Restaurants\n");
        printf("3. Order History\n");
        printf("4. Manage an Existing Order\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Restart the program and enter menu numbers.\n");
            return 1;
        }

        switch (choice) {
            case 1:
                chooseRestaurant();
                if (selectedRestaurant != -1) {
                    restaurantMenu();
                    selectedRestaurant = -1;
                }
                break;

            case 2:
                showRestaurants(0);
                break;

            case 3:
                orderHistory();
                break;

            case 4:
                printf("Enter order ID: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid order ID.\n");
                    return 1;
                }
                index = findOrder(id);
                if (index == -1) {
                    printf("Order not found.\n");
                } else {
                    manageOrder(index);
                }
                break;

            case 5:
                printf("Thank you for using BiteRush!\n");
                saveOrders();
                return 0;

            default:
                printf("Invalid choice. Enter 1 to 5.\n");
        }
    }
}
