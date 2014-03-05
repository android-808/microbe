#ifndef BOOKMARKDIALOG_H
#define BOOKMARKDIALOG_H

#include "dialogues/AbstractDialog.h"
#include "Bookmark.h"

class QDialogButtonBox;
class QGridLayout;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QMaemo5ValueButton;

class BookmarkDialog : public AbstractDialog
{
    Q_OBJECT
public:
    explicit BookmarkDialog(const QString& url, const QString& title, QWidget *parent = 0);

private:
    void setLandscapeLayout();
    void setPortraitLayout();
    void accept();

protected:
    QDialogButtonBox* mButtonBox;
    QLineEdit* mTitle;
    QLineEdit* mUrl;
    QGridLayout* mGrid;
};

#endif // BOOKMARKDIALOG_H
