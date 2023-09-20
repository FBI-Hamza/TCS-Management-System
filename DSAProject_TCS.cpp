#include <cstring>
#include <iostream>
using namespace std;
#define V 7


// STRUCTURES
struct Order {
    int    orderId;
    string currLocation;
    string destination;
    string productType;
    int    weight;
    int    charges;
    Order* next = NULL;
    Order* prev = NULL;
};
Order* cancelledFront = NULL; // normal orderlist -> cancelled orderlist front
Order* cancelledRear  = NULL;

Order* deliveredFront = NULL; // delivered list
Order* deliveredRear  = NULL;

Order* allFront = NULL; // order List (placed orders)
Order* allRear  = NULL;


struct customer {
    string    name;
    string    gender;
    string    phoneNo;
    string    address;
    int       id;
    int       password;
    customer* right = NULL;
    customer* left  = NULL;
    int bf = 0;

    Order*  simpleOrder   = NULL;
    Order*  priorityOrder = NULL;
};
customer* root          = NULL;

Order*  simpleFront   = NULL; // simple order Linked list in customer node 
Order*  simpleRear    = NULL;

Order*  priorityFront = NULL; // priority order Linked list in customer node
Order*  priorityRear  = NULL;

//Vertex for shortest path.
struct Vertex{
    string label;
    bool visited;
    int distance;
};
struct Vertex* vertices[6] ;
int vertexCount = 0;

// VARIABLES
string locations[V] = {"ISB", "RWP", "SGD", "LHR", "FSD", "KHI", "MUL"};
bool   visited[V];
int    minDis[V];
int    edges[V][V];


int productId  = 0;
int orderKms   = 0;
int signUpId   = 1000;
int pricePerKg = 100;

int array[100];
int top  = -1;
int top2 = -1;


// METHODS:
void addVertex(string label);
int  findMinVertex();
void shortestpath (int start, int end);
void path (int pp);
void addEdge (int u, int v, int w);
void graphPath ();
int  distance ();
void chooseLocation ();

void adminBlock ();
void customerBlock ();
void adminSecurity ();
void viewCancelList ();
void showCancelledOrders ();


void    AdminCancelOrder (customer*);
customer* createCustomer ();
void    deleteCustomer ();
void    showAllOrders ();
void    cancelOrder (customer*);
void    customerDashBoard (customer*);
void    viewOrderList (customer*);
customer* signIn ();
void    overAllDisplay (customer*);
void    onlyCustomerDisplay (customer*);


void    feedbackCustomer ();
void    undeliveredOrders (customer* root);
void    enqueueSimpleOrder (customer*);
void    enqueueUrgentOrder (customer*);

int     delCustomer (int);
customer* search (int);
void    deliverOrder (customer* i);
void    insertCancel (Order* i);
void    AdminDeliverOrder (customer* r);
void    deliveredList ();

void push (int);
bool is_emptyy ();
bool is_full ();
int  pop ();


// AVL
int height (customer* node);
int balanceFac (customer* node);
void balFacALL (customer* node);

customer* llRotation (customer* node);
customer* rrRotation (customer* node);
customer* lrRotation (customer* node);
customer* rlRotation (customer* node);
customer* balancing (customer* node);

customer* insert (customer* node, customer* curr);
customer* search (customer* node, int key);
customer* deletion (customer* node, int key);

int minValue (customer* node);
bool isEmpty ();



int height (customer* node) {
    if (node == NULL) {
        return 0;
    }
    else {
        int lh = height (node->left);
        int rh = height (node->right);
        return (max(lh, rh) + 1);
    }
}


int balanceFac (customer* node) {
    return height(node->left) - height(node->right);
}


customer* llRotation (customer* node) {
    customer* temp = node -> left;
    node -> left = temp -> right;
    temp -> right = node;

    // if rotated node is root node
    if (node == root) {
        root = temp;
    }

    return temp;
}


customer* rrRotation (customer* node) {
    customer* temp = node -> right;
    node -> right = temp -> left;
    temp -> left = node;

    // if rotated node is root node
    if (node == root) {
        root = temp;
    }

    return temp;
}


customer* lrRotation (customer* node) {
    customer* temp1 = node -> left;
    customer* temp2 = temp1 -> right;

    node -> left = temp2 -> right;
    temp1 -> right = temp2 -> left;

    temp2 -> right = node;
    temp2 -> left = temp1;

    // if rotated node is root node
    if (node == root) {
        root = temp2;
    }

    return temp2;
}


customer* rlRotation (customer* node) {
    customer* temp1 = node -> right;
    customer* temp2 = temp1 -> left;

    node -> right = temp2 -> left;
    temp1 -> left = temp2 -> right;

    temp2 -> left = node;
    temp2 -> right = temp1;

    // if rotated node is root node
    if (node == root) {
        root = temp2;
    }

    return temp2;
}


int minValue (customer* node) {
    while (node -> left != NULL) {
        node = node -> left;
    }
    return node -> id;
}


