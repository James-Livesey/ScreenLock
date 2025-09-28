#include "screenlock.h"

ScreenLock::ScreenLock() :
_rootGrid(),
_messageBox(),
_messageGrid(),
_messageTitle("Test"),
_messageBody("Test"),
_passwordEntry() {
    _messageGrid.attach(_messageTitle, 0, 0);
    _messageGrid.attach(_messageBody, 0, 1);

    _messageBox.append(_messageGrid);
    _messageBox.set_expand(true);

    _rootGrid.attach(_messageBox, 0, 0);
    _rootGrid.attach(_passwordEntry, 0, 1);

    set_child(_rootGrid);

    fullscreen();
}