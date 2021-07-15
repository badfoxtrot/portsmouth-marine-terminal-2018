/// Brian Julien - CS 150 Summer 2018

class yard
{
public:
    yard() {
        inv = 0;
        noLoad = false;
        inventory = NULL;
    }
    ~yard(){cout << "~Portsmouth Marine Terminal shut down." << endl;}

    void setNoLoad(bool b) {noLoad = b;}
    bool getNoLoad() {return noLoad;}

    int getInvNum() {return inv;}

    void addInv(kontainer * k) {
        if (inventory == NULL) {inventory = k;}
        else {k->setNext(inventory); inventory = k;}
        inv++;
    }

    void genInitInv(default_random_engine * gptr) {
        uniform_int_distribution<int> gInitDist(1,999);
        kontainer * kptr = NULL;
        for (int i=0; i < 20; i++){
            if (inventory == NULL){
                inventory = new kontainer;
                inventory->setID(gInitDist(*gptr));
                inventory->genOrigDest(gptr);
                inv++;
                kptr = inventory;
            }
            else {
                kptr = new kontainer;
                kptr->setID(gInitDist(*gptr));
                kptr->genOrigDest(gptr);
                kptr->setNext(inventory);
                inv++;
                inventory = kptr;
            }
        }
    }
    void addRandInv(default_random_engine * gptr) {
        uniform_int_distribution<int> gInitDist(1,999);
        kontainer * kptr = NULL;
            kptr = new kontainer;
            kptr->setID(gInitDist(*gptr));
            kptr->genOrigDest(gptr);
            kptr->setNext(inventory);
            inv++;
            inventory = kptr;
            cout << "Semi delivered Kontainer " << kptr->getID() << " to Yard." << endl;
    }

    kontainer * getNextVal() {
        curr = inventory;
        if (curr == NULL) {return NULL;}
        inventory = inventory->getNext();
        curr->setNext(NULL);
        return curr;
    }

    kontainer * onload() {
        kontainer * kptr;
        kptr = inventory;
        inventory = inventory->getNext();
        kptr->setNext(NULL);
        inv--;
        return kptr;
    }

    kontainer * unload(kontainer * kptr) {
        curr = inventory;
        if (kptr == curr) {
            inventory = inventory->getNext();
            kptr->setNext(NULL);
            return kptr;
        }
        else {
            while (curr->getNext() != kptr) {curr = curr->getNext();}
            curr->setNext(kptr->getNext());
            kptr->setNext(NULL);
            return kptr;
        }
    }

    kontainer * onloadDestTrain(train * tptr) {
        /*kontainer * firstKont;
        firstKont = inventory;
        inventory = inventory->getNext();
        firstKont->setNext(NULL);*/
        kontainer * prev;

        char kontDest, rangeL, rangeR, temp;
        temp = tptr->getDest();
        temp--;
        rangeL = temp;
        temp = temp + 2;
        rangeR = temp;

        kontDest = tptr->getDest();
        cout << "Destinations " << kontDest << " " << rangeL << " " << rangeR << " boarding." << endl;
        curr = inventory;

        while (curr != NULL) {
            char dest;
            kontainer * target = NULL;
            dest = curr->getDest();

            if ((dest == rangeL) || (dest == kontDest) || (dest == rangeR)) {
                cout << "Kontainer " << curr->getID() << " - Dest " << dest << " to be loaded on Train." << endl;
                target = curr;
                prev = inventory;
                while (prev->getNext() != target) {prev = prev->getNext();}
            }
            else {/*cout << curr->getID() << "-" << dest << " not to be loaded." << endl;*/}

            if (target != NULL) {
                prev->setNext(target->getNext()); target->setNext(NULL); inv--; return target;
            }
            curr = curr->getNext();
        }

        cout << "No more kontainers to load. Advance to next day." << endl;
        noLoad = true;
        return NULL;
    }

    kontainer * onloadDestShip(ship * sptr) {
        /*kontainer * firstKont;
        firstKont = inventory;
        inventory = inventory->getNext();
        firstKont->setNext(NULL);*/
        kontainer * prev;

        char kontDest, rangeL, rangeR, temp;
        temp = sptr->getDest();
        temp--;
        rangeL = temp;
        temp = temp + 2;
        rangeR = temp;

        kontDest = sptr->getDest();
        cout << "Destinations " << kontDest << " " << rangeL << " " << rangeR << " boarding." << endl;
        curr = inventory;

        while (curr != NULL) {
            char dest;
            kontainer * target = NULL;
            dest = curr->getDest();

            if ((dest == rangeL) || (dest == kontDest) || (dest == rangeR)) {
                cout << "Kontainer " << curr->getID() << " - Dest " << dest << " to be loaded on Ship " << sptr->getID() << endl;
                target = curr;
                prev = inventory;
                while (prev->getNext() != target) {prev = prev->getNext();}
            }
            else {/*cout << curr->getID() << "-" << dest << " not to be loaded." << endl;*/}

            if (target != NULL) {
                prev->setNext(target->getNext()); target->setNext(NULL); inv--; return target;
            }
            curr = curr->getNext();
        }

        cout << "No more kontainers to load. Advance to next day." << endl;
        noLoad = true;
        return NULL;
    }

    kontainer * getInv() {return inventory;}

    void display() {
        cout << "---Portsmouth Yard---" << endl;
        cout << "Inventory: " << inv << endl;
        kontainer * curr;
        curr = inventory;
        while (curr != NULL) {
            curr->display();
            curr = curr->getNext();
            cout << "---------------------" << endl;
        }
        cout << endl;
    }
private:
    int inv;
    bool noLoad;
    kontainer * inventory;
    kontainer * curr;
    train * tptr;
    ship * sptr;
    };