customer* balancing (customer* node) {
    
    int balFac = balanceFac(node);
    
        if (balFac > 1) {
            if (balanceFac(node -> left) > 0) {
                node = llRotation(node);
            }
            else {
                node = lrRotation(node);
            }
        }
        else if (balFac < -1) {
            if (balanceFac(node -> right) > 0) {
                node = rlRotation(node);
            }
            else {
                node = rrRotation(node);
            }
        }

    return node;
}


customer* insert (customer* node, customer* curr) {

    if (node == NULL) {
        return curr;
    }
    else if (curr -> id < node -> id) {
        node -> left = insert (node -> left, curr);
        node = balancing(node);
    }
    else {
        node -> right = insert (node -> right, curr);
        node = balancing(node);
    }

    return node;
}


customer* deletion (customer* node, int key) {

    if (node == NULL) {
        return node;
    }
    else if (key < node -> id) {
        node -> left = deletion(node -> left, key);
    }
    else if (key > node -> id) {
        node -> right = deletion(node -> right, key);
    }
    else {
        // if node has further 0/1 child nodes
        if (node -> left == NULL) {
            customer* temp = node -> right;
            free(node);

            return temp;
        }
        else if (node -> right == NULL) {
            customer* temp = node -> left;
            free(node);

            return temp;
        }
        // if node has further 2 child nodes
        else {
            node -> id = minValue(node -> right);
            node -> right = deletion(node -> right, node -> id);
        }
    }
    node = balancing(node);

    return node;
}


customer* search (customer* node, int key) {
    if (node == NULL || node -> id == key) {
        return node;
    }
    else if (key < node -> id) {
        return search (node -> left, key);
    }
    else {
        return search (node -> right, key);
    }
}


void balFacALL (customer* node) {
    if (node == NULL) {

        return;
    }
    else {
        balFacALL(node -> left);
        node -> bf = balanceFac(node);
        balFacALL(node -> right);
    }
}


bool isEmpty () {
    return (root == NULL);
}




void mainMenu () 
{
    int opt;
        while (opt != 0) {
        cout << "\n\n*******************\n1) ADMIN BLOCK \n2) CUSTOMER BLOCK \n0) EXIT\n*******************\nOPTION: ";
        cin >> opt;

        switch (opt) {
            case 0: {
                cout << "\n***** PROGRAM TERMINATED *****\n" << endl; 
                exit(0);
            }

            case 1: {
                adminSecurity (); 
                break;
            }

            case 2: {
                customerBlock ();
                break;
            }

            default: {
                cout << "Invalid Option!" << endl; 
                break;
            }
        }
    }

}


int distance ()
{
    int d = 0;
    cout << "Destination: ";
    cin >> d;

    shortestpath (0, d);
    
    path (d);
    cout << "\n";
    return d;
}


void path (int pp)
{
    cout << "The shortest distance to reach " << locations[pp] << " from " << locations[0] << " is ";
    cout << vertices[pp]->distance << " km." << endl;
    orderKms = vertices[pp]->distance;
}


// int shortestpath (int start, int end)
void shortestpath (int start, int end)
{
    vertices[start] -> distance = 0;
    
    for (int i = 0; i < V; i++){
        if(i == end){
            break;
        }
        else{
        int minVertex = -1;

        for (int i = 0; i < V; i++){
        if (vertices[i] -> visited == false && (minVertex == -1 || vertices[i] -> distance < vertices[minVertex] -> distance )){
            minVertex = i;
            }
        }
        vertices[minVertex] -> visited = true;
           
           for (int j = 0; j < V; j++){
            if (vertices[j] -> visited == false && edges[minVertex][j] != 0){
                int d = vertices[minVertex] -> distance + edges[minVertex][j];
                if (d < vertices[j] -> distance){
                    vertices[j] -> distance = d;
                    
                }
            }
        }
      }
    }
    
    for (int i = 0; i < V; i++) {
        vertices[i]->visited = false;
    }
}


void addVertex(string label){
    Vertex* vertex = new Vertex;
    vertex -> label = label;
    vertex -> visited = false;
    vertex -> distance = 9999;
    vertices[vertexCount++] = vertex;
}


bool is_full () { return (top == 99); }

bool is_emptyy () { return (top == -1); }

void push (int x)
{
    if (is_full ()) {
        cout << "\nNo space";
    }
    else {
        array[++top] = x;
        cout << "THANKS FOR YOUR TIME";
    }
}


int pop ()
{
    int x;
    if (is_emptyy ()) {
        cout << "\nNothing to pop";
    }
    else {
        x = array[top--];
        return x;
    }
}



void adminSecurity ()
{
    cout << "\n                               *********************** WELCOME TO ADMIN ***********************" << endl;

    cout << "\n*******************************" << endl;
    cout << "             LOGIN" << endl;
    cout <<   "*******************************" << endl;

    string user = "";
    string pass;

    int chances = 0;
    while (chances < 3) {

        ++chances;
        if (user != "admin") {
            cout << "\nUSERNAME: ";
            cin >> user;
            cout << "PASSWORD: ";
            cin >> pass;
        }

        if (user == "admin" && pass == "12345") {
            cout << "\nACCESS GRANTED !!!" << endl;
            adminBlock ();
        }
        else if (user == "admin" && pass != "12345" && chances < 3) {
            printf("INCORRECT PASSWORD! Enter Again: ");
            cin >> pass;
        }
        else if (chances < 3) {
            printf("INCORRECT Credentials! Try Again!\n");
        }
        
    }
    printf("Too Many Wrong Attempts, Try Again Later!\n\n");
    mainMenu();
}


