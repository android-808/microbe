#ifndef SESSION_H
#define SESSION_H

#include <QObject>

class QMozContext;
class BookmarksModel;
class Settings;
//class Notifications;
//class TransferManager;
class QNetworkConfigurationManager;
class QNetworkAccessManager;
//class ThumbnailCache;
#ifdef Q_WS_MAEMO_5
//class WindowStack;
#endif

class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(QObject *parent = 0);
    ~Session();
    inline QNetworkAccessManager* networkAccessManager() const { return m_nam; }
    static Session* instance();
    inline BookmarksModel* bookmarks() const { return m_bookmarks; }

private Q_SLOTS:
    void onInitialized();
    
private:
    QNetworkConfigurationManager *m_ncm;
    QNetworkAccessManager *m_nam;
    BookmarksModel *m_bookmarks;
    Settings *m_settings;
    QMozContext *m_context;
//    Notifications *m_notifications;
//    TransferManager *m_transferManager;
//    ThumbnailCache *m_cache;
#ifdef Q_WS_MAEMO_5
//    WindowStack *m_stack;
#endif
};

#endif // SESSION_H
