/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2023. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_I_OUTPUT                   2
#define  PANEL_I_RANGE                    3       /* callback function: change_i_range */
#define  PANEL_V_COMP                     4       /* callback function: change_v_compliance */
#define  PANEL_V_OUTPUT                   5
#define  PANEL_V_RANGE                    6       /* callback function: chang_v_range */
#define  PANEL_I_COMP                     7       /* callback function: change_i_compliance */
#define  PANEL_R_MEAS                     8
#define  PANEL_V_MEAS                     9
#define  PANEL_I_MEAS                     10
#define  PANEL_DTSKT_R_MEAS_URL           11
#define  PANEL_DTSKT_I_OUTPUT_URL         12
#define  PANEL_DTSKT_V_MEAS_URL           13
#define  PANEL_DTSKT_I_MEAS_URL           14
#define  PANEL_ENABLE_DTSKT_R_MEAS        15      /* callback function: enable_dataskt_r_meas */
#define  PANEL_DTSKT_V_OUTPUT_URL         16
#define  PANEL_ENABLE_DTSKT_V_MEAS        17      /* callback function: enable_dataskt_v_meas */
#define  PANEL_ENABLE_DTSKT_I_OUTPUT      18      /* callback function: enable_dataskt_i_output */
#define  PANEL_ENABLE_DTSKT_I_MEAS        19      /* callback function: enable_dataskt_i_meas */
#define  PANEL_ENABLE_DTSKT_V_OUTPUT      20      /* callback function: enable_dataskt_v_output */
#define  PANEL_GPIB_ADDR                  21
#define  PANEL_OUTPUT_TYPE                22      /* callback function: select_output_type */
#define  PANEL_CONNECT                    23      /* callback function: connect */
#define  PANEL_UPDATE_MEAS                24      /* callback function: update_meas */
#define  PANEL_TEXTMSG_5                  25
#define  PANEL_TEXTMSG_6                  26
#define  PANEL_TEXTMSG_7                  27
#define  PANEL_TEXTMSG_8                  28


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK chang_v_range(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK change_i_compliance(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK change_i_range(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK change_v_compliance(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK connect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK enable_dataskt_i_meas(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK enable_dataskt_i_output(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK enable_dataskt_r_meas(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK enable_dataskt_v_meas(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK enable_dataskt_v_output(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK select_output_type(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK update_meas(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
