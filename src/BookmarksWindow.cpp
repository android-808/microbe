#include "BookmarksWindow.h"
#include "BookmarksModel.h"
#include "NavigationToolBar.h"
#include "Session.h"
#include "Settings.h"
#include <QListView>

BookmarksWindow::BookmarksWindow(QWidget *parent) :
    StackedWindow(parent),
    mListView(new QListView(this)),
    mModel(Session::instance()->bookmarks())
{    
    this->setWindowTitle("Bookmarks");

    this->setCentralWidget(mListView);
    mListView->setModel(mModel);

    this->onOrientationChanged();
}

void BookmarksWindow::setLandscapeLayout() {
}

void BookmarksWindow::setPortraitLayout() {
}

