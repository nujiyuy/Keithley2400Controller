//==============================================================================
//
// Title:       Keithley2400Controller
// Purpose:     A short description of the application.
//
// Created on:  2023-06-20 at 18:54:34 by Yijun.
// Copyright:   . All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "asynctmr.h"
#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include "Keithley2400Controller.h"
#include "KE2400.h"
#include "toolbox.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle;

int gpib_address;
int output_type=1;
int connected=0;
int measure_state=0;

double v_output;
double i_output;
double v_range;
double i_range;
double i_comp;
double v_comp;
double last_v_output;
double last_i_output;

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions
int CVICALLBACK update (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2)
{
	double *measure;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			if (connected==1)
			{
				if (output_type==1)
				{
					GetCtrlVal(panelHandle,PANEL_V_OUTPUT,&v_output);
					if(v_output!=last_v_output)
					{
						ke2400_set_output_voltage(v_output);
						last_v_output=v_output;
					}
				}
				if (output_type==2)
				{
					GetCtrlVal(panelHandle,PANEL_I_OUTPUT,&i_output);
					if(i_output!=last_i_output)
					{
						ke2400_set_output_current(i_output);
						last_i_output=i_output;
					}
				}
				if(measure_state==1)
				{
					measure=ke2400_read();
					SetCtrlVal(panelHandle, PANEL_V_MEAS,measure[0]);
					SetCtrlVal(panelHandle, PANEL_I_MEAS,measure[1]);
					SetCtrlVal(panelHandle, PANEL_R_MEAS,measure[2]);
				}
			}
			break;
	}
	return 0;
} 


/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
    int error = 0;
    int timerId;
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "Keithley2400Controller.uir", PANEL));
	
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
	timerId=NewAsyncTimer (0.1, -1, 1, update, NULL);
    errChk (RunUserInterface ());

Error:
    /* clean up */
    DiscardPanel (panelHandle);
    return 0;
}
//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

int CVICALLBACK connect (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int state;
	switch (event)
	{
		case EVENT_COMMIT:
			if (connected==0)
			{
				connected=1;
				GetCtrlVal(panel,PANEL_GPIB_ADDR,&gpib_address);
				GetCtrlVal(panel,PANEL_OUTPUT_TYPE,&output_type);
				ke2400_initialize(gpib_address);
				if(output_type==1)
				{
					GetCtrlVal(panel,PANEL_V_RANGE,&v_range);
					GetCtrlVal(panel,PANEL_I_COMP,&i_comp);
					GetCtrlVal(panel,PANEL_V_OUTPUT,&v_output);
					ke2400_setup_voltage_source(v_range,i_comp);
					ke2400_set_output_voltage(v_output);
				}
				if(output_type==2)
				{
					GetCtrlVal(panel,PANEL_I_RANGE,&i_range);
					GetCtrlVal(panel,PANEL_V_COMP,&v_comp);
					GetCtrlVal(panel,PANEL_I_OUTPUT,&i_output);
					ke2400_setup_current_source(i_range,v_comp);
					ke2400_set_output_current(i_output);
				}
				SetCtrlAttribute(panel,PANEL_CONNECT,ATTR_LABEL_TEXT, "Disconnect");
				SetCtrlAttribute(panel,PANEL_GPIB_ADDR,ATTR_DIMMED,1);
				SetCtrlAttribute(panel,PANEL_OUTPUT_TYPE,ATTR_DIMMED,1);
				
			}
			else
			{
				connected=0;
				SetCtrlAttribute(panel,PANEL_CONNECT,ATTR_LABEL_TEXT, "Connect");
				SetCtrlAttribute(panel,PANEL_GPIB_ADDR,ATTR_DIMMED,0);
				SetCtrlAttribute(panel,PANEL_OUTPUT_TYPE,ATTR_DIMMED,0);
				
			}
			break;
	}
	return 0;
}

int CVICALLBACK enable_dataskt_v_output (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int state;
	DSHandle dsHandle;
	char url[100];
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,PANEL_ENABLE_DTSKT_V_OUTPUT,&state); 
			if (state==1)
			{
				SetCtrlAttribute(panel,PANEL_DTSKT_V_OUTPUT_URL,ATTR_DIMMED,1);
				SetCtrlAttribute(panel,PANEL_V_OUTPUT,ATTR_CTRL_MODE,VAL_INDICATOR);
				GetCtrlVal(panel,PANEL_DTSKT_V_OUTPUT_URL,url);
				DSBindCtrl (panel, PANEL_V_OUTPUT, url, VAL_DS_READ, &dsHandle, NULL);
			}
			else
			{
				SetCtrlAttribute(panel,PANEL_DTSKT_V_OUTPUT_URL,ATTR_DIMMED,0);
				SetCtrlAttribute(panel,PANEL_V_OUTPUT,ATTR_CTRL_MODE,VAL_HOT);
				DSUnbind (panel, PANEL_V_OUTPUT, NULL);  
			}
			break;
	}
	return 0;
}