void addEdge (int u, int v, int w)
{
    edges[u][v]  = w;
    edges[v][u]  = w;
}


void adminBlock ()
{
    int i = -1;
    while (i != 0) {
        cout << "\n\n************************************\n 1) DISPLAY CUSTOMER RECORDS\n 2) DELETE CUSTOMER RECORDS\n 3) SHOW ALL THE ORDERS EVER PLACED\n 4) SHOW UNDELIVERED ORDERS\n 5) SHOW LOCATIONS/EDGES\n 6) CANCEL ORDERS\n 7) SHOW CANCELLED ORDERS\n 8) SHOW DELIVERED ORDERS\n 9) DELIVER ORDER \n10) CHECK FEEDBACKS\n 0) EXIT\n************************************\nOPTION: ";
        cin >> i;

        switch (i) {
            case 0: {
                mainMenu ();
                break;
            }
            case 1: {
                overAllDisplay (root);
                break;
            }
            case 2: {
                onlyCustomerDisplay (root);
                deleteCustomer ();
                break;
            }
            case 3: {
                showAllOrders ();
                break;
            }
            case 4: {
                undeliveredOrders (root);
                break;
            }
            case 5: {
                cout << "      ";
                for (int i = 0; i < V; i++) {
                    cout << locations[i] << "    ";
                }
                printf("\n     -----------------------------------------------\n");
                
                for (int i = 0; i < V; i++) {
                    cout << locations[i] << "  |";
                    for (int j = 0; j < V; j++) {
                        printf("%-4d   ", edges[i][j]);
                    }
                    printf("\n");
                }
                break;
            }
            case 6: {
                if (root == NULL) {
                    cout << "No customer record exists" << endl;
                    break;
                }
                AdminCancelOrder (root);
                break;
            }
            case 7: {
                showCancelledOrders ();
                break;
            }
            case 8: {
                deliveredList ();
                break;
            }
            case 9: {
                cout << "Following are the Pending Orders" << endl;
                AdminDeliverOrder (root);
                break;
            }
            case 10: {
                cout << "**********ALL FEEDBACKS**********" << endl;
                while (top != -1) {
                    int value = pop ();
                    if (value == 1) {
                        cout << "\nVERY GOOD\n";
                    }
                    else if (value == 2) {
                        cout << "GOOD\n";
                    }
                    else if (value == 3) {
                        cout << "AVERAGE\n";
                    }
                    else if (value == 4) {
                        cout << "POOR\n";
                    }
                    else if (value == 5) {
                        cout << "VERY POOR\n";
                    }
                }
                break;
            }
            default: {
                cout << "Invalid Option!" << endl;
                break;
            }
        }
    }
}


void chooseLocation ()
{
    cout << "\n";
    for (int gk = 0; gk < V; gk++) {
        cout << "Press\t" << gk << "\tfor\t" << locations[gk] << " \n";
    }
    cout << "\n";
}


void graphPath ()
{
    //   0      1      2      3      4      5      6
    // "ISB", "RWP", "SGD", "LHR", "FSD", "KHI", "MUL"
    addEdge (0, 1, 20);
    addEdge (0, 3, 365);
    addEdge (0, 5, 1410);
    addEdge (1, 2, 230);
    addEdge (1, 3, 340);
    addEdge (1, 4, 300);
    addEdge (1, 5, 1370);
    addEdge (1, 6, 520);
    addEdge (2, 4, 130);
    addEdge (3, 4, 181);
    addEdge (5, 6, 883);
    addEdge (6, 2, 349);
    
    for(int x = 0;x<V;x++){
    string y = locations[x];
    addVertex(y);
    }
}


void showCancelledOrders ()
{
    if (cancelledFront == NULL) {
        cout << "No orders available to display!!" << endl;
    }
    else {
        Order* p = cancelledFront;
        while (p != NULL) {
            cout << "ID: " << p -> orderId << endl;
            cout << "Product type: " << p -> productType << endl;
            cout << "Current location: " << p->currLocation << endl;
            cout << "Drop off location: " << p -> destination << endl;
            cout << "Weight: " << p -> weight << endl;
            cout << "Charges: " << p -> charges << endl;
            cout << "**************************\n" << endl;
            p = p -> next;
        }
    }
}


void showAllOrders ()
{
    Order* p = allFront;
    if (allFront == NULL) {
        cout << "No orders available to display!!" << endl;
    }
    else {
        while (p != NULL) {
            cout << "ID: " << p -> orderId << endl;
            cout << "Product type: " << p -> productType << endl;
            cout << "Current location: " << p -> currLocation << endl;
            cout << "Drop off location: " << p -> destination << endl;
            cout << "Weight: " << p -> weight << endl;
            cout << "Charges: " << p -> charges << endl;
            cout << "**************************\n" << endl;
            p = p -> next;
        }
    }
}


