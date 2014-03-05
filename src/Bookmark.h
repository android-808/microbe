#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QObject>

class Bookmark : public QObject {
    Q_OBJECT

public:
    Bookmark(QString url, QString title, QString favicon, QObject* parent = 0);

    QString url() const;
    void setUrl(QString url);

    QString title() const;
    void setTitle(QString title);

    QString favIcon() const;
    void setFavIcon(QString favicon);

Q_SIGNALS:
    void urlChanged();
    void titleChanged();
    void faviconChanged();

private:
    QString mUrl;
    QString mTitle;
    QString mFavIcon;
};

#endif // BOOKMARK_H
