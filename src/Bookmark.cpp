#include "Bookmark.h"

Bookmark::Bookmark(QString url, QString title, QString favicon, QObject* parent) :
    QObject(parent), mUrl(url), mTitle(title), mFavIcon(favicon)
{
}

QString Bookmark::url() const {
    return mUrl;
}

void Bookmark::setUrl(QString url) {
    if(url != mUrl) {
        mUrl = url;
        Q_EMIT urlChanged();
    }
}

QString Bookmark::title() const {
    return mTitle;
}

void Bookmark::setTitle(QString title) {
    if(title != mTitle) {
        mTitle = title;
        Q_EMIT titleChanged();
    }
}

QString Bookmark::favIcon() const {
    return mFavIcon;
}

void Bookmark::setFavIcon(QString favicon) {
    if(favicon != mFavIcon) {
        mFavIcon = favicon;
        Q_EMIT faviconChanged();
    }
}