void customerBlock ()
{
    int i = -1;
    while (i != 0) {
        cout << "\n1) SIGN UP\n2) SIGN IN\n0) EXIT\nOPTION: ";
        cin >> i;
        cout << endl;

        switch (i) {
            case 0: {
                mainMenu ();
                break;
            }
            case 1: {
                customer* curr = createCustomer ();
                root = insert(root, curr);
                balFacALL(root);

                break;
            }
            case 2: {
                customer* f = signIn ();
                customerDashBoard (f);
                break;
            }
            default: {
                cout << "Invalid Option!" << endl;
                break;
            }
        }
    }
}


void customerDashBoard (customer* i)
{
    int o = -1;
    while (o != 0) {
        cout << "\n\n1) PLACE ORDER\n2) PLACE ORDER ON URGENT BASIS\n3) CANCEL ORDER\n4) VIEW ORDERS\n5) GIVE FEEDBACK\n6) RETURN TO CUSTOMER BLOCK\nOPTION: ";
        cin >> o;

        switch (o) {
            case 1: {
                enqueueSimpleOrder (i);
                cout << "ORDER PLACED SUCCESSFULLY!!" << endl;
                break;
            }
            case 2: {
                enqueueUrgentOrder (i);
                cout << "ORDER PLACED SUCCESSFULLY!!" << endl;
                break;
            }
            case 3: {
                cancelOrder (i);
                break;
            }
            case 4: {
                viewOrderList (i);
                break;
            }
            case 5: {
                feedbackCustomer ();
                break;
            }
            case 6: {
                customerBlock ();
            }
            default: {
                cout << "Invalid Option!" << endl;
                break;
            }
        }
    }
}


void feedbackCustomer ()
{
    int x;
    cout << "\n*******************" << endl;
    cout << "     FEEDBACK ";
    cout << "\n*******************" << endl;
    cout << "HOW WAS YOUR EXPERIENCE?" << endl;
    cout << "1) VERY GOOD\n2) GOOD\n3)AVERAGE\n4) POOR\n5) VERY POOR\n";
    cout << "Enter any option: ";
    cin >> x;
    if (x == 1) {
        push (1);
    }
    else if (x == 2) {
        push (2);
    }
    else if (x == 3) {
        push (3);
    }
    else if (x == 4) {
        push (4);
    }
    else if (x == 5) {
        push (5);
    }
}


void AdminDeliverOrder (customer* r)
{
    if (root == NULL) {
        cout << "No customer record exists" << endl;
        return;
    }

    int id;
    cout << "----------------- ALL ORDERS -----------------";
    overAllDisplay (root);
    cout << "ENTER THE ORDER ID OF THE CUSTOMER WHOSE ORDERS YOU WANT TO DELIVER: ";
    cin >> id;

    customer* customer = search (root, id);

    if (customer -> simpleOrder == NULL && customer -> priorityOrder == NULL) {
        cout << "THIS CUSTOMER HAVEN'T PLACED ANY ORDERS" << endl;
        adminBlock ();
    }
    deliverOrder (customer);
}


void AdminCancelOrder (customer* r)
{
    int id;
    cout << "\n----------------- ALL ORDERS -----------------\n";
    overAllDisplay (root);
    cout << "ENTER THE ORDER ID OF THE CUSTOMER WHOSE ORDERS YOU WANT TO CANCEL: ";
    cin >> id;
    cout << "\n";

    customer* customer = search (root, id);

    if (customer -> simpleOrder == NULL && customer -> priorityOrder == NULL) {
        cout << "\nTHIS CUSTOMER HAVEN'T PLACED ANY ORDERS!\n";
        adminBlock ();
    }
    cancelOrder (customer);
}


void onlyCustomerDisplay (customer* root)
{

    if (root != NULL) {
        cout << "\n*******************" << endl;
        cout << "     CUSTOMER ";
        cout << "\n*******************" << endl;

        cout << "Id: " << root -> id << endl;
        cout << "Name: " << root -> name << endl;
        cout << "Gender: " << root -> gender << endl;
        cout << "Phone Number: " << root -> phoneNo << endl;
        cout << "Address: " << root -> address << endl;
        onlyCustomerDisplay (root -> left);
        onlyCustomerDisplay (root -> right);
    }
}


void undeliveredOrders (customer* root)
{
    if (root != NULL) {
        Order* l = root -> priorityOrder;

        if (l == NULL) {
            cout << "\nURGERT ORDERS = NULL " << endl;
        }
        else {
            cout << "\n*******************" << endl;
            cout << "   URGENT ORDERS ";
            cout << "\n*******************" << endl;

            while (l != NULL) {

                cout << "ID: " << l -> orderId << endl;
                cout << "Product type: " << l -> productType << endl;
                cout << "Current location: " << l -> currLocation << endl;
                cout << "Drop off location: " << l -> destination << endl;
                cout << "Weight: " << l -> weight << endl;
                cout << "Charges: " << l -> charges << endl;
                cout << "**************************" << endl;

                l = l -> next;
            }
        }
        Order* l2 = root -> simpleOrder;

        if (l2 == NULL) {
            cout << "\nSIMPLE ORDERS = NULL " << endl;
        }
        else {
            cout << "\n*******************" << endl;
            cout << "   SIMPLE ORDERS";
            cout << "\n*******************" << endl;

            while (l2 != NULL) {
                cout << "ID: " << l2 -> orderId << endl;
                cout << "Product type: " << l2 -> productType << endl;
                cout << "Current location: " << l2 -> currLocation << endl;
                cout << "Drop off location: " << l2 -> destination << endl;
                cout << "Weight: " << l2 -> weight << endl;
                cout << "Charges: " << l2 -> charges << endl;

                cout << "**************************" << endl;
                l2 = l2 -> next;
            }
        }

        undeliveredOrders (root -> left);
        undeliveredOrders (root -> right);
        return;
    }
}


