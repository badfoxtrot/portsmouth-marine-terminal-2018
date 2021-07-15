/// Brian Julien - CS 150 Summer 2018

class car
{
public:
    car()
    {
        next = NULL;
        k1 = NULL;
        k2 = NULL;
        idNum = 0;
    }
    ~car(){}

    void setID(int i) {idNum = i;}
    void setK1(kontainer * k) {k1 = k;}
    void setK2(kontainer * k) {k2 = k;}
    void setNext(car * n) {next = n;}
    void genID(default_random_engine * gptr) {
        uniform_int_distribution<int> carDist(0,999);
        idNum = carDist(*gptr);
    }

    int getID() {return idNum;}
    kontainer * getK1() {return k1;}
    kontainer * getK2() {return k2;}
    car * getNext() {return next;}

    void display() {
        cout << "Car ID: " << idNum << endl;
        if (k1 != NULL) {cout << "1 - Kontainer ID: " << k1->getID() << endl;}
        if (k2 != NULL) {cout << "2 - Kontainer ID: " << k2->getID() << endl;}
    }

    void cleanup() {
        if (next != NULL) {next->cleanup();}
        cout << "Deleting " << idNum << endl;
        delete this;
    }
private:
    int idNum;
    car * next;
    kontainer * k1;
    kontainer * k2;
};
