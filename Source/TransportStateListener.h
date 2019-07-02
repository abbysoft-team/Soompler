#pragma once

enum TransportState
{
    Stopped,
    Starting,
    Playing,
    Stopping,
    Ready
};

class TransportStateListener {
public:
    virtual void transportStateChanged(TransportState state) = 0;
};
