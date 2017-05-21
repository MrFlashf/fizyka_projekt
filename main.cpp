#include <iostream>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <tgmath.h>
//#include <graphics.h>
using namespace std;
const int SIZE = 50;

void randomizeArray(double array[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            double r = rand() % 2;
            array[i][j] = r == 0 ? (double)-1 : r;
        }
    }
}

class Model {
private:
    int steps;
    int stepsDone = 0;
    double array[SIZE][SIZE];
    double temperature;
public:
    Model(int steps, double temperature) {
        this->steps = steps;
        this->temperature = temperature;
        randomizeArray(array);
        run();
    };

    void invert(int x, int y) {
        array[y][x] *= -1.0;
    }

    void touch() {
        int x = rand() % SIZE;
        int y = rand() % SIZE;

        double neighbours[4] = {
            array[y][x + 1 == SIZE ? 0 : x + 1], //right
            array[y + 1 == SIZE ? 0 : y + 1][x], //bottom
            array[y][x - 1 == -1 ? SIZE-1 : x - 1],//left
            array[y - 1 == -1 ? SIZE-1 : y - 1][x] //top
        };

        double state = array[y][x];

        double neighboursSum = 0;
        for (int i = 0; i < 4; i++) {
            neighboursSum += neighbours[i];
        }

        double ep = -1 * state * neighboursSum;
        double ek =      state * neighboursSum;

        double deltaE   = ek - ep;
        double t        = -1 * (deltaE / temperature);
        double exp1     = exp(t);
        double r        = ((double)rand() / (double)RAND_MAX);

        if (deltaE <= 0 || r <= exp1) {
            invert(x,y);
        }
    }

    void display() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                string toDisplay = array[i][j] == 1 ? "x" : "o";
                cout << toDisplay;
            }
            cout << endl;
        }
        cout << endl<<endl;
    }

    void mcStep() {
        for (int i = 0; i < SIZE*SIZE; i++) {
            touch();
        }
    }

    bool sum() {
        double m = 0.0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
               m += array[i][j];
            }
        }
        m = m / (SIZE*SIZE);
        return m == 1 || m == -1;
    }

    void run() {
//        display();
        for (int i = 0; i < steps; i++) {
            mcStep();
            stepsDone++;
            if (i % 5000==0)
                display();
            if (sum()) {
                cout << stepsDone;
                display();
                return;
            }
        }
    }
};

int main() {
    srand(time(0));
    new Model(6000000, 2);
}