int CVICALLBACK enable_dataskt_i_output (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int state;
	DSHandle dsHandle;
	char url[100];
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,PANEL_ENABLE_DTSKT_I_OUTPUT,&state); 
			if (state==1)
			{
				SetCtrlAttribute(panel,PANEL_DTSKT_I_OUTPUT_URL,ATTR_DIMMED,1);
				SetCtrlAttribute(panel,PANEL_I_OUTPUT,ATTR_CTRL_MODE,VAL_INDICATOR);
				GetCtrlVal(panel,PANEL_DTSKT_I_OUTPUT_URL,url);
				DSBindCtrl (panel, PANEL_I_OUTPUT, url, VAL_DS_READ, &dsHandle, NULL);
			}
			else
			{
				SetCtrlAttribute(panel,PANEL_DTSKT_I_OUTPUT_URL,ATTR_DIMMED,0);
				SetCtrlAttribute(panel,PANEL_I_OUTPUT,ATTR_CTRL_MODE,VAL_HOT);
				DSUnbind (panel, PANEL_I_OUTPUT, NULL); 
			}
			break;
	}
	return 0;
}



int CVICALLBACK select_output_type (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,PANEL_OUTPUT_TYPE,&output_type);
			if (output_type==1)
			{
				GetCtrlVal(panel,PANEL_V_OUTPUT,&v_output);
				GetCtrlVal(panel,PANEL_V_RANGE,&v_range);
				GetCtrlVal(panel,PANEL_I_COMP,&i_comp);
			
				SetCtrlAttribute(panel,PANEL_V_OUTPUT,ATTR_VISIBLE,1);
				SetCtrlAttribute(panel,PANEL_V_RANGE,ATTR_VISIBLE,1);
				SetCtrlAttribute(panel,PANEL_I_COMP,ATTR_VISIBLE,1);
				SetCtrlAttribute(panel,PANEL_ENABLE_DTSKT_V_OUTPUT,ATTR_VISIBLE,1);
				SetCtrlAttribute(panel,PANEL_DTSKT_V_OUTPUT_URL,ATTR_VISIBLE,1);
				SetCtrlVal(panel,PANEL_ENABLE_DTSKT_V_OUTPUT,0); 
				SetCtrlAttribute(panel,PANEL_V_OUTPUT,ATTR_CTRL_MODE,VAL_HOT);
			
				SetCtrlAttribute(panel,PANEL_I_OUTPUT,ATTR_VISIBLE,0);
				SetCtrlAttribute(panel,PANEL_I_RANGE,ATTR_VISIBLE,0);
				SetCtrlAttribute(panel,PANEL_V_COMP,ATTR_VISIBLE,0);
				SetCtrlAttribute(panel,PANEL_ENABLE_DTSKT_I_OUTPUT,ATTR_VISIBLE,0);
				SetCtrlAttribute(panel,PANEL_DTSKT_I_OUTPUT_URL,ATTR_VISIBLE,0);
			
				SetCtrlVal(panel,PANEL_ENABLE_DTSKT_I_OUTPUT,0);
				SetCtrlAttribute(panel,PANEL_DTSKT_I_OUTPUT_URL,ATTR_DIMMED,0);
				DSUnbind (panel, PANEL_I_OUTPUT, NULL);
			}
			if (output_type==2)
			{
				GetCtrlVal(panel,PANEL_I_OUTPUT,&i_output);
				GetCtrlVal(panel,PANEL_I_RANGE,&i_range);
				GetCtrlVal(panel,PANEL_V_COMP,&v_comp);
			
				SetCtrlAttribute(panel,PANEL_I_OUTPUT,ATTR_VISIBLE,1);
				SetCtrlAttribute(panel,PANEL_I_RANGE,ATTR_VISIBLE,1);
				SetCtrlAttribute(panel,PANEL_V_COMP,ATTR_VISIBLE,1);
				SetCtrlAttribute(panel,PANEL_ENABLE_DTSKT_I_OUTPUT,ATTR_VISIBLE,1);
				SetCtrlAttribute(panel,PANEL_DTSKT_I_OUTPUT_URL,ATTR_VISIBLE,1);
				SetCtrlVal(panel,PANEL_ENABLE_DTSKT_I_OUTPUT,0);
				SetCtrlAttribute(panel,PANEL_I_OUTPUT,ATTR_CTRL_MODE,VAL_HOT);
			
				SetCtrlAttribute(panel,PANEL_V_OUTPUT,ATTR_VISIBLE,0);
				SetCtrlAttribute(panel,PANEL_V_RANGE,ATTR_VISIBLE,0);
				SetCtrlAttribute(panel,PANEL_I_COMP,ATTR_VISIBLE,0);
				SetCtrlAttribute(panel,PANEL_ENABLE_DTSKT_V_OUTPUT,ATTR_VISIBLE,0);
				SetCtrlAttribute(panel,PANEL_DTSKT_V_OUTPUT_URL,ATTR_VISIBLE,0);
			
				SetCtrlVal(panel,PANEL_ENABLE_DTSKT_V_OUTPUT,0);
				SetCtrlAttribute(panel,PANEL_DTSKT_V_OUTPUT_URL,ATTR_DIMMED,0);
				DSUnbind (panel, PANEL_V_OUTPUT, NULL);
			}
		break;
	}
	return 0;
}