void overAllDisplay (customer* root)
{
    if (root != NULL) {

        overAllDisplay (root -> left);

        cout << "\n*******************" << endl;
        cout << "     CUSTOMER ";
        cout << "\n*******************" << endl;

        cout << "Id: " << root -> id << endl;
        cout << "Name: " << root -> name << endl;
        cout << "Gender: " << root -> gender << endl;
        cout << "Phone Number: " << root -> phoneNo << endl;
        cout << "Address: " << root -> address << endl;


        // printing priority orders from the 
        Order* l = root -> priorityOrder;
        if (l == NULL) {
            cout << "\nNo Urgent Orders to Display!" << endl;
        }
        else {
            cout << "\n*******************" << endl;
            cout << "   URGENT ORDERS ";
            cout << "\n*******************" << endl;

            while (l != NULL) {
                cout << "ID: " << l -> orderId << endl;
                cout << "Product type: " << l -> productType << endl;
                cout << "Current location: " << l -> currLocation << endl;
                cout << "Drop off location: " << l -> destination << endl;
                cout << "Weight: " << l -> weight << endl;
                cout << "Charges: " << l -> charges << endl;
                cout << "**************************" << endl;

                l = l -> next;
            }
        }

        Order* l2 = root -> simpleOrder;

        if (l2 == NULL) {
            cout << "\nNo Simple Orders to Display!" << endl;
        }
        else {
            cout << "\n*******************" << endl;
            cout << "   SIMPLE ORDERS";
            cout << "\n*******************" << endl;

            while (l2 != NULL) {
                cout << "ID: " << l2 -> orderId << endl;
                cout << "Product type: " << l2 -> productType << endl;
                cout << "Current location: " << l2 -> currLocation << endl;
                cout << "Drop off location: " << l2 -> destination << endl;
                cout << "Weight: " << l2 -> weight << endl;
                cout << "Charges: " << l2 -> charges << endl;

                cout << "**************************" << endl;
                l2 = l2 -> next;
            }
        }

        overAllDisplay (root -> right);
    }
}


customer* createCustomer ()
{
    signUpId++;
    string  n, gender, ph, add;
    int     pass;
    customer* newCustomer = new customer ();
    
    cout << "***** SIGNING UP *****" << endl;
    cout << "Enter name: ";
    cin >> n;
    cout << "Enter gender: ";
    cin >> gender;
    cout << "Enter phone number: ";
    cin >> ph;
    cout << "Enter address: ";
    cin >> add;
    cout << "Enter password: ";
    cin >> pass;

    cout << "\nYour Generated ID is " << signUpId << ".\nYou can use it for logging into your account." << endl;
    insertList (n, gender, ph, add, pass, signUpId);

    newCustomer -> name        = n;
    newCustomer -> gender      = gender;
    newCustomer -> phoneNo     = ph;
    newCustomer -> address     = add;
    newCustomer -> password    = pass;
    newCustomer -> id          = signUpId;

    return newCustomer;
}


customer* signIn ()
{
    customer* i = root;
    int id;
    int pass;

    cout << "ID: ";
    cin >> id;
    cout << "Password: ";
    cin >> pass;

    while (i != NULL && (i -> password != pass && i -> id != id)) {
        // p = i;
        if (id < i -> id) {
            i = i -> left;
        }
        else {
            i = i -> right;
        }
    }

    if (i == NULL) {
        printf("\nMATCH NOT FOUND!\nYou have to SIGNUP first!\n");
        customerBlock();
    }
    else if (i != NULL) {
        if (id == i -> id && pass == i -> password) {
            printf("LogIn Successful !!!\n");
            customerDashBoard (i);
        }
        else {
            printf("Invalid Credentials!\nTry Again!\n");
            signIn();
        }
        return i;
    }
}



void viewCancelList ()
{
    Order* p = cancelledFront;
    if (cancelledFront == NULL) {
        cout << "NO CANCELLED ORDERS" << endl;
    }
    while (p != NULL) {
        cout << "\nORDER ID: " << p -> orderId << endl;
        cout << "PRODUCT TYPE: " << p -> productType << endl;
        cout << "CURRENT LOCATION: " << p -> currLocation << endl;
        cout << "DROP OFF LOCATION: " << p -> destination << endl;
        cout << "WEIGHT OF ORDER:	" << p -> weight << endl;
        cout << "CHARGES OF ORDER: " << p -> charges << endl;
        p = p -> next;
    }
}


