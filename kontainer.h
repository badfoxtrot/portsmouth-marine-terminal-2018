/// Brian Julien - CS 150 Summer 2018

class kontainer
{
public:
    kontainer() {
        next = NULL;
        idNum = 0;
        orig = 'Z';
        secCh = 'P';
        dest = 'Z';
    }
    ~kontainer() {/*cout << "~Kontainer " << idNum << " destroyed." << endl;*/}
    void setID(int i) {idNum = i;}
    void setOrig(char o) {orig = o;}
    void setDest(char d) {dest = d;}
    void genID(default_random_engine * gptr) {
        uniform_int_distribution<int> kontDist(0,999);
        idNum = kontDist(*gptr);
    }
    void genOrigDest(default_random_engine * gptr) {
        uniform_int_distribution<int> charGen(0,7);
        uniform_int_distribution<int> cointoss(0,1);
        int i, j;
        i = cointoss(*gptr);
        j = charGen(*gptr);
        if (i==0) {
            orig = 'A' + j;
            i = cointoss(*gptr);
            j = charGen(*gptr);
            if (i==0) {dest = 'A' + j;}
            else {dest = 'a' + j;}
        }
        else {
            orig = 'a' + j;
            /// Destination code MUST be capital
            j = charGen(*gptr);
            dest = 'A' + j;
        }
    }
    void setNext(kontainer * n) {next = n;}

    int getID() {return idNum;}
    char getOrig() {return orig;}
    char getSec() {return secCh;}
    char getDest() {return dest;}
    kontainer * getNext() {return next;}

    void display() {
        cout << "Kontainer ID: " << idNum << endl;
        cout << "Kontainer Code: " << orig << secCh << dest << endl;
    }
private:
    char orig;      /// Capital by sea, lower case by rail
    char secCh;     /// Will always be P
    char dest;
    int idNum;      /// sequential from 1000
    kontainer * next;
};
