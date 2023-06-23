/* Controls Keithly2400 sourcemeter */
#include <gpib.h>
#include <ansi_c.h>


struct {
	int handle;
} ke2400;

void GPIB_WRITE(int handle, const char formatString[], ...);
void GPIB_READ(int handle, char *str);

void ke2400_initialize(int address);
void ke2400_destory();
void ke2400_setup_voltage_source(double voltage_range,double current_limit);
void ke2400_setup_current_source(double current_range,double voltage_limit);
void ke2400_set_output_voltage_range(double voltage_range);
void ke2400_set_output_current_range(double current_range);
void ke2400_set_current_compliance(double current_limit);
void ke2400_set_voltage_compliance(double voltage_limit);
void ke2400_set_output_voltage(double voltage);
void ke2400_set_output_current(double current);   
double* ke2400_read(void);
