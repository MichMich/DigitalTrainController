#include "RandomTimeTask.h"

    RandomTimeTask::RandomTimeTask(uint32_t minimumTime, uint32_t maximumTime) : Task(random(minimumTime, maximumTime))
    { 
        _minimumTime = minimumTime;
        _maximumTime = maximumTime;
    };

    unsigned long RandomTimeTask::remainingTime() 
    {
        return _remainingTime;
    }
    unsigned long RandomTimeTask::remainingTimeInSeconds() 
    {
        return 1 + ((_remainingTime - 1) / 1000); ;
    }

    void RandomTimeTask::setCallback(TaskCallbackFunction callback) {
        _callback = callback;
    }

    bool RandomTimeTask::OnStart() // optional
    {
        setTimeInterval(random(_minimumTime, _maximumTime));
        return true;
    }

    void RandomTimeTask::OnUpdate(uint32_t deltaTime)
    {
        _callback();
        setTimeInterval(random(_minimumTime, _maximumTime));
    }