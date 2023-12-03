#ifndef QXIKEYWATCHER_H
# define QXIKEYWATCHER_H
# include <QHash>
# include <QObject>
# include <QThread>

typedef struct _XDisplay Display;

class QXIKeyWatcherThread : public QThread
{
    Q_OBJECT

public:
    QXIKeyWatcherThread(QObject *parent);
    void run() override;

signals:
    void sendKey(unsigned long scancode);

private:
    Display *_disp;
    int _group;
    int _xiOpcode;
    int _xkbEventCode;
};

class QXIKeyWatcher : public QObject
{
    Q_OBJECT

public:
    QXIKeyWatcher();

    void start();
    void unwatchKey(const char *str);
    void watchKey(const char *str, int index);

signals:
    void keyPressed(int);

private slots:
    void recvKey(unsigned long key);

private:
    QHash<unsigned long, int> hotkeys;
    QXIKeyWatcherThread *kwt;
};

#endif