void viewOrderList (customer* i)
{
    Order* f = i -> simpleOrder;
    Order* p = i -> priorityOrder;

    cout << "\n************** URGENT ORDERS **************" << endl;
    if (p == NULL) {
        cout << "NO URGENT ORDERS" << endl;
    }
    else {
        while (p != NULL) {
            cout << "\nORDER ID: " << p -> orderId << endl;
            cout << "PRODUCT TYPE: " << p -> productType << endl;
            cout << "CURRENT LOCATION: " << p -> currLocation << endl;
            cout << "DROP OFF LOCATION: " << p -> destination << endl;
            cout << "WEIGHT OF ORDER: " << p -> weight << endl;
            cout << "CHARGES OF ORDER: " << p -> charges << endl;

            for (int l = 0; l <= 50; l++) {
                cout << "-";
            }
            p = p -> next;
        }
    }

    cout << "\n************** SIMPLE ORDERS **************" << endl;
    if (f == NULL) {
        cout << "NO SIMPLE ORDERS" << endl;
    }
    else {
        while (f != NULL) {
            cout << "\nORDER ID: " << f -> orderId << endl;
            cout << "PRODUCT TYPE: " << f -> productType << endl;
            cout << "CURRENT LOCATION: " << f -> currLocation << endl;
            cout << "DROP OFF LOCATION: " << f -> destination << endl;
            cout << "WEIGHT OF ORDER: " << f -> weight << endl;
            cout << "CHARGES OF ORDER: " << f -> charges << endl;

            for (int q = 0; q <= 50; q++) {
                cout << "-";
            }

            f = f -> next;
        }
    }
}


void enqueueSimpleOrder (customer* i)
{
    Order* newOrder  = new Order ();
    Order* new2Order = new Order ();

    string product, destination;
    string sourceNode = "ISB";
    int weight;


    cout << "\nEnter your product type: ";
    cin >> product;

    cout << "Enter weight of the product in kgs: ";
    cin >> weight;

    chooseLocation();           // print locations to select
    int tem = distance ();      // take destination as input from user

    if (tem == 0) {
        destination = "ISB";
    }
    else if (tem == 1) {
        destination = "RWP";
    }
    else if (tem == 2) {
        destination = "SGD";
    }
    else if (tem == 3) {
        destination = "LHR";
    }
    else if (tem == 4) {
        destination = "FSD";
    }
    else if (tem == 5) {
        destination = "KHI";
    }
    else if (tem == 6) {
        destination = "MUL";
    }

    int priceOfWeight = 0;
    priceOfWeight     = weight * 5;

    int charges = 0;
    charges     = (orderKms * 10) + priceOfWeight;


    // graphPath ();
    newOrder -> orderId       = ++productId;
    newOrder -> productType   = product;
    newOrder -> currLocation  = sourceNode;
    newOrder -> destination   = destination;
    newOrder -> charges       = charges;
    newOrder -> weight        = weight;

    new2Order -> orderId      = productId;
    new2Order -> productType  = product;
    new2Order -> currLocation = sourceNode;
    new2Order -> destination  = destination;
    new2Order -> charges      = charges;
    new2Order -> weight       = weight;


    // addition of the order node (linked list - order structure) in customer node (bst - customer structure)
    if (i -> simpleOrder == NULL) {
        i -> simpleOrder = newOrder;
        simpleFront = simpleRear = newOrder;
    } else {
        simpleRear -> next = newOrder;
        newOrder   -> prev = simpleRear;
        simpleRear         = newOrder;
    }

    // addition of the order in the ALL ORDERS LIST
    if (allFront == NULL) {
        allFront = allRear = new2Order;
    } else {
        allRear -> next = new2Order;
        allRear         = new2Order;
    }


    cout << "DESTINATION\t\tKMS\t\tPRICE" << endl;
    cout << destination << "\t\t\t" << orderKms << "\t\t" << charges << endl;
}


void enqueueUrgentOrder (customer* i)
{
    Order* newOrder  = new Order ();
    Order* new2Order = new Order ();

    int    weight;
    string sourceNode, destination;

    sourceNode  = "ISB";

    cout << "Enter your product type: " << endl;
    string product;
    cin >> product;

    chooseLocation();
    int tem = distance ();

    cout << "Enter weight of the product in kgs: " << endl;
    cin >> weight;
   
    int priceOfWeight = 0;
    priceOfWeight     = weight * 5;

    int charges = 0;
    charges     = (orderKms * 100) + priceOfWeight + 100;

    if (tem == 0) {
        destination = "ISB";
    }
    else if (tem == 1) {
        destination = "RWP";
    }
    else if (tem == 2) {
        destination = "SGD";
    }
    else if (tem == 3) {
        destination = "LHR";
    }
    else if (tem == 4) {
        destination = "FSD";
    }
    else if (tem == 5) {
        destination = "KHI";
    }
    else if (tem == 6) {
        destination = "MUL";
    }

    if (i -> priorityOrder == NULL) {
        i -> priorityOrder = newOrder;
        priorityFront = priorityRear = newOrder;
    }
    else {
        priorityRear -> next        = newOrder;
        newOrder -> prev = priorityRear;
        priorityRear              = newOrder;
    }


    if (allFront == NULL) {
        allFront = allRear = new2Order;
    }
    else {
        allRear -> next = new2Order;
        allRear       = new2Order;
    }

    newOrder -> currLocation  = sourceNode;
    newOrder -> destination  = destination;
    newOrder->orderId  = ++productId;
    newOrder -> productType  = product;
    newOrder -> weight  = weight;
    newOrder -> charges  = charges;
    
    new2Order -> charges = charges;
    new2Order -> weight = weight;
    new2Order -> productType = product;\
    new2Order->orderId = productId;
    new2Order -> currLocation = sourceNode;
    new2Order -> destination = destination;

    cout << "DESTINATION\t\tKMS\t\tPRICE" << endl;
    cout << destination << "\t\t\t" << orderKms << "\t\t" << charges << endl;
}


