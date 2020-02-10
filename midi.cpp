#include <iostream>
#include <cstdlib>
#include <vector>
#include <signal.h>
#include "rtmidi/RtMidi.h"

using namespace std;

// Grab CTRL-C to end streams
bool done;
static void finish(int ignore) {done = true;}

// Function prototypes
void getOpenPorts(RtMidiIn*, RtMidiOut*);

int main() {
    // Container for midi messages
    vector<unsigned char> messages;

    // These bois point to a midi port
    RtMidiIn *midiIn = nullptr;
    RtMidiOut *midiOut = nullptr;

    // Construct midiIn port pointer
    try {
        midiIn = new RtMidiIn;
    } catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }

    // Construct midiOut port pointer
    try {
        midiOut = new RtMidiOut();
    } catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }

    // Set up CTRL-C interupt signal
    (void) signal(SIGINT, finish);

    // Initialize super dumb command prompt
    string word;
    string argument;
    do {
        cout << "> ";
        cin >> word;

        if (word == "ports") getOpenPorts(midiIn, midiOut);
        else if (word == "connect") {
            int portToOpen;
            cin >> argument;
            cin >> portToOpen;
            if (argument == "input")
                try {
                    midiIn->openPort(portToOpen - 1);
                    // Don't ignore sysex, time, or sense messages
                    midiIn->ignoreTypes(false, false, false);
                    cout << "Connected to input port " << portToOpen << endl;
                } catch(RtMidiError &error) {
                    error.printMessage();
                }
            else if (argument == "output")
                try {
                    midiOut->openPort(portToOpen - 1);
                    cout << "Connected to output port " << portToOpen;
                } catch(RtMidiError &error) {
                    error.printMessage();
                }
            else cout << "Invalid command" << endl;
        }
        else if (word == "track") {
            done = false;
            while (!done) {
                // Read in midi signal into buffer
                double stamp = midiIn->getMessage(&messages);
                int size = messages.size();
                for (int i = 0; i < size; i++) {
                    cout << "Byte " << i << ": " << (int)messages.at(i) << endl;
                }
                if (size > 0) cout << "Stamp: " << stamp << endl << endl;
                messages.clear();
            }
        }
        else cout << "Invalid command" << endl;
    } while(word != "exit");

    cleanup:
    delete midiIn;
    delete midiOut;

    return 0;
}

// Prints out the open input and output MIDI ports
void getOpenPorts(RtMidiIn* midiIn, RtMidiOut* midiOut) {
    // Find open input ports
    unsigned int nPorts = midiIn->getPortCount();
    cout << "There are " << nPorts << " open MIDI ports" << endl;
    string portName;
    // Print open input MIDI ports
    for (unsigned int i = 0; i < nPorts; i++) {
        try {
            portName = midiIn->getPortName(i);
        } catch(RtMidiError &error) {
            error.printMessage();
            return;
        }
        cout << "  Input Port #" << i + 1 << ": " << portName << endl;
    }

    // Print open output MIDI ports
    nPorts = midiOut->getPortCount();
    cout << "There are " << nPorts << " open output MIDI ports" << endl;
    for (unsigned int i = 0; i < nPorts; i++) {
        try {
            portName = midiOut->getPortName(i);
        } catch(RtMidiError &error) {
            error.printMessage();
            return;
        }
        cout << "  Output port #" << i + 1 << ": " << portName << endl;
    }
}
