
#include "mraa.hpp"
#include "i2c.hpp"
#include "aio.hpp"
#include "gpio.hpp"
#include <iostream>
#include <unistd.h>

#define I2C_Bus  6
#define I2C_Address 0x4c

int get_jerk(mraa::I2c* i2c, int axis)
{
	int x_value=0,x_accel=0,negated=0, a=0, b=0;
	int jerk = 0;


        //Read the Current Register Value
    	x_value = i2c->readReg(0x02);
    	//Mask the appropriate bits to determine alert
    	negated = x_value & 0b00100000;
    	x_accel = x_value & 0b00011111;
    	//Negate if value is negative
		if(!negated){x_accel = x_accel + 30;}
		a = x_accel;
		usleep(2000);
		x_value = i2c->readReg(axis);
    	negated = x_value & 0b00100000;
    	x_accel = x_value & 0b00011111;
    	//Negate if value is negative
		if(!negated){x_accel = x_accel + 30;}
		b = x_accel;

		jerk = a-b;



	return(jerk);
}

int main()
{
  int jerk_x = 0, jerk_y = 0, jerk_z= 0,jerk_Threshold_x = 55, jerk_Threshold_y = 50, jerk_Threshold_z = 50;

	mraa::I2c* i2c;
    mraa::Gpio* buzzer;
    mraa::Gpio* power;
    mraa::Gpio* gnd;
    i2c = new mraa::I2c(I2C_Bus,false);
	buzzer = new mraa::Gpio(8);
	buzzer->dir(mraa::DIR_OUT);
	power = new mraa::Gpio(12);
	power->dir(mraa::DIR_OUT);
	power->write(1);
	gnd = new mraa::Gpio(13);
	gnd->dir(mraa::DIR_OUT);
	gnd->write(0);
	i2c->address(0x4c);
	i2c->writeReg(0x07,0x01);

    int count=0;
    while(1){
    	count++;

    	jerk_x = get_jerk(i2c,0x00);
    	jerk_y = get_jerk(i2c,0x01);
    	jerk_z = get_jerk(i2c,0x02);
    if(jerk_x > jerk_Threshold_x)
    {
    	buzzer->write(1);
    	usleep(50000);
    	buzzer->write(0);
    	std::clog << jerk_x;
    	std::clog << "X";
    	//usleep(5000);
    }
    if(jerk_y > jerk_Threshold_y)
    {
    	buzzer->write(1);
    	usleep(50000);
    	buzzer->write(0);
    	std::clog << jerk_y;
    	std::clog << "Y";
    	//usleep(5000);
    }
    if(jerk_z > jerk_Threshold_z)
    {
    	buzzer->write(1);
    	usleep(50000);
    	buzzer->write(0);
    	std::clog << jerk_z;
    	std::clog << "Z";
    	//usleep(5000);
    }




}


	return mraa::SUCCESS;
}
