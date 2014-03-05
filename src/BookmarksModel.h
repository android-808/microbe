#ifndef FAVORITEMODEL_H
#define FAVORITEMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QMap>
#include "Bookmark.h"

class BookmarksModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BookmarksModel(QObject *parent = 0);
    
    enum BookmarkRoles {
       UrlRole = Qt::UserRole + 1,
       TitleRole,
       FaviconRole
    };

    Q_INVOKABLE void addBookmark(const QString& url, const QString& title, const QString& favicon);
    Q_INVOKABLE void removeBookmark(const QString& url);
    Q_INVOKABLE bool contains(const QString& url) const;
    
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    
Q_SIGNALS:
    void countChanged();

private:
    void save();

    QMap<QString, Bookmark*> bookmarks;
    QStringList bookmarkUrls;
};

#endif // FAVORITEMODEL_H
