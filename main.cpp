#include <iostream>
#include <fstream>
#include <random>
#include <tgmath.h>
#include <algorithm>

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
        this->steps       = steps;
        this->temperature = temperature;
        randomizeArray(array);
        run();
    };

    void invert(int x, int y) {
        array[y][x] *= -1.0;
    }

    void touch(int x,int y) {

        double neighboursSum = 0;

        neighboursSum +=
                  array[y][x + 1 == SIZE ? 0 : x + 1] //right
                + array[y + 1 == SIZE ? 0 : y + 1][x] //bottom
                + array[y][x - 1 == -1 ? SIZE-1 : x - 1]//left
                + array[y - 1 == -1 ? SIZE-1 : y - 1][x]; //top

        double state = array[y][x];

        double ek = state * neighboursSum;
        double ep = -1 * ek;

        double deltaE   = ek - ep;
        double r        = ((double)rand() / (double)RAND_MAX);

        if ((deltaE <= 0) || r <= exp(-1 * (deltaE / temperature))) {
            invert(x,y);
        }
    }

    void mcStep() {
        for (int i = 0; i < SIZE*SIZE; i++) {
            int x = rand() % SIZE;
            int y = rand() % SIZE;
            touch(x,y);
        }
    }

    double sum() {
        double m = 0.0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
               m += array[i][j];
            }
        }
        m = m / (SIZE*SIZE);
        m < 0 ? m = -1*m : m;
        return m;
    }

    void run() {
        ofstream file;
        string fileName = "./wykres_"+to_string(temperature)+".txt";
        file.open(fileName);

        for (int i = 0; i < steps; i++) {
            mcStep();
            stepsDone++;
            double summ = sum();
            if (i % 10 == 0) {
                string sumString = to_string(summ);
                replace(sumString.begin(), sumString.end(), '.', ',');
                file << sumString << endl;
            }
            if (summ == 1.0) {
                cout << stepsDone;
                file << summ << endl;
                return;
            }
        }
        file.close();
    }
};

int main(int argc, char** argv) {
    srand(time(0));

    int mcsteps = argc > 2 ? stoi(argv[1]) : 50000;
    double temp = argc > 2 ? stod(argv[2]) : 1;
    new Model(mcsteps, temp);
    return 0;
}
