#include <iostream>
#include <cstdlib>
#include "rtmidi/RtMidi.h"

using namespace std;

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
            goto cleanup;
        }
        cout << "\tInput Port #" << i + 1 << ": " << portName << endl;
    }
    cout << endl;

    // Print open output MIDI ports
    nPorts = midiOut->getPortCount();
    cout << "There are " << nPorts << " open output MIDI ports" << endl;
    for (unsigned int i = 0; i < nPorts; i++) {
        try {
            portName = midiOut->getPortName(i);
        } catch(RtMidiError &error) {
            error.printMessage();
            goto cleanup;
        }
        cout << "\tOutput port #" << i + 1 << ": " << portName << endl;
    }
    cout << endl;

    cleanup:
    delete midiIn;
    delete midiOut;

    return 0;
}