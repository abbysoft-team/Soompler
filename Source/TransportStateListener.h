#pragma once

enum TransportState
{
    Stopped,
    Starting,
    Playing,
    Stopping,
    NewFile,
    Ready
};

class TransportStateListener {
public:
    virtual void transportStateChanged(TransportState state) = 0;
    
    virtual ~TransportStateListener() = default;
};
