#ifndef SCREENLOCK_H_
#define SCREENLOCK_H_

#include <gtkmm.h>

class ScreenLock : public Gtk::Window {
    public:
        ScreenLock();
        ~ScreenLock() noexcept override {}

    protected:
        Gtk::Grid _rootGrid;
        Gtk::Box _messageBox;
        Gtk::Grid _messageGrid;
        Gtk::Label _messageTitle;
        Gtk::Label _messageBody;
        Gtk::PasswordEntry _passwordEntry;

        Glib::RefPtr<Gtk::CssProvider> _cssProviderRef;

        void onResize();
};

#endif