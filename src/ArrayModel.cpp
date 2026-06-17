#include "ArrayModel.h"
#include <algorithm>
#include <random>

ArrayModel::ArrayModel(QObject* parent) : QObject(parent) {}

void ArrayModel::generateRandom(int size) {
    size = std::clamp(size, 5, 30);
    m_data.clear();
    m_data.resize(size);

    QVector<int> pool;
    for (int i = 1; i <= size * 2; ++i) {
        pool.append(i);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(pool.begin(), pool.end(), gen);

    for (int i = 0; i < size; ++i) {
        m_data[i] = pool[i];
    }

    m_originalData = m_data;
    emit dataChanged();
    emit sizeChanged();
}

void ArrayModel::reset() {
    m_data = m_originalData;
    emit dataChanged();
}
void ArrayModel::setArray(const QVector<int>& arr) {
    m_data = arr;
    emit dataChanged();
}
