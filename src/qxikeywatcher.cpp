#include "qxikeywatcher.h"

#include <X11/extensions/XInput2.h>
#include <X11/XKBlib.h>

#define DISPLAY_NAME ":0"

QXIKeyWatcherThread::QXIKeyWatcherThread(QObject *parent)
{
    _disp = XOpenDisplay(NULL);

    if (NULL == _disp) {
        fprintf(stderr, "Cannot open X _display '%s'\n", DISPLAY_NAME);
        exit(1);
    }

    {
        int queryEvent, queryError;

        if (! XQueryExtension(_disp, "XInputExtension", &_xiOpcode,
                    &queryEvent, &queryError)) {
            fprintf(stderr, "X Input extension not available\n");
            exit(2);
        }
    }
    {
        int major = 2, minor = 0;
        int queryResult = XIQueryVersion(_disp, &major, &minor);

        if (queryResult == BadRequest) {
            fprintf(stderr, "Need XI 2.0 support (got %d.%d)\n", major, minor);
            exit(3);
        } else if (queryResult != Success) {
            fprintf(stderr, "XIQueryVersion failed!\n");
            exit(4);
        }
    }
    {
        Window root = DefaultRootWindow(_disp);
        XIEventMask mask[2];
        XIEventMask *m;

        m = &mask[0];
        m->deviceid = XIAllDevices;
        m->mask_len = XIMaskLen(XI_LASTEVENT);
        m->mask = (unsigned char *)calloc(m->mask_len, sizeof(char));
        XISetMask(m->mask, XI_KeyPress);

        m = &mask[1];
        m->deviceid = XIAllMasterDevices;
        m->mask_len = XIMaskLen(XI_LASTEVENT);
        m->mask = (unsigned char *)calloc(m->mask_len, sizeof(char));

        XISelectEvents(_disp, root, &mask[0], 2);
        XSync(_disp, false);
        free(mask[0].mask);
        free(mask[1].mask);
    }

    int xkbOpcode;

    {
        int queryError, majorVersion, minorVersion;

        if (! XkbQueryExtension(_disp, &xkbOpcode, &_xkbEventCode, &queryError,
                    &majorVersion, &minorVersion)) {
            fprintf(stderr, "Xkb extension not available\n");
            exit(2);
        }
    }

    // Register to receive events when the keyboard's keysym group changes.
    // Keysym groups are normally used to switch keyboard layouts.  The
    // keyboard continues to send the same keycodes (numeric identifiers of
    // keys) either way, but the active keysym group determines how those map
    // to keysyms (textual names of keys).
    XkbSelectEventDetails(_disp, XkbUseCoreKbd, XkbStateNotify,
            XkbGroupStateMask, XkbGroupStateMask);

    {
        XkbStateRec state;

        XkbGetState(_disp, XkbUseCoreKbd, &state);
        _group = state.group;
    }

    XSync(_disp, False);
}

void QXIKeyWatcherThread::run()
{
    while (1) {
        XEvent event;
        XGenericEventCookie *cookie = (XGenericEventCookie*)&event.xcookie;

        XNextEvent(_disp, &event);

        if (XGetEventData(_disp, cookie) &&
            cookie->type == GenericEvent &&
            cookie->extension == _xiOpcode)
        {
            XIDeviceEvent *ev = (XIDeviceEvent *)cookie->data;
            KeySym s = XkbKeycodeToKeysym(ev->display, 
                ev->detail, ev->group.effective, 0 /*shift level*/);

            emit sendKey(s);
        }
    }
}

QXIKeyWatcher::QXIKeyWatcher()
{
    kwt = new QXIKeyWatcherThread(this);

    connect(kwt, &QXIKeyWatcherThread::sendKey,
            this, &QXIKeyWatcher::recvKey,
            Qt::QueuedConnection);
}

void QXIKeyWatcher::watchKey(const char *str, int index)
{
    KeySym s = XStringToKeysym(str);

    hotkeys[s] = index;
}

void QXIKeyWatcher::unwatchKey(const char *str)
{
    KeySym s = XStringToKeysym(str);

    hotkeys.take(s);
}

void QXIKeyWatcher::recvKey(unsigned long keysym)
{
    int result = hotkeys.value(keysym, -1);

    if (result == -1)
        return;

    emit keyPressed(result);
}

void QXIKeyWatcher::start()
{
    kwt->start();
}
