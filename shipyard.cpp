/// Brian Julien - CS 150 Summer 2018

#include <iostream>
#include <random>
#include <ctime>
#include <fstream>

using namespace std;

#include "kontainer.h"
#include "ship.h"
#include "car.h"
#include "train.h"
#include "yard.h"

char menu();

int main()
{
    default_random_engine * gptr;
    gptr = new default_random_engine (time(NULL));

    fstream fout;
    fout.open("terminal.txt",ios::out);
    fout << "Portsmouth Marine Terminal Log" << endl << endl;

    int day = 0;
    int numDocked = 0;

    yard Y;
    Y.genInitInv(gptr);

    train * tptr;
    tptr = new train;
    tptr->genID(gptr);
    tptr->createFreight(gptr);

    ship * fleet = NULL;
    ship * sptr = NULL;
    ship * last1 = NULL;
    ship * waitlist = NULL;

    char ans;
    bool cont = true;
    while (cont)
    {
        ans = menu();
        switch (ans)
        {
        case 'N': {     /// Time Loop -- Time stepping algorithm
                day++;
                cout << "------------------------------" << endl;
                cout << "Day: " << day << endl; fout << "Day: " << day << endl;
                fout << "Yard Kontainer Count: " << Y.getInvNum() << endl;

                uniform_int_distribution<int> shipDist(0,4);
                int s = shipDist(*gptr);
                cout <<"Ships Generated: " << s << endl; fout <<"Ships Generated: " << s << endl;

                if (s != 0) {      /// skip if 0 ships coming in
                    for (int i=0; i<s; i++){        /// Daily Ship Generation
                        if (fleet == NULL) {
                            fleet = new ship;
                            fleet->genID(gptr);
                            fleet->genCargo(gptr);
                            last1 = fleet;
                            fout << "Ship " << fleet->getID() << " arrives." << endl;
                        }
                        else {
                            sptr = new ship;
                            sptr->genID(gptr);
                            sptr->genCargo(gptr);
                            last1->setNext(sptr);
                            last1 = sptr;
                            fout << "Ship " << sptr->getID() << " arrives." << endl;
                        }
                    }
                }
                else {if (fleet == NULL){break;}}
                Y.addRandInv(gptr);
                fout << "Semi delivered a kontainer to Yard." << endl;

                waitlist = fleet;

                sptr = waitlist;
                while (sptr != NULL) {          /// Daily Ship Docking
                    if (numDocked < 3) {
                        if (sptr->getDock()) {sptr = sptr->getNext();}
                        else {
                            cout << "Docking - Ship " << sptr->getID() << endl;
                            fout << "Docking - Ship " << sptr->getID() << endl;
                            sptr->setDock(true);
                            sptr->setStatus('U');
                            numDocked++;
                            sptr = sptr->getNext();
                        }
                    }
                    else {sptr = sptr->getNext();}
                }

                sptr = waitlist;
                while (sptr != NULL) {
                    if (sptr->getDock()) {sptr->workday(); sptr = sptr->getNext();}
                    else {sptr = sptr->getNext();}
                }

                sptr = waitlist;
                while (sptr->getStatus() == 'S') {
                        ship * temp;
                        temp = fleet;
                        fleet = fleet->getNext();

                        /// Deleting Ship
                        cout << "Ship " << temp->getID() << " preparing for departure." << endl;
                        fout << "Ship " << temp->getID() << " departs." << endl;
                        temp->setNext(NULL);
                        numDocked--;
                        delete temp;

                        /// Docking replacement ship from waitlist
                        temp = fleet;
                        while (temp->getDelay() != 0) {temp = temp->getNext();}
                        temp->setDock(true);
                        numDocked++;
                        cout << "Docking - Ship " << temp->getID() << endl;
                        fout << "Docking - Ship " << temp->getID() << endl;
                        temp->setStatus('U');
                        temp->workday();

                        cout << "Adding to Yard - " << temp->getID()<< endl;
                        //fout << "Adding to Yard - " << temp->getID()<< endl;
                        for (int i=0; i<5; i++) {Y.addInv(temp->unload());}
                        sptr = fleet;
                }

                sptr = waitlist;
                while (sptr != NULL) {
                    if (sptr->getStatus() == 'U') {    /// Status U
                            cout << "Adding to Yard - " << sptr->getID()<< endl;
                            //fout << "Adding to Yard - " << sptr->getID()<< endl;
                            for (int i=0; i<5; i++) {Y.addInv(sptr->unload());}
                            if (sptr->getCargo() == NULL) {sptr->setStatus('L');}
                        }
                    else if (sptr->getStatus() == 'L') {    /// Status L
                        cout << "Removing from Yard - " << sptr->getID()<< endl;
                        //fout << "Removing from Yard - " << sptr->getID()<< endl;
                        if (sptr->getDest() == 'z') {sptr->addCargo(Y.onload());}

                        cout << "Ship " << sptr->getID() << " Dest: " << sptr->getDest() << endl;
                        //fout << "Ship " << sptr->getID() << " Dest: " << sptr->getDest() << endl;
                        while ( !Y.getNoLoad() ) {
                            kontainer * stemp;
                            stemp = Y.onloadDestShip(sptr);
                            if (stemp != NULL) {
                                sptr->addCargo(stemp);
                                fout << "Kontainer " << stemp->getID() << " loaded to ship." << endl;
                            }
                        }
                        Y.setNoLoad(false);

                        if (sptr->getDelay() >= 4) {sptr->setStatus('S');}
                    }
                    sptr = sptr->getNext();
                }

                if (tptr->getDest() == 'z') { tptr->loadFreight(Y.onload()); }
                cout << "Train Dest: " << tptr->getDest() << endl;
                cout << "Train Destination set to " << tptr->getDest() << "." << endl;

                while ( !Y.getNoLoad() ) {
                    kontainer * ktemp;
                    ktemp = Y.onloadDestTrain(tptr);
                    if(ktemp!=NULL) {
                        tptr->loadFreight(ktemp);
                        fout << "Kontainer " << ktemp->getID() << " loaded to train." << endl;
                    }
                }

                if (tptr->getFreightNum() > 26) {
                    cout << "Train " << tptr->getID() << " preparing for departure." << endl;
                    fout << "Train " << tptr->getID() << " departs." << endl;
                    delete tptr;
                    tptr = new train;
                    tptr->genID(gptr);
                    tptr->createFreight(gptr);
                }
                Y.setNoLoad(false); /// allows function to run again

                sptr = fleet;
                fout << "------------------------------" << endl;
                break;
            }

        case 'Y': { Y.display(); break; }   // Display Yard

        case 'T': { tptr->display(); break; }   // Display Train

        case 'S': {     // Display Ships
            if (fleet==NULL) {cout << "No ships out to sea. Advance to next day." << endl;}
            else {sptr = fleet;}
            while (sptr != NULL) {
                cout << "Ship: " << sptr->getID() << " Status: " << sptr->getStatus() <<
                    " Cargo: " << sptr->getCargoNum() << " Day: " << sptr->getDelay() << endl;
                sptr = sptr->getNext();
            }
            cout << endl;
            sptr = fleet;
            break;
        }

        case 'W': {     // Display Waitlist
            if (sptr==NULL) {cout << "No ships out to sea. Advance to next day." << endl;}
            else {sptr->showWait();}
            sptr = fleet;
            break;
        }

        case 'F': {     // Display All
            Y.display();
            tptr->display();
            if (sptr == NULL) {cout << "No ships out to sea. Advance to next day." << endl;}
            while (sptr != NULL){
                sptr->display();
                cout << "ADDR: " << sptr << endl;
                sptr = sptr->getNext();
                cout << "NEXT: " << sptr << endl << endl;
            }
            sptr = fleet;
            break;
        }

        case 'Q': {cont = false; fout << "User closed program." << endl; break;}

        default: {cout << ans << " not an option." << endl; break;}
        }// End of Menu Switch
    }// End of Menu While

    fout.close();
    return 0;
}// End of Main

char menu()
{
    char ans;
    cout << "     Portsmouth Marine Terminal" << endl;
    cout << "N    Advance to Next Day" << endl;
    cout << "Y    Display Yard" << endl;
    cout << "T    Display Train" << endl;
    cout << "S    Display Ships" << endl;
    cout << "W    Display Waitlist" << endl;
    cout << "F    Display Full Terminal Status" << endl;
    cout << "Q    Quit" << endl;
    cin >> ans;
    cout << endl;
    return ans;
}
