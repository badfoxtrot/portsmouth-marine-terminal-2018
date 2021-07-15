/// Brian Julien - CS 150 Summer 2018

class ship
{
public:
    ship() {
        next = NULL;
        cargo = NULL;
        name = "";
        idNum = 0;
        dest = 'z';
        docked = false;
        status = 'W';
        cargoNum = 0;
        delay = 0;
    }
    ship(string n, int i) {
        next = NULL;
        cargo = NULL;
        name = n;
        idNum = i;
        dest = 'P';
        docked = false;
    }
    ~ship() {
        while (cargo != NULL) {
            kontainer * tempKont;
            tempKont = cargo;
            cargo = cargo->getNext();
            delete tempKont;
        }
        cout << "~Ship " << idNum << " sails away." << endl;
    }

    void setName(string n) {name = n;}
    void setID(int i) {idNum = i;}
    void setDest(char d) {dest = d;}
    void setAllDest(kontainer * k) {
        kontainer * kptr;
        char tempD;
        char rangeL, rangeR;
        tempD = kptr->getDest();
        tempD--;
        destL = tempD;
        tempD = tempD + 2;
        destR = tempD;
    }

    void setDock(bool d) {docked = d;}
    void setStatus(char s) {status = s;}
    void setNext(ship * n) {next = n;}
    void startDelay() {delay = 0;}
    void addCargoNum() {cargoNum++;}
    void minCargoNum() {cargoNum--; if (cargoNum < 0) {cargoNum = 0;}}

    string getName() {return name;}
    int getID() {return idNum;}
    int getCargoNum() {return cargoNum;}
    char getDest() {return dest;}
    bool getDock() {return docked;}
    char getStatus() {return status;}
    int getDelay() {return delay;}
    kontainer * getCargo() {return cargo;}
    ship * getNext() {return next;}

    void genID(default_random_engine * gptr) {
        uniform_int_distribution<int> shipDist(1,500);
        idNum = shipDist(*gptr);
    }

    void genCargo(default_random_engine * gptr) {
        kontainer * kptr = NULL;
        for (int i=0; i < 10; i++){
            if (cargo == NULL){
                cargo = new kontainer;
                addCargoNum();
                cargo->setID(1000+i);
                cargo->genOrigDest(gptr);
                kptr = cargo;
            }
            else {
                kptr = new kontainer;
                addCargoNum();
                kptr->setID(1000+i);
                kptr->genOrigDest(gptr);
                kptr->setNext(cargo);
                cargo = kptr;
            }
        }
    }

    void workday() {delay++; if (delay > 5) {delay = 5;}}

    void addCargo(kontainer * k) {
        if (cargo == NULL) {dest = k->getDest(); cargo = k;}
        else {k->setNext(cargo); cargo = k;}
        addCargoNum();
    }

    void addDestCargo(kontainer * k) {
        kontainer * firstKont;
        firstKont = cargo;
        firstKont->setNext(NULL);
        cargo = cargo->getNext();
        dest = firstKont->getDest();
        dest--;
        destL = dest;
        dest = dest + 2;
        destR = dest;
    }

    kontainer * unload() {
        kontainer * kptr;
        kptr = cargo;
        cargo = cargo->getNext();
        kptr->setNext(NULL);
        minCargoNum();
        return kptr;
    }

    void display() {
        cout << "Ship ID: " << idNum << endl;
        if (docked) {cout << "Ship docked at Portsmouth. Status: " << status << endl;}
        else {cout << "Ship at sea." << endl;}
        cout << "---Ship " << idNum << " Cargo---" << endl;
        cout << "Cargo Ptr: " << cargo << endl;   /// DEL
        kontainer * curr;
        curr = cargo;
        while (curr != NULL) {
            curr->display();
            cout << "A: " << curr << endl;  /// DEL
            curr = curr->getNext();
            cout << "N: " << curr << endl;  /// DEL
            cout << "-------------------" << endl;
        }
    }

    void showWait() {
        if (next != NULL) {
            if (!docked) {cout <<"Ship " << idNum << " at sea." << endl;}
            next->showWait();
        }
        else if (next == NULL) {if (!docked) {cout << idNum << " at sea." << endl;}}
    }

private:
    bool docked;
    char status;    /// Unloading, Loading, Waiting
    string name;
    int idNum;
    int cargoNum;
    int delay;
    char dest;
    char destL, destR;
    ship * next;
    kontainer * cargo;
};
