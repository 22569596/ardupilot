#include "Rover.h"

float rudderAngle;
int32_t heading;
int32_t currentHeading;
bool zigzag;
float desired_throttle;
float desired_steering; 
float desired_speed;

bool ModeSimple::_enter()
{
    // Send message to GCS to show zigzag has been initialized
    gcs().send_text(MAV_SEVERITY_INFO, "Initialized ZIGZAG");

    // Set rudder angle
    rudderAngle = 3000.0f;
    heading  = ahrs.yaw_sensor;
    heading = heading/100;



    // init zigzag flag false is left 
    zigzag = true;
    gcs().send_text(MAV_SEVERITY_INFO, "Current Heading: %1.0d",(int)heading);

    // Can be any speed due to no velocity control of sailboat
    desired_speed = 1.0f;
    
    return true;
}


void ModeSimple::init_heading()
{
    _initial_heading_cd = ahrs.yaw_sensor;
    
    
}

void ModeSimple::update()
{

    gcs().send_text(MAV_SEVERITY_INFO, "Rudder Angle set to %1.0F", rudderAngle);

    if(zigzag==true)
    {
        g2.motors.set_steering(rudderAngle, true);
    }else
    {
        g2.motors.set_steering(-rudderAngle, true);
    }
    gcs().send_text(MAV_SEVERITY_INFO, "Start Heading: %1.0d",(int)heading);
    gcs().send_text(MAV_SEVERITY_INFO, "Current Heading: %1.0d",(int)ahrs.yaw_sensor/100);

    // convert yaw to -180 - 180
    if ((heading>180)){
        heading = heading - 360;
    }
    currentHeading = ahrs.yaw_sensor/100;
    if (currentHeading>180){
        currentHeading = currentHeading - 360;
    }
    if((currentHeading-heading) > 20){
        zigzag = false;
    }else if ((currentHeading-heading) < -20){
        zigzag = true;
    }

    //calc_throttle(desired_speed, true);
}

void ModeSimple::_exit()
{
    gcs().send_text(MAV_SEVERITY_INFO, "ZIGZAG Stopped");
}