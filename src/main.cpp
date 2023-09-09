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
#include <sys/mount.h>

#define FIRMWARE_FOLDER "/sdcard/.firmwares/"

Video::Screen __system_screen("/dev/graphics/fb0");
Video::BackFramebuffer __system_framebuffer(__system_screen);
Graphics::PSFFont __system_font("/UniCyr8x8.psf");

void do_mount_routines() {
	// ...
	// Find system partition by name and mount it to /system
	// Find userdata partition by name and mount it to /data
	// Then hardlink folder /data/media/0/ to /sdcard (ln -s ... /sdcard)

	try {
		Directory devices("/dev/block/platform/");

		auto dirs = devices.list_file_names();

		// Also count ".", ".."
		if(dirs.size() < 3) {
			logerr() << "Failed to find named partitions" << logend();

			return;
		}

		// FIXME: Only 1 device supported.
		Directory partitions(devices.get_path() + dirs[2] + "/by-name/");
		auto partition_names = partitions.list_file_names();

		if(std::find(partition_names.begin(), partition_names.end(), "system") != partition_names.end()) {
			auto full_path = partitions.get_path() + "system";

			std::cout << "Found system partition! [" << full_path.c_str() << "]" << std::endl;

			auto result = mount(
					full_path.c_str(),
					"/system",
					"ext4",
					0,
					nullptr
			);

			if(result < 0) {
				logerr() << strerror(errno) << logend();
			}

			std::cout << "System OK!" << std::endl;
		}

		if(std::find(partition_names.begin(), partition_names.end(), "userdata") != partition_names.end()) {
			auto full_path = partitions.get_path() + "userdata";

			std::cout << "Found userdata partition! [" << full_path.c_str() << "]" << std::endl;

			auto result = mount(
					full_path.c_str(),
					"/data",
					"ext4",
					0,
					nullptr
			);

			if(result < 0) {
				logerr() << strerror(errno) << logend();
			}

			std::cout << "Data OK!" << std::endl;
			std::cout << "Linking /sdcard!" << std::endl;

			auto link_status = symlink("/data/media/0/", "/sdcard");

			if(link_status < 0) {
				logerr() << strerror(errno) << logend();
			}
		}
	} catch (const std::runtime_error& error) {
		logerr() << error.what() << logend();
	}
}

int main() {
	do_mount_routines();

    std::vector<UI::List::ListAction*> actions = {
		new UI::List::ListAction {"Run TWRP Recovery", [](const std::string& name){
			execl("/sbin/recovery", "recovery", (void*)nullptr);
		}},
		new UI::List::ListAction  {"Clear screen and hang", [](const std::string& name){
			for(int i = 0; i < __system_screen.info.height; i++)
				for(int j = 0; j < __system_screen.info.width; j++)
					Graphics::DrawPixel(__system_framebuffer, j, i, 0x00000000);

			__system_framebuffer.flush();

			while(1);
		}},
		new UI::List::ListAction {"Reboot to system", [](const std::string& name){
			execl("/sbin/reboot", "reboot", (char*)nullptr);
		}},
		new UI::List::ListAction {"Reboot to fastboot", [](const std::string& name){
			execl("/sbin/reboot", "reboot", "bootloader", (void*)nullptr);
		}},
		new UI::List::ListAction {"Reboot to recovery", [](const std::string& name){
			execl("/sbin/reboot", "reboot", "recovery", (void*)nullptr);
		}},
		new UI::List::ListAction {"Power off", [](const std::string& name){
			execl("/sbin/poweroff", "poweroff", (void*)nullptr);
		}},
    };

	try {
		Directory dir(FIRMWARE_FOLDER);

		auto files = dir.list_file_names();

		for (const auto &i: files) {
			if (i == "." || i == "..")
				continue;

			std::printf("Found firmware: %s\n", i.c_str());

			actions.insert(actions.begin(), new UI::List::ListAction {
				i, [](const std::string& name){
					auto fwpath = FIRMWARE_FOLDER + name;

					std::cout << "Launching: " << fwpath << std::endl;

					execl(fwpath.c_str(), fwpath.c_str(), (void*)nullptr);

					while(1);
				}
			});
		}
	} catch(std::runtime_error& rte) {
		logerr() << rte.what() << logend();
		logerr() << "Firmwares will be not visible in the menu" << logend();
	}

	std::cout << "OK" << std::endl;

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
				std::cout << "Selected: " << list.get_position() << " [" << actions[list.get_position()]->name << "]" << std::endl;

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

	actions[pos]->action(actions[pos]->name);

	for(auto* i : actions) {
		delete i;
	}

    return 0;
}
