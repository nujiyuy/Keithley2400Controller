#include <userint.h>
#include <utility.h>
#include <gpib.h>
#include <ansi_c.h>
#include "KE2400.h"

void ke2400_initialize(int address)	{
	ke2400.handle=ibdev (0, address, NO_SAD, T100ms, 1, 0);
	if (ke2400.handle<0)
	MessagePopup("Error","Meter not found.");
}

void ke2400_destory() {ibonl(ke2400.handle,0);}

void GPIB_WRITE(int handle, const char formatString[], ...)
{
	char str[100];
	va_list args;
	memset(str,'\0', sizeof(str));
	va_start(args, formatString);
	vsprintf(str,formatString,args);
	va_end(args);
	ibwrt (handle, str, strlen(str));
}
void GPIB_READ(int handle, char *str)
{
	memset(str,'\0', 150);
	ibrd(handle, str, 150); 
}

void ke2400_setup_voltage_source(double voltage_range,double current_limit)
{
	GPIB_WRITE(ke2400.handle, ":SENS:RES:MODE MAN\n");
	GPIB_WRITE(ke2400.handle, ":SENS:CURR:RANG:AUTO 0\n");
	GPIB_WRITE(ke2400.handle, ":SENS:FUNC:ON:ALL\n");
	GPIB_WRITE(ke2400.handle, ":SOUR:FUNC VOLT\n");
	ke2400_set_output_voltage_range(voltage_range);
	ke2400_set_current_compliance(current_limit);
	GPIB_WRITE(ke2400.handle, ":OUTP:STAT ON\n");
}

void ke2400_setup_current_source(double current_range,double voltage_limit)
{
	GPIB_WRITE(ke2400.handle, ":SENS:RES:MODE MAN\n"); 
	GPIB_WRITE(ke2400.handle, ":SENS:VOLT:RANG:AUTO 0\n"); 
	GPIB_WRITE(ke2400.handle, ":SENS:FUNC:ON:ALL\n"); 
	GPIB_WRITE(ke2400.handle, ":SOUR:FUNC CURR\n");
	ke2400_set_output_current_range(current_range);
	ke2400_set_voltage_compliance(voltage_limit);
	GPIB_WRITE(ke2400.handle, ":OUTP:STAT ON\n");
}

void ke2400_set_output_voltage(double voltage)	{	GPIB_WRITE(ke2400.handle, ":SOUR:VOLT:LEV:IMM:AMPL %e\n",voltage);}
void ke2400_set_output_current(double current)	{	GPIB_WRITE(ke2400.handle, ":SOUR:CURR:LEV:IMM:AMPL %e\n",current);}
void ke2400_set_output_voltage_range(double voltage_range)	{	GPIB_WRITE(ke2400.handle, ":SOUR:VOLT:RANG %e\n",voltage_range);}
void ke2400_set_output_current_range(double current_range)	{	GPIB_WRITE(ke2400.handle, ":SOUR:CURR:RANG %e\n",current_range);}
void ke2400_set_current_compliance(double current_limit)	{GPIB_WRITE(ke2400.handle,":SENS:CURR:PROT %e\n",current_limit);}
void ke2400_set_voltage_compliance(double voltage_limit)	{GPIB_WRITE(ke2400.handle,":SENS:VOLT:PROT %e\n",voltage_limit);}

double* ke2400_read(void)
{
	char buf[150]="";
	char var[14]="";
	double* output = (double*)malloc(3 * sizeof(double));
	memset(var, '\0', sizeof(var)); 
	GPIB_WRITE(ke2400.handle,":READ?\n"); 
	GPIB_READ(ke2400.handle,buf);
	memcpy(var,buf,13);
	output[0]=atof(var);
	memcpy(var,buf+14,13);
	output[1]=atof(var);
	memcpy(var,buf+28,13);
	output[2]=atof(var);
	return output;
}

