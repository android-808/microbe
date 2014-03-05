#ifndef SELECTIONMODELS_H
#define SELECTIONMODELS_H

#include "Enums.h"
#include <QStandardItemModel>

class SelectionModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit SelectionModel(QObject *parent = 0);
    void addItem(const QString &name, const QVariant &value);
    Q_INVOKABLE QString name(int row) const;
    Q_INVOKABLE QVariant value(int row) const;
    Q_INVOKABLE QVariantMap get(int row) const;
    Q_INVOKABLE inline Qt::Alignment textAlignment() const { return m_textAlignment; }
    Q_INVOKABLE inline void setTextAlignment(Qt::Alignment alignment) { m_textAlignment = alignment; }

private:
    Qt::Alignment m_textAlignment;
};

class LanguageModel : public SelectionModel
{
    Q_OBJECT

    Q_PROPERTY(int count
               READ rowCount
               NOTIFY countChanged)

public:
    explicit LanguageModel(QObject *parent = 0);

Q_SIGNALS:
    void countChanged(int count);
};


#endif // SELECTIONMODELS_H