customer* search (int key)
{
    customer* p = root;
    while (p -> password != key && p != NULL) {
        if (p -> password < key) {
            p = p -> right;
        }
        else {
            p = p -> left;
        }
    }

    if (p == NULL) {
        cout << "Id Does not match";
        return p;
    }
    else if (p -> password == key) {
        return p;
    }
}


void deleteCustomer ()
{
    if (root == NULL) {
        cout << "No customer record exists" << endl;
        return;
    }

    int id;
    cout << "Enter the id which you want to delete: ";
    cin >> id;
    deletion(root, id);
}



void insertCancel (Order* i)
{
    Order* newCancel = new Order ();
    newCancel->orderId      = i->orderId;
    newCancel->currLocation = i->currLocation;
    newCancel->destination  = i->destination;
    newCancel->productType  = i->productType;
    newCancel->weight       = i->weight;
    newCancel->charges      = i->charges;
    newCancel->next         = i->next;
    newCancel->prev         = i->prev;
    
    if (cancelledFront == NULL) {
        cancelledFront = cancelledRear = newCancel;
    }
    else {
        cancelledRear -> next = newCancel;
        cancelledRear       = newCancel;
    }
}


void cancelOrder (customer* i)
{
    Order* f = i -> simpleOrder;
    Order* g = i -> priorityOrder;
    int    id;
    simpleFront = f;

    cout << "\n----------------- YOUR ORDERS -----------------" << endl;
    viewOrderList(i);

    cout << "\nENTER THE ORDER ID WHICH YOU WANT TO CANCEL: ";
    cin >> id;
    cout << "\n";

    while (f != NULL && f -> orderId != id) {
        f = f -> next;
    }

    if (f == NULL) {
        cout << "\nId not found in simple orders list\n";
    }
    else if (f -> orderId == id) {
        Order* forward  = f -> next;
        Order* previous = f -> prev;

        // if only 1 Node
        if (f -> orderId == simpleFront -> orderId && f -> next == NULL) {
            i -> simpleOrder = NULL;
            simpleFront    = NULL;
            f -> next  = NULL;
            insertCancel (f);
            delete (f);
            cout << "-------- Order # " << id << " Deleted ---------";
            return;
        }
        // if F is last Node
        if (f -> next == NULL && f -> orderId == simpleRear -> orderId) {

            previous -> next = NULL;
            f -> next        = NULL;
            insertCancel (f);
            delete (f);
            cout << "-------- Order # " << id << " Deleted ---------";
            return;
        }
        // if F is 1st Node
        else if (f -> orderId == simpleFront -> orderId) {
            i       -> simpleOrder = forward;
            forward -> prev        = i -> simpleOrder;
            f       -> next        = NULL;
            insertCancel (f);
            delete (f);
            cout << "-------- Order # " << id << " Deleted ---------";
            return;
        }
        // if F is any middle Node
        else {
            previous -> next = forward;
            forward  -> prev = previous;
            f        -> next = NULL;
            insertCancel (f);
            delete (f);
        }

        cout << "-------- Order # " << id << " Deleted ---------\n";
        return;
    }


    while (g != NULL && g -> orderId != id) {
        g = g -> next;
    }

    if (g == NULL) {
        cout << "\nId not found in urgent orders list";
    }
    else if (g -> orderId == id) {
        Order* forward  = g -> next;
        Order* previous = g -> prev;

        if (g -> orderId == priorityFront -> orderId && g -> next == NULL) {
            i -> priorityOrder = NULL;
            priorityFront      = NULL;
            g -> next          = NULL;
            insertCancel (g);
            delete (g);
            cout << "-------- Order # " << id << " Deleted ---------";
            return;
        }
        else if (g -> next == NULL && g -> orderId == priorityRear -> orderId) {
            cout << priorityRear -> orderId;
            g -> prev -> next = NULL;
            g -> next         = NULL;
            insertCancel (g);
            delete (g);
            cout << "-------- Order # " << id << " Deleted ---------";
            return;
        }
        else if (g -> orderId == priorityFront -> orderId) {
            i -> priorityOrder  = forward;
            forward -> prev = i -> priorityOrder;
            g -> next       = NULL;
            insertCancel (g);
            delete (g);
            cout << "-------- Order # " << id << " Deleted ---------";
            return;
        }
        else {
            previous -> next = forward;
            forward  -> prev = previous;
            g -> next        = NULL;
            insertCancel (g);
        }

        cout << "-------- Order # " << id << " Deleted ---------";
        return;
    }
    return;
}


