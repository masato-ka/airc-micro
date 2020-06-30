#include "../interface/I_motor_driver.h"

class DRV8835 : public IMotorDriver {
    public :
        void init(int pin);
        void cmd(float steering, float throttle);
        void lock();
        void stop();
}


void DRV8835::init(int pin){
// Initialize motor driver


}


void DRV8835::cmd(float steering, float throttle){

}


