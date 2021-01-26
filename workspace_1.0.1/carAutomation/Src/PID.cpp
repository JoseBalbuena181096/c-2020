#include<iostream>
template<class T>
class PID{
    private:
        //CONSTANS PID
        T kp;
        T ki;
        T kd;
        T error;
        T error_last;
        T error_sum;
        T *set_point;
        T *input;
        T *output;
        T output_min,output_max;
        T sample_time;
        bool mode;
        bool direction;
        uint8_t unit_time;
    public:
        #define ON 1
        #define OFF 0
        #define DIRECT 1
        #define REVERSE 0
        #define SECONDS 0
        #define MILLISEC 1
        #define MICROSEC 2
        PID();
        PID(T *input,T *output,T *set_point);
        PID(T *input,T *output,T *set_point,T kp,T ki, T kd);
        PID(T *input,T *output,T *set_point,T kp,T ki, T kd,bool direction);
        bool compute();
        void setTunings(T kp,T ki, T kd,bool direction);
        void setTunings(T kp,T ki, T kd);
        void setSampleTime(T sample_time,int unit_time);
        void setSampleTime(T sample_time);
        void setOutputLimits(T output_min,T output_max);
        void setMode(bool mode);
        void setDirection(bool direction);
        T getKp()const;
        T getKi()const;
        T getKd()const;
        T getSampleTime()const;
        T getSetpoint()const;
};
template<class T>
PID<T>::PID(){
    this->output = NULL;
    this->input = NULL;
    this->set_point = NULL;
    setMode(ON);
    setDirection(DIRECT);
    setOutputLimits(static_cast<T>(0),static_cast<T>(255));
    setSampleTime(1,SECONDS);
    setTunings(static_cast<T>(0),static_cast<T>(0),static_cast<T>(0));
    error = static_cast<T>(0);
    error_last = static_cast<T>(0);
    error_sum = static_cast<T>(0);
}
template<class T>
PID<T>::PID(T *input,T *output,T *set_point,T kp,T ki, T kd,bool direction){
    this->output = output;
    this->input = input;
    this->set_point = set_point;
    setMode(ON);
    setDirection(direction);
    setOutputLimits(static_cast<T>(0),static_cast<T>(255));
    setSampleTime(1,SECONDS);
    setTunings(kp,ki,kd);
    error = static_cast<T>(0);
    error_last = static_cast<T>(0);
    error_sum = static_cast<T>(0);
}
template<class T>
PID<T>::PID(T *input,T *output,T *set_point){
    this->output = output;
    this->input = input;
    this->set_point = set_point;
    setMode(ON);
    setDirection(DIRECT);
    setOutputLimits(static_cast<T>(0),static_cast<T>(255));
    setSampleTime(1,SECONDS);
    kp=ki=kd = static_cast<T>(0);
    error = static_cast<T>(0);
    error_last = static_cast<T>(0);
    error_sum = static_cast<T>(0);
}
template<class T>
PID<T>::PID(T *input,T *output,T *set_point,T kp,T ki, T kd){
    this->output = output;
    this->input = input;
    this->set_point = set_point;
    setMode(ON);
    setDirection(DIRECT);
    setOutputLimits(static_cast<T>(0),static_cast<T>(255));
    setSampleTime(1,SECONDS);
    setTunings(kp,ki,kd);
    error = static_cast<T>(0);
    error_last = static_cast<T>(0);
    error_sum = static_cast<T>(0);
}
template<class T>
bool PID<T>::compute(){
    //if(!mode) return false;
    error =  (*set_point) - (*input);
    error_sum += ki*error;
    *output = kp*error + kd*(error-error_last) + error_sum;
    error_last = error;
    if(*output > output_max)
        *output = output_max;
    else if(*output< output_min)
        *output = output_min;
    return true;
}
template<class T>
void PID<T>::setTunings(T Kp,T Ki,T Kd){
    if (Kp<0 || Ki<0 || Kd<0) return;
    T sample_time_secS = 0;
    if(unit_time== SECONDS)
        sample_time_secS = sample_time;
    else if(unit_time==MILLISEC)
        sample_time_secS = sample_time/static_cast<T>(1000);
    else if(unit_time==MICROSEC)
        sample_time_secS = sample_time/static_cast<T>(1000000);
    this->kp = Kp;
    this->ki = Ki * sample_time_secS;
    this->kd = Kd / sample_time_secS ;
    if(direction == REVERSE){
        this->kp = (0-kp);
        this->ki = (0-ki);
        this->kd = (0-kd);
   }
}
template<class T>
void PID<T>::setMode(bool mode){
    this->mode = mode;
}
template<class T>
void PID<T>::setDirection(bool direction){
    this->direction = direction;
}
template<class T>
void PID<T>::setSampleTime(T sample_time,int unit_time){
    T ratio;
    if(unit_time>=SECONDS && unit_time<=MICROSEC)
        this->unit_time = unit_time;
    if(sample_time>0){
        ratio  = sample_time/this->sample_time;
        ki *= ratio;
        kd /= ratio;
        this->sample_time = sample_time;
    }
}
template<class T>
void PID<T>::setOutputLimits(T output_min,T output_max){
    if(output_min>output_max)
        return;
    this->output_min = output_min;
    this->output_max = output_max;
}
template<class T>
T PID<T>::getKp()const{
    return this->kp;
}
template<class T>
T PID<T>::getKi()const{
    return this->ki;
}
template<class T>
T PID<T>::getKd()const{
    return this->kd;
}
template<class T>
T PID<T>::getSampleTime()const{
    return this->sample_time;
}
template<class T>
T PID<T>::getSetpoint()const{
    return this->set_point;
}
