#include <iostream>
#include <unistd.h>
#include "Helpers.hpp"
#include "Video/Screen.hpp"
#include "Video/BackFramebuffer.hpp"
#include "Graphics/Pixel.hpp"
#include "Graphics/PSFFont.hpp"
#include "Graphics/Rect.hpp"
#include "Input/Event.hpp"
#include "UI/List.hpp"
#include "Version.hpp"
#include "Input/VolumeButtons.hpp"
#include "Input/PowerButton.hpp"
#include "Directory.hpp"

#define FIRMWARE_FOLDER "/firmware/"

Video::Screen __system_screen("/dev/graphics/fb0");
Video::BackFramebuffer __system_framebuffer(__system_screen);
Graphics::PSFFont __system_font("/UniCyr8x8.psf");

int main() {
	try {
		Directory dir(FIRMWARE_FOLDER);

		for(const auto& i : dir.list_file_names())
			std::printf("- %s\n", i.c_str());
	} catch(std::runtime_error& rte) {
		logerr() << rte.what() << logend();
		logerr() << "Firmwares will be not visible in the menu" << logend();
	}


    std::vector<UI::List::ListAction> actions = {
		{"Run TWRP Recovery", [](){
			execl("/sbin/recovery", "recovery", (void*)nullptr);
		}},
		{"Clear screen and hang", [](){
			for(int i = 0; i < __system_screen.info.height; i++)
				for(int j = 0; j < __system_screen.info.width; j++)
					Graphics::DrawPixel(__system_framebuffer, j, i, 0x00000000);

			__system_framebuffer.flush();

			while(1);
		}},
        {"Reboot to system", [](){
			execl("/sbin/reboot", "reboot", (char*)nullptr);
		}},
		{"Reboot to fastboot", [](){
			execl("/sbin/reboot", "reboot", "bootloader", (void*)nullptr);
		}},
		{"Reboot to recovery", [](){
			execl("/sbin/reboot", "reboot", "recovery", (void*)nullptr);
		}},
		{"Power off", [](){
			execl("/sbin/poweroff", "poweroff", (void*)nullptr);
		}},
    };

    UI::List list(actions);

    list.set_text_color(0xFF0000);
    list.set_select_color(0x00FF00);

	// For volume buttons
    Input::Event volbtn_event_listener("/dev/input/event3", true);
    Input::Event power_event_listener("/dev/input/event4", true);

    while(true) {
		struct input_event volbtn_ev = volbtn_event_listener.get_event();
		struct input_event powerbtn_ev = power_event_listener.get_event();

		for(int i = 0; i < __system_screen.info.height; i++)
			for(int j = 0; j < __system_screen.info.width; j++)
				Graphics::DrawPixel(__system_framebuffer, j, i, 0x00555555);

		if(volbtn_ev.code == EV_VOLUME_UP) {
			if(volbtn_ev.value == 1)
				list.set_position(list.get_position() - 1);
		} else if(volbtn_ev.code == EV_VOLUME_DOWN) {
			if (volbtn_ev.value == 1)
				list.set_position(list.get_position() + 1);
		} else if(powerbtn_ev.code == EV_POWER) {
			if (powerbtn_ev.value == 1) {
				std::cout << "Selected: " << list.get_position() << std::endl;

				break;
			}
		}

		__system_font.draw_string(
				__system_framebuffer,
				std::string(PROGNAME) + " v" + VERSION_STRING + " - The Simple Loader",
				10,
				10,
				0x00FF00
		);

		std::string time = Time::GetTime("%H:%M:%S");

		__system_font.draw_string(
				__system_framebuffer,
				time,
				__system_framebuffer.screen.info.width - 10 - (time.length() * __system_font.get_glyph_width()),
				10,
				0x00FF00
		);

		Graphics::DrawFillRect(__system_framebuffer, 0, 25, __system_screen.info.width, 1, 0x00FF00);

		list.render(0, 30);

		__system_framebuffer.flush();
	}

	size_t pos = list.get_position();

	actions[pos].action();

    return 0;
}