int CVICALLBACK chang_v_range (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (connected)
			{
				GetCtrlVal(panel,PANEL_V_RANGE,&v_range);
				ke2400_set_output_voltage_range(v_range);
			}
			break;
	}
	return 0;
}

int CVICALLBACK change_i_range (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (connected)
			{
				GetCtrlVal(panel,PANEL_I_RANGE,&i_range);
				ke2400_set_output_current_range(i_range);
			}
			break;
	}
	return 0;
}

int CVICALLBACK change_i_compliance (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (connected)
			{
				GetCtrlVal(panel,PANEL_I_COMP,&i_comp);
				ke2400_set_current_compliance(i_comp);
			}
			break;
	}
	return 0;
}

int CVICALLBACK change_v_compliance (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (connected)
			{
				GetCtrlVal(panel,PANEL_V_COMP,&v_comp);
				ke2400_set_voltage_compliance(v_comp);
			}
			break;
	}
	return 0;
}

int CVICALLBACK update_meas (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,PANEL_UPDATE_MEAS,&measure_state);
			break;
	}
	return 0;
}

int CVICALLBACK enable_dataskt_i_meas (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int state;
	DSHandle dsHandle;
	char url[100];
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,PANEL_ENABLE_DTSKT_I_MEAS,&state); 
			if (state==1)
			{
				SetCtrlAttribute(panel,PANEL_DTSKT_I_MEAS_URL,ATTR_DIMMED,1);
				GetCtrlVal(panel,PANEL_DTSKT_I_MEAS_URL,url);
				DSBindCtrl (panel, PANEL_I_MEAS, url, VAL_DS_WRITE, &dsHandle, NULL);
			}
			else
			{
				SetCtrlAttribute(panel,PANEL_DTSKT_I_MEAS_URL,ATTR_DIMMED,0);
				DSUnbind (panel, PANEL_I_MEAS, NULL);  
			}
			break;
	}
	return 0;
}

int CVICALLBACK enable_dataskt_v_meas (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int state;
	DSHandle dsHandle;
	char url[100];
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,PANEL_ENABLE_DTSKT_V_MEAS,&state); 
			if (state==1)
			{
				SetCtrlAttribute(panel,PANEL_DTSKT_V_MEAS_URL,ATTR_DIMMED,1);
				GetCtrlVal(panel,PANEL_DTSKT_V_MEAS_URL,url);
				DSBindCtrl (panel, PANEL_V_MEAS, url, VAL_DS_WRITE, &dsHandle, NULL);
			}
			else
			{
				SetCtrlAttribute(panel,PANEL_DTSKT_V_MEAS_URL,ATTR_DIMMED,0);
				DSUnbind (panel, PANEL_V_MEAS, NULL);  
			}
			break;
	}
	return 0;
}

int CVICALLBACK enable_dataskt_r_meas (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int state;
	DSHandle dsHandle;
	char url[100];
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,PANEL_ENABLE_DTSKT_R_MEAS,&state); 
			if (state==1)
			{
				SetCtrlAttribute(panel,PANEL_DTSKT_R_MEAS_URL,ATTR_DIMMED,1);
				GetCtrlVal(panel,PANEL_DTSKT_R_MEAS_URL,url);
				DSBindCtrl (panel, PANEL_R_MEAS, url, VAL_DS_WRITE, &dsHandle, NULL);
			}
			else
			{
				SetCtrlAttribute(panel,PANEL_DTSKT_R_MEAS_URL,ATTR_DIMMED,0);
				DSUnbind (panel, PANEL_R_MEAS, NULL);  
			}
			break;
	}
	return 0;
}
