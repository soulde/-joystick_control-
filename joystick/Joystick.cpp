//
// Created by colin on 2023/2/4.
//

#include "Joystick.h"

#include <memory>
#include <utility>

Joystick::Joystick(std::string device) : dev(std::move(device)), endLoop(false) {

}

int Joystick::open() {
    xbox_fd = ::open(dev.c_str(), O_RDONLY);
    readThread.reset(new std::thread([this] { readLoop(); }));
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
                map.trigger_a = !(value & map.a);
                map.a = value;
                break;

            case XBOX_BUTTON_B:
                map.trigger_b = !(value & map.b);
                map.b = value;
                break;

            case XBOX_BUTTON_X:
                map.trigger_x = !(value & map.x);
                map.x = value;
                break;
            case XBOX_BUTTON_Y:
                map.trigger_y = !(value & map.y);
                map.y = value;
                break;
            case XBOX_BUTTON_LB:
                map.trigger_lb = !(value & map.lb);
                map.lb = value;
                break;

            case XBOX_BUTTON_RB:
                map.trigger_rb = !(value & map.rb);
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
    endLoop = true;
    readThread->join();
    close(xbox_fd);
}

void Joystick::readLoop() {
    while (!endLoop) {
        int ret = read();
        if (ret < 0) {
            reset();
        }
    }
}

void Joystick::reset() {
    if (xbox_fd > 0) {
        close(xbox_fd);
    }
    xbox_fd = ::open(dev.c_str(), O_RDONLY);
}
