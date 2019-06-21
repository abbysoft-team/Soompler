#pragma once

enum TransportState
{
    Stopped,
    Starting,
    Playing,
    Stopping
};

class TransportStateListener {
public:
    virtual void transportStateChanged(TransportState state) = 0;
};
