/// Brian Julien - CS 150 Summer 2018

class train
{
public:
    train()
    {
        freight = NULL;
        dest = 'z';
        idNum = 0;
        cars = 0;
        freightNum = 0;
    }
    ~train(){cout << "~Train " << idNum << " starts rolling." << endl;}
    void setID(int i) {idNum = i;}
    void setCars(int c) {cars = c;}
    void setFreightNum(int f) {freightNum = f;}
    void genID(default_random_engine * gptr) {
        uniform_int_distribution<int> tDist(1,50);
        idNum = tDist(*gptr);
    }
    kontainer * getFreight() {return firstKont;}

    int getID() {return idNum;}
    int getNumCars() {return cars;}
    char getDest() {return dest;}
    int getFreightNum() {return freightNum;}

    void createFreight(default_random_engine * gptr) {
        car * cptr = NULL;
        for (int i=0; i < 15; i++) {
            if (freight == NULL) {
                freight = new car;
                freight->genID(gptr);
                cptr = freight;
            }
            else {
                cptr = new car;
                cptr->genID(gptr);
                cptr->setNext(freight);
                freight = cptr;
            }
        }
    }

    car * getCar() {return freight;}
    car * getNextCar() {return freight->getNext();}

    void loadFreight(kontainer * kptr) {
        if (freight->getK1() == NULL) {dest = kptr->getDest();}
        firstKont = freight->getK1();

        car * cptr;
        cptr = freight;
        while (cptr != NULL) {
            if (cptr->getK1() != NULL) {
                if (cptr->getK2() != NULL) {cptr = cptr->getNext();}
                else {
                    cptr->setK2(kptr);
                    cout << "Loading " << kptr->getID() << " on Car " << cptr->getID() << "-2" << endl;
                    freightNum++;
                    cptr = NULL;
                }
            }
            else {cptr->setK1(kptr); cout << "Loading " << kptr->getID() << " on Car " << cptr->getID() << "-1" << endl; freightNum++; cptr = NULL;}
        }//end while
    }//end void

    void loadFreightDest(kontainer * kptr) {

    }

    void display() {
        cout << "Train ID: " << idNum << endl;
        cout << "Destination: " << dest << endl;
        cout << "---Current Freight---" << endl;
        car * curr;
        curr = freight;
        while (curr != NULL) {
            curr->display();
            curr = curr->getNext();
            cout << "---------------------" << endl;
        }
        cout << endl;
    }

private:
    int idNum;
    int cars;
    char dest;
    int freightNum;
    car * freight;
    kontainer * firstKont;
};
