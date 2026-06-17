#ifndef SORTINGCONTROLLER_H
#define SORTINGCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <memory>
#include "SortingStrategy.h"
#include "ArrayModel.h"

class SortingController : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(bool isPaused READ isPaused NOTIFY isPausedChanged)
    Q_PROPERTY(int currentStep READ currentStep NOTIFY currentStepChanged)
    Q_PROPERTY(int totalSteps READ totalSteps NOTIFY totalStepsChanged)
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(QString currentAlgorithm READ currentAlgorithm NOTIFY currentAlgorithmChanged)
    Q_PROPERTY(QString stepDescription READ stepDescription NOTIFY stepDescriptionChanged)
    Q_PROPERTY(QVector<int> activeIndexes READ activeIndexes NOTIFY activeIndexesChanged)
    Q_PROPERTY(int pivotIndex READ pivotIndex NOTIFY activeIndexesChanged)
    Q_PROPERTY(QString algorithmDescription READ algorithmDescription NOTIFY algorithmDescriptionChanged)
    Q_PROPERTY(QString complexity READ complexity NOTIFY complexityChanged)

public:
    static SortingController& instance();
    explicit SortingController(ArrayModel* model, QObject* parent = nullptr);
    ~SortingController();

    bool isRunning() const { return m_isRunning; }
    bool isPaused() const { return m_isPaused; }
    int currentStep() const { return m_currentStep; }
    int totalSteps() const { return m_steps.size(); }
    int speed() const { return m_speed; }
    int pivotIndex() const { return m_pivotIndex; }

    QString currentAlgorithm() const { return m_algorithmName; }
    QString stepDescription() const { return m_stepDescription; }
    QVector<int> activeIndexes() const { return m_activeIndexes; }
    QString complexity() const { return m_complexity; }
    QString algorithmDescription() const { return m_algorithmDescription; }

    Q_INVOKABLE void setSpeed(int speed);

    Q_INVOKABLE void selectAlgorithm(const QString& name);
    Q_INVOKABLE void start();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void nextStep();
    Q_INVOKABLE void previousStep();

signals:
    void isRunningChanged();
    void isPausedChanged();
    void currentStepChanged();
    void totalStepsChanged();
    void speedChanged();
    void currentAlgorithmChanged();
    void stepDescriptionChanged();
    void activeIndexesChanged();
    void sortingFinished();
    void algorithmDescriptionChanged();
    void complexityChanged();

private slots:
    void onTimerTick();

private:
    static SortingController* s_instance;
    ArrayModel* m_model;
    std::unique_ptr<SortingStrategy> m_strategy;
    QVector<Step> m_steps;
    QTimer m_timer;
    int m_currentStep = 0;
    int m_speed = 500;
    int m_pivotIndex = -1;
    bool m_isRunning = false;
    bool m_isPaused = false;
    QString m_algorithmName;
    QString m_stepDescription;
    QVector<int> m_activeIndexes;
    QString m_algorithmDescription;
    QString m_complexity;

    void executeStep(const Step& step);
    void resetToStart();
};

#endif
