class IRangeSensorDriver {
public :
    virtual void initialize(int pin) = 0;
    virtual float getData() = 0;
};