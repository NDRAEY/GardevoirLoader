#pragma once

#include <stdint.h>
#include <vector>

namespace UI {
    class List {
        public:

        typedef struct {
            const char* name;
            void (*action)();
        } ListAction;

        List(const std::vector<ListAction>& named_actions);
        ~List();

        void render(size_t x, size_t y);

        void set_position(size_t pos);
        size_t get_position() const;

        void set_select_color(size_t color);
        size_t get_select_color() const;

        void set_text_color(size_t color);
        size_t get_text_color() const;

        private:

        const std::vector<ListAction>& actions;
        size_t position = 0;
        size_t select_color = 0x00FF00;
        size_t text_color = 0xFF00FF;
    };
}