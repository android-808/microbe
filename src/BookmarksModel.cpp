#include "BookmarksModel.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDesktopServices>
#include <QVariant>
#include <QVariantMap>
#include <qjson/parser.h>
#include <qjson/serializer.h>

BookmarksModel::BookmarksModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QString settingsLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/bookmarks.json";
    QScopedPointer<QFile> file(new QFile(settingsLocation));

    qDebug() << "Bookmark path: " + settingsLocation;

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open bookmarks " + settingsLocation;

        file.reset(new QFile(QString("/home/user/bookmarks.json")));
        if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Unable to open bookmarks defaults";
            return;
        }
    }

    QJson::Parser parser;
    bool ok;
    QVariantList result = parser.parse(file->readAll(), &ok).toList();
    if(ok)
    {
        Q_FOREACH(QVariant record, result)
        {
            QVariantMap map = record.toMap();
            QString url = map["url"].toString();
            Bookmark* m = new Bookmark(url,
                                       map["title"].toString(),
                                       map["favicon"].toString());
            bookmarks.insert(url, m);
            bookmarkUrls.append(url);
            qDebug() << "Url:" << url;
        }
    }
    else
    {
        qDebug() << "Parser error";
    }
    Q_EMIT countChanged();
    file->close();
}

QHash<int, QByteArray> BookmarksModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UrlRole] = "url";
    roles[TitleRole] = "title";
    roles[FaviconRole] = "favicon";
    return roles;
}

void BookmarksModel::addBookmark(const QString& url, const QString& title, const QString& favicon) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    bookmarks.insert(url, new Bookmark(url, title, favicon));
    bookmarkUrls.append(url);
    endInsertRows();

    Q_EMIT countChanged();
    save();
}

void BookmarksModel::removeBookmark(const QString& url) {
    if(!contains(url)) {
        return;
    }

    if (bookmarks.contains(url)) {
        int index = bookmarkUrls.indexOf(url);
        beginRemoveRows(QModelIndex(), index, index);
        Bookmark* bookmark = bookmarks.take(url);
        delete bookmark;
        bookmarkUrls.removeAt(index);
        endRemoveRows();

        Q_EMIT countChanged();
        save();
    }
}

int BookmarksModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent)
    return bookmarks.count();
}

QVariant BookmarksModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > bookmarkUrls.count())
        return QVariant();

    const Bookmark * bookmark = bookmarks.value(bookmarkUrls[index.row()]);
    if (role == UrlRole) {
        return bookmark->url();
    } else if (role == TitleRole) {
        return bookmark->title();
    } else if (role == FaviconRole) {
        return bookmark->favIcon();
    } else if (role == Qt::DisplayRole) {
        return bookmark->title();
    }
    
    return QVariant();
}

bool BookmarksModel::contains(const QString& url) const {
    return bookmarks.contains(url);
}

void BookmarksModel::save()
{
    QString settingsLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QDir dir(settingsLocation);
    if(!dir.exists()) {
        if(!dir.mkpath(settingsLocation)) {
            qDebug() << "Can't create directory "+ settingsLocation;
            return;
        }
    }
    QString path = settingsLocation + "/bookmarks.json";
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Can't create file "+ path;
        return;
    }
    
    QTextStream out(&file);
    QVariantList items;
    out.setCodec("UTF-8");

    QMapIterator<QString, Bookmark*> bookmarkIterator(bookmarks);
    while (bookmarkIterator.hasNext()) {
        bookmarkIterator.next();
        QVariantMap title;
        Bookmark* bookmark = bookmarkIterator.value();
        title.insert("url", bookmark->url());
        title.insert("title", bookmark->title());
        title.insert("favicon", bookmark->favIcon());
        items.append(title);
    }

    // Using serializer.serialize(items, &out, &ok), output causes error, probably codec related. 
    
    QJson::Serializer serializer;
    QByteArray json = serializer.serialize(items);
    if(!json.isEmpty())
    {
        qDebug() << json;
    }
    else
    {
        qDebug() << "Serialize error";
    }

    out << json;
    file.close();
}
