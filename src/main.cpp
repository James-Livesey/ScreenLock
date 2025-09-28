#include <gtkmm/application.h>

#include "screenlock.h"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("me.jamesl.screenlock");

    return app->make_window_and_run<ScreenLock>(argc, argv);
}