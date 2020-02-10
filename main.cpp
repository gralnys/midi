#include <iostream>
#include <cstdlib>
#include "rtmidi/RtMidi.h"

using namespace std;

// Function prototypes
void getOpenPorts(RtMidiIn*, RtMidiOut*);

int main() {
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

    // Initialize command prompt
    string word;
    do {
        cout << "> ";
        cin >> word;

        if (word == "ports") getOpenPorts(midiIn, midiOut);
    } while(word != "exit");

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
