#include "BookmarkDialog.h"
#include "BookmarksModel.h"
#include "Session.h"

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMaemo5ListPickSelector>
#include <QMaemo5ValueButton>
#include <QPushButton>
#include <QStandardItemModel>

#include <QDebug>

BookmarkDialog::BookmarkDialog(const QString& url, const QString& title, QWidget *parent) :
    AbstractDialog(parent),
    mButtonBox(new QDialogButtonBox(QDialogButtonBox::Save, Qt::Vertical, this)),
    mTitle(new QLineEdit(title)),
    mUrl(new QLineEdit(url)),
    mGrid(new QGridLayout(this))
{
    // Window Setup
    this->setWindowTitle(tr("Add Bookmark"));
    
    mGrid->addWidget(new QLabel("Name"), 0, 0, Qt::AlignRight);
    mGrid->addWidget(mTitle, 0, 1);
    
    mGrid->addWidget(new QLabel("Address"), 1, 0, Qt::AlignRight);
    mGrid->addWidget(mUrl, 1, 1);
    
    connect(mButtonBox, SIGNAL(accepted()), this, SLOT(accept()));

    this->onOrientationChanged();
}

void BookmarkDialog::setLandscapeLayout() {
    this->setMaximumHeight(260);
    mButtonBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    mGrid->removeWidget(mButtonBox);
    mGrid->addWidget(mButtonBox, 1, 2, Qt::AlignBottom);
}

void BookmarkDialog::setPortraitLayout() {
    this->setMaximumHeight(400);
    mButtonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    mGrid->removeWidget(mButtonBox);
    mGrid->addWidget(mButtonBox, 2, 0, 1, 2);
}

void BookmarkDialog::accept()
{
    Session::instance()->bookmarks()->addBookmark(mUrl->text(), mTitle->text(), QString(""));
    
    QDialog::accept();
}
