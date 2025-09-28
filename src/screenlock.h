#ifndef SCREENLOCK_H_
#define SCREENLOCK_H_

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/passwordentry.h>

class ScreenLock : public Gtk::Window {
    public:
        ScreenLock();
        ~ScreenLock() override {}

    protected:
        Gtk::Grid _rootGrid;
        Gtk::Box _messageBox;
        Gtk::Grid _messageGrid;
        Gtk::Label _messageTitle;
        Gtk::Label _messageBody;
        Gtk::PasswordEntry _passwordEntry;
};

#endif