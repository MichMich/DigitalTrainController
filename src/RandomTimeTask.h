#include <Arduino.h>
#include <Task.h>

class RandomTimeTask : public Task
{
    public:
        typedef void (*TaskCallbackFunction)(void);

        RandomTimeTask(uint32_t time);
        unsigned long remainingTime();
        unsigned long remainingTimeInSeconds();
        void setCallback(TaskCallbackFunction callback);
        void setMinMaxTime(uint32_t minimumTime, uint32_t maximumTime);

        uint32_t _minimumTime;
        uint32_t _maximumTime;

    private:
        TaskCallbackFunction _callback;
        
        
        virtual bool OnStart();
        virtual void OnUpdate(uint32_t deltaTime);
};