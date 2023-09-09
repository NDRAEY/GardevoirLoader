#include "Graphics/Rect.hpp"
#include "Graphics/PSFFont.hpp"
#include "UI/List.hpp"

extern Video::BackFramebuffer __system_framebuffer;
extern Graphics::PSFFont __system_font;

UI::List::List(const std::vector<ListAction*>& named_actions)
: actions(named_actions) {
    // ...
}

void UI::List::set_position(int pos) {
	if(pos < 0) {
		position = actions.size() + pos;
	} else if (pos < actions.size()) {
        position = pos;
    } else {
		position = 0;
	}
}

size_t UI::List::get_position() const {
    return position;
}

void UI::List::set_select_color(size_t color) {
    select_color = color;
}

size_t UI::List::get_select_color() const {
    return select_color;
}

void UI::List::set_text_color(size_t color) {
    text_color = color;
}

size_t UI::List::get_text_color() const {
    return text_color;
}

void UI::List::render(size_t x, size_t y) {
    size_t sy = y;
    
    for(int i = 0; i < actions.size(); i++) {
        const ListAction* elem = actions[i];
        
        size_t text_color_now;

        if(i == position) {
            text_color_now = select_color;
        
            Graphics::DrawFillRect(
                __system_framebuffer,
                x,
                sy - 1,
                __system_framebuffer.screen.info.width,
                __system_font.get_glyph_height() + 4,
                text_color
            );
        } else {
            text_color_now = text_color;
        }

        __system_font.draw_string(
            __system_framebuffer,
			elem->name,
            x + 10,
            sy,
            text_color_now
        );

        sy += __system_font.get_glyph_height() + 5;
    }
}

UI::List::~List() {
    // ...
}