#include <chrono>

class FrameTimer {
    private:
        std::chrono::time_point<std::chrono::system_clock> mStartTime;
        std::chrono::time_point<std::chrono::system_clock> mEndTime;
        std::chrono::time_point<std::chrono::system_clock> mLastFrameTime;
        bool mRunning = true;

    public: 
        FrameTimer() {
            Start();
        }

        void Start() {
            mStartTime = std::chrono::system_clock::now();
            mLastFrameTime = mStartTime;
            mRunning = true;
        }

        void Stop() {
            mEndTime = std::chrono::system_clock::now();
            mRunning = false;
        }
        
        void Frame() {
            if (mRunning) {
                mLastFrameTime = std::chrono::system_clock::now();
            }
        }

        double GetFrameElapsed(bool isMilli = false) {
            std::chrono::time_point<std::chrono::system_clock> endTime;
            if(mRunning) {
                endTime = std::chrono::system_clock::now();
            } else {
                endTime = mEndTime;
            }
            double dt = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - mLastFrameTime).count();
            return isMilli ? dt : dt / 1000;
        }
    
        double GetTotalElapsed(bool isMilli = false) {
            std::chrono::time_point<std::chrono::system_clock> endTime;
            if(mRunning) {
                endTime = std::chrono::system_clock::now();
            } else {
                endTime = mEndTime;
            }
            double dt = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - mStartTime).count();
            return isMilli ? dt : dt / 1000;
        }
};