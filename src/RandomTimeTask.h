#include <Arduino.h>
#include <Task.h>

class RandomTimeTask : public Task
{
    public:
        typedef void (*TaskCallbackFunction)(void);

        RandomTimeTask(uint32_t minimumTime, uint32_t maximumTime);
        unsigned long remainingTime();
        unsigned long remainingTimeInSeconds();
        void setCallback(TaskCallbackFunction callback);

    private:
        TaskCallbackFunction _callback;
        uint32_t _minimumTime;
        uint32_t _maximumTime;
        
        virtual bool OnStart();
        virtual void OnUpdate(uint32_t deltaTime);
};