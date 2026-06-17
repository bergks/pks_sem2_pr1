#ifndef ARRAYMODEL_H
#define ARRAYMODEL_H

#include <QObject>
#include <QVector>

class ArrayModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<int> data READ data NOTIFY dataChanged)
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)

public:
    explicit ArrayModel(QObject* parent = nullptr);

    QVector<int> data() const { return m_data; }
    int size() const { return m_data.size(); }

    Q_INVOKABLE void generateRandom(int size);
    Q_INVOKABLE void setArray(const QVector<int>& arr);
    Q_INVOKABLE void reset();

signals:
    void dataChanged();
    void sizeChanged();

private:
    QVector<int> m_data;
    QVector<int> m_originalData;
};

#endif