void insertDeliver (Order* i)
{
    Order* newdel = new Order ();

    newdel->orderId      = i->orderId;
    newdel->currLocation = i->currLocation;
    newdel->destination  = i->destination;
    newdel->productType  = i->productType;
    newdel->weight       = i->weight;
    newdel->charges      = i->charges;
    newdel->next         = i->next;
    newdel->prev         = i->prev;

    if (deliveredFront == NULL) {
        deliveredFront = deliveredRear = newdel;
    }
    else {
        deliveredRear -> next = newdel;
        deliveredRear       = newdel;
    }
    return;
}


void deliverOrder (customer* i)
{
    Order* f = i -> simpleOrder;
    Order* g = i -> priorityOrder;
    int    id;
    simpleFront = f;

    cout << "\n----------------- YOUR ORDERS -----------------" << endl;
    viewOrderList (i);
    cout << "\nENTER THE ORDER ID WHICH YOU WANT TO DELIVER: ";
    cin >> id;

    while (f != NULL && f -> orderId != id) {
        f = f -> next;
    }

    if (f == NULL) {
        cout << "Id not found";
    }
    else if (f -> orderId == id) {
        Order* forward  = f -> next;
        Order* previous = f -> prev;

        // only 1 Node
        if (f -> orderId == simpleFront -> orderId && f -> next == NULL) {
            i -> simpleOrder = NULL;
            simpleFront    = NULL;
            f -> next  = NULL;
            insertDeliver (f);
            delete (f);
            cout << "------- Order # " << id << " Delivered ---------";
            return;
        }
        // if F is last Node
        if (f -> next == NULL && f -> orderId == simpleRear -> orderId) {
            previous -> next = NULL;
            f -> next        = NULL;
            insertDeliver (f);
            delete (f);
            cout << "------- Order # " << id << " Delivered ---------";
            return;
        }
        // if F is first Node
        else if (f -> orderId == simpleFront -> orderId) {
            i -> simpleOrder          = forward;
            forward -> prev = i -> simpleOrder;
            f -> next           = NULL;
            insertDeliver (f);
            delete (f);
            cout << "------- Order # " << id << " Delivered ---------";
            return;
        }
        else {
            previous -> next = forward;
            forward -> prev = previous;
            f -> next        = NULL;
            insertDeliver (f);
            delete (f);
            return;
        }

        cout << "------- Order # " << id << " Delivered ---------";
        return;
    }


    while (g != NULL && g -> orderId != id) {
        g = g -> next;
    }

    if (g == NULL) {
        cout << "Id not found";
    }
    else if (g -> orderId == id) {
        Order* forward  = g -> next;
        Order* previous = g -> prev;

        // only 1 Node
        if (g -> orderId == priorityFront -> orderId && g -> next == NULL) {
            i -> priorityOrder = NULL;
            priorityFront      = NULL;
            g -> next          = NULL;
            insertDeliver (g);
            delete (g);
            cout << "------- Order # " << id << " Delivered ---------";
            return;
        }
        else if (g -> next == NULL && g -> orderId == priorityRear -> orderId) {
            cout << priorityRear -> orderId;
            g -> prev -> next = NULL;
            g -> next           = NULL;
            insertDeliver (g);
            delete (g);
            cout << "------- Order # " << id << " Delivered ---------";
            return;
        }
        else if (g -> orderId == priorityFront -> orderId) {
            i -> priorityOrder  = forward;
            forward -> prev = i -> priorityOrder;
            g -> next           = NULL;
            insertDeliver (g);
            delete (g);
            cout << "------- Order # " << id << " Delivered ---------";
            return;
        }
        else {
            previous -> next = forward;
            forward -> prev = previous;
            g -> next        = NULL;
            insertDeliver (g);
            delete (g);
        }

        cout << "------- Order # " << id << " Delivered ---------";
        return;
    }
    return;
}


void deliveredList ()
{
    if (deliveredFront == NULL) {
        cout << "No orders available to display!!" << endl;
        adminBlock ();
    }
    else {
        Order* p = deliveredFront;
        while (p != NULL) {
            cout << "ID: " << p -> orderId << endl;
            cout << "Product type: " << p -> productType << endl;
            cout << "Current location: " << p -> currLocation << endl;
            cout << "Drop off location: " << p -> destination << endl;
            cout << "Weight: " << p -> weight << endl;
            cout << "Charges: " << p -> charges << endl;
            cout << "**************************\n" << endl;
            p = p -> next;
        }
    }
}


int main ()
{
    cout << "                               ****************************************************************" << endl;
    cout << "                                                W E L C O M E   T O   T C S" << endl;
    cout << "                               ****************************************************************" << endl;

    graphPath ();
    mainMenu ();
    return 0;
}