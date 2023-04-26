//
// Created by colin on 2023/2/4.
//

#include "Joystick.h"

Joystick::Joystick(const std::string device) {
    dev = std::move(device);
}

int Joystick::open() {
    xbox_fd = ::open(dev.c_str(), O_RDONLY);
    return xbox_fd;
}
int Joystick::read() {

    int len, type, number, value;
    struct js_event js{};

    len = ::read(xbox_fd, &js, sizeof(struct js_event));

    type = js.type;
    number = js.number;
    value = js.value;

    map.time = js.time;

    if (type == JS_EVENT_BUTTON) {
        switch (number) {
            case XBOX_BUTTON_A:
                map.a = value;
                break;

            case XBOX_BUTTON_B:
                map.b = value;
                break;

            case XBOX_BUTTON_X:
                map.x = value;
                break;
            case XBOX_BUTTON_Y:
                map.y = value;
                break;
            case XBOX_BUTTON_LB:
                map.lb = value;
                break;

            case XBOX_BUTTON_RB:
                map.rb = value;
                break;

            case XBOX_BUTTON_START:
                map.start = value;
                break;

            case XBOX_BUTTON_BACK:
                map.back = value;
                break;

            case XBOX_BUTTON_HOME:
                map.home = value;
                break;

            case XBOX_BUTTON_LO:
                map.lo = value;
                break;

            case XBOX_BUTTON_RO:
                map.ro = value;
                break;

            default:
                break;
        }
    } else if (type == JS_EVENT_AXIS) {
        switch (number) {
            case XBOX_AXIS_LX:
                map.lx = value;
                break;

            case XBOX_AXIS_LY:
                map.ly = value;
                break;

            case XBOX_AXIS_RX:
                map.rx = value;
                break;

            case XBOX_AXIS_RY:
                map.ry = value;
                break;

            case XBOX_AXIS_LT:
                map.lt = value;
                break;

            case XBOX_AXIS_RT:
                map.rt = value;
                break;

            case XBOX_AXIS_XX:
                map.xx = value;
                break;

            case XBOX_AXIS_YY:
                map.yy = value;
                break;

            default:
                break;
        }
    }

    return len;
}

Joystick::~Joystick() {
    close(xbox_fd);
}
