#ifndef STREAMER_ENUMS_H
#define STREAMER_ENUMS_H

class StreamerEnums {
public:
    enum { //Used to identify contents of JSON transmissions
        TRANSMISSION_INPUT,
        TRANSMISSION_DATA,
        TRANSMISSION_COMMAND
    };
    enum { //Commands used to interact over the input socket
        COMMAND_AV_PORT,
        COMMAND_AV_BANDWIDTH,
        COMMAND_SCREEN_SIZE_W,
        COMMAND_SCREEN_SIZE_H,
        COMMAND_SCREEN_OFFSET_X,
        COMMAND_SCREEN_OFFSET_Y,
        COMMAND_SCREENS,
        COMMAND_APPLY_SCREENRECT,
        COMMAND_SET_OVERLAY,

        //Specifying receiver in name, new syntax?
        COMMAND_S_SET_SCREEN,
        COMMAND_S_SET_BANDWIDTH,

        COMMAND_C_SET_OVERLAY,
        COMMAND_C_SET_STREAM,
        COMMAND_C_SET_S_S_W, //Screen size width
        COMMAND_C_SET_S_S_H,
        COMMAND_C_SET_S_O_X, //Screen offset X
        COMMAND_C_SET_S_O_Y,
        COMMAND_C_SET_RECT, //Screen rectangle used to scale coordinates
        COMMAND_C_REQ_SCREENS,
        COMMAND_C_PING_LATENCY
    };
};

#endif // STREAMER_ENUMS_H

