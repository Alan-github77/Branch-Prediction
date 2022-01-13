#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

// How to run: ./branchsimulator config.txt trace.txt
int main (int argc, char** argv) {


    ifstream config;
    config.open(argv[1]);

    int m;
    config >> m;

    config.close();

    ofstream out;
    string out_file_name = string(argv[2]) + ".out";
    out.open(out_file_name.c_str());
    ifstream trace;
    trace.open(argv[2]);

    // initial saturating counter 11
    vector<bitset<2> > counter;
    int counter_size = pow(2, m);
    counter.resize(counter_size);
    for (int i=0; i<counter_size; i++){
        counter[i] = 3;
    }

    int l = 0;
    double misprediction = 0;

    while (!trace.eof()) {
        unsigned long pc; bool taken;
        trace >> std::hex >> pc >> taken;
        bool prediction;

        // calculate index of saturating counter
        bitset<32> idx32 = pc;
        int idx = 0;
        for (int i=0; i<m; i++) {
            if (idx32.test(i)){
                idx = idx + pow(2, i);
            }
        }

        // determine prediction; counter, right one -> predict taken
        bitset<2> state = counter[idx];
        prediction = state.test(1);

        if (taken){
            // after prediction, updating state machine
            if (state != 3){
                counter[idx] = state.to_ulong() + 1;
            }
        }
        else{   // not taken
            if (state != 0){
                counter[idx] = state.to_ulong() - 1;
            }
        }
        if (prediction != taken)
            misprediction++;

        out << prediction << endl;
    }

    cout << "m: " << m << "; misprediction rate: " << (misprediction/548) << endl;

    trace.close();
    out.close();
}