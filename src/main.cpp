#include <iostream>
#include "Helpers.hpp"
#include "Video/Screen.hpp"
#include "Video/BackFramebuffer.hpp"
#include "Graphics/Pixel.hpp"
#include "Graphics/PSFFont.hpp"
#include "Graphics/Rect.hpp"
#include "Input/Event.hpp"
#include "UI/List.hpp"
#include "Version.hpp"

Video::Screen __system_screen("/dev/graphics/fb0");
Video::BackFramebuffer __system_framebuffer(__system_screen);
Graphics::PSFFont __system_font("/UniCyr8x8.psf");

int main() {
    for(int i = 0; i < __system_screen.info.height; i++)
        for(int j = 0; j < __system_screen.info.width; j++)
            Graphics::DrawPixel(__system_framebuffer, j, i, 0x00555555);

    __system_font.draw_string(
        __system_framebuffer,
        std::string(PROGNAME) + " v" + VERSION_STRING + " - The Simple Loader",
        10,
        10,
        0x0000FF00
    );

    Graphics::DrawFillRect(__system_framebuffer, 200, 200, 100, 200, 0x00FF00);
    Graphics::DrawRect(__system_framebuffer, 300, 300, 50, 50, 0xFFFF00);

    std::vector<UI::List::ListAction> actions = {
        {"Run Koraidon Firmware", [](){}},
        {"Clear screen and exit to adb", [](){}},
        {"Reboot to system", [](){}},
        {"Reboot to fastboot", [](){}},
    };

    UI::List list(actions);

    list.set_text_color(0xFF0000);
    list.set_select_color(0x00FF00);

    // For volume buttons
    // Input::Event event_listener("/dev/input/event3", false);

//    while(true) {
//        struct input_event ev = event_listener.get_event();
//
//        std::cout << "Event: code: " << ev.code << "; type: " << ev.type << "; value: " << ev.value << std::endl;
//    }

     while(true) {
        list.render(0, 30);

        __system_framebuffer.flush();
     }

    return 0;
}