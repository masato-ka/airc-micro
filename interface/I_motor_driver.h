class MotorDriver {
public:
    virtual void initialize(int pin) = 0;
    virtual void cmd(float steering, float throttle) = 0;
    virtual void lock() = 0;
    virtual void stop() = 0;
};




