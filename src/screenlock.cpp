#include <experimental/filesystem>
#include <stdlib.h>
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>

#include "screenlock.h"

std::string getBasePath() {
    char result[PATH_MAX];

    ssize_t length = readlink("/proc/self/exe", result, PATH_MAX);

    return std::string(result, length > 0 ? length : 0);
}

std::string getUsername() {
    char* userString = getenv("SUDO_USER");

    return std::string(userString ? userString : "");
}

ScreenLock::ScreenLock() :
_rootGrid(),
_messageBox(),
_messageGrid(),
_messageTitle(""),
_messageBody(""),
_passwordEntry() {
    _cssProviderRef = Gtk::CssProvider::create();

    Gtk::StyleContext::add_provider_for_display(get_display(), _cssProviderRef, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    std::string path = getBasePath();

    _cssProviderRef->load_from_path(path.substr(0, path.find_last_of("/")) + "/theme/style.css");

    _messageFile.open("/home/" + getUsername() + "/.screenlock");

    std::string title;

    std::getline(_messageFile, title);

    std::string body {std::istreambuf_iterator<char>(_messageFile), std::istreambuf_iterator<char>()};

    _messageTitle.set_text(title);
    _messageBody.set_text(body);

    _messageTitle.add_css_class("messageTitle");
    _messageTitle.set_wrap(true);

    _messageBody.add_css_class("messageBody");
    _messageBody.set_wrap(true);

    _messageGrid.add_css_class("messageGrid");
    _messageGrid.attach(_messageTitle, 0, 0);
    _messageGrid.attach(_messageBody, 0, 1);

    _messageBox.add_css_class("messageBox");
    _messageBox.set_halign(Gtk::Align::CENTER);
    _messageBox.set_valign(Gtk::Align::CENTER);
    _messageBox.append(_messageGrid);
    _messageBox.set_expand(true);

    _passwordEntry.add_css_class("passwordEntry");
    _passwordEntry.set_alignment(0.5);
    _passwordEntry.property_placeholder_text().set_value("Enter password for user " + getUsername());

    _rootGrid.add_css_class("rootGrid");
    _rootGrid.attach(_messageBox, 0, 0);
    _rootGrid.attach(_passwordEntry, 0, 1);

    set_child(_rootGrid);

    fullscreen();

    signal_close_request().connect(sigc::mem_fun(*this, &ScreenLock::onClose), false);

    property_default_width().signal_changed().connect(sigc::mem_fun(*this, &ScreenLock::onResize));

    _passwordEntryControllerKey = Gtk::EventControllerKey::create();

    _passwordEntryControllerKey->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    _passwordEntryControllerKey->signal_key_pressed().connect(sigc::mem_fun(*this, &ScreenLock::onPasswordEntry), false);

    _passwordEntry.add_controller(_passwordEntryControllerKey);
}

bool ScreenLock::checkPassword(std::string password) {
    const char* username = getUsername().c_str();
    struct passwd* entry = getpwnam(username);

    if (!entry) {
        printf("User does not exist; exiting anyway\n");

        return true;
    }

    std::string hash = entry->pw_passwd;

    if (hash == "x" || hash == "*") {
        // Check shadow file

        struct spwd* shadowEntry = getspnam(username);

        if (!shadowEntry) {
            printf("Could not read shadow file; exiting anyway\n");

            return true;
        }

        std::string shadowHash = shadowEntry->sp_pwdp;

        return crypt(password.c_str(), shadowHash.c_str()) == shadowHash;
    } else {
        return crypt(password.c_str(), hash.c_str()) == hash;
    }
}

bool ScreenLock::onClose() {
    return !checkPassword(_passwordEntry.get_text());
}

void ScreenLock::onResize() {
    auto display = Gdk::Display::get_default();

    if (!display) {
        return;
    }

    Gdk::Rectangle geometry;

    display->get_monitor_at_surface(get_surface())->get_geometry(geometry);

    int desiredWidth = 800;

    _messageBox.property_margin_start().set_value((geometry.get_width() - desiredWidth) / 2);
    _messageBox.property_margin_end().set_value((geometry.get_width() - desiredWidth) / 2);
}

bool ScreenLock::onPasswordEntry(guint keyval, guint keycode, Gdk::ModifierType state) {
    if (keyval == GDK_KEY_Return || keyval == GDK_KEY_KP_Enter) {
        if (checkPassword(_passwordEntry.get_text())) {
            close();
        } else {
            _passwordEntry.property_placeholder_text().set_value("Incorrect password; try again");
            _passwordEntry.set_text("");
        }

        return true;
    }

    return false;
}