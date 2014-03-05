#include "SelectionModels.h"
#include <QFile>
#include <QVariantMap>
#include <qjson/parser.h>

SelectionModel::SelectionModel(QObject *parent) :
    QStandardItemModel(parent),
    m_textAlignment(Qt::AlignCenter)
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "name";
    roles[Qt::UserRole + 1] = "value";
    this->setRoleNames(roles);
}

QString SelectionModel::name(int row) const {
    return this->data(this->index(row, 0), Qt::DisplayRole).toString();
}

QVariant SelectionModel::value(int row) const {
    return this->data(this->index(row, 0), Qt::UserRole + 1);
}

QVariantMap SelectionModel::get(int row) const {
    QVariantMap item;
    item.insert("name", this->name(row));
    item.insert("value", this->value(row));

    return item;
}

void SelectionModel::addItem(const QString &name, const QVariant &value) {
    QStandardItem *item = new QStandardItem(name);
    item->setData(value);
    item->setTextAlignment(this->textAlignment());
    item->setEditable(false);
    this->appendRow(item);
}

LanguageModel::LanguageModel(QObject *parent) :
    SelectionModel(parent)
{
    this->addItem(tr("All languages"), "all");
    this->addItem(tr("English"), "en");
    
    Q_EMIT countChanged(this->rowCount());
}
