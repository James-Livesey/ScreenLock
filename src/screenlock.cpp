#include "screenlock.h"

ScreenLock::ScreenLock() :
_rootGrid(),
_messageBox(),
_messageGrid(),
_messageTitle("This is a test"),
_messageBody("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed maximus lectus libero, at faucibus felis blandit quis. Interdum et malesuada fames ac ante ipsum primis in faucibus.\n\nInteger ornare id purus vitae suscipit."),
_passwordEntry() {
    _cssProviderRef = Gtk::CssProvider::create();

    Gtk::StyleContext::add_provider_for_display(get_display(), _cssProviderRef, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    _cssProviderRef->load_from_path("theme/style.css");

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
    _passwordEntry.property_placeholder_text().set_value("Enter password for user james");

    _rootGrid.add_css_class("rootGrid");
    _rootGrid.attach(_messageBox, 0, 0);
    _rootGrid.attach(_passwordEntry, 0, 1);

    set_child(_rootGrid);

    fullscreen();

    property_default_width().signal_changed().connect(sigc::mem_fun(*this, &ScreenLock::onResize));
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