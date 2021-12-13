#include "CameraGUI_Attributes.h"

int CameraGUI_Attributes::SCREEN_ASPECT = 80;
int CameraGUI_Attributes::width = 0;
int CameraGUI_Attributes::height = 0;
float CameraGUI_Attributes::CameraTransValue_array[3] = { 0, 0, 0 };
float CameraGUI_Attributes::CameraRotateValue_array[3] = { 0, 0, 0 };

float CameraGUI_Attributes::CameraWorldTransValue_array[3] = { 0, 0, 0 };
float CameraGUI_Attributes::CameraWorldRotateValue_array[3] = { 0, 0, 0 };

float CameraGUI_Attributes::ortho_array[3] = { 30, 30, 30 };
float CameraGUI_Attributes::perspective_array[3] = { 30, 30, 30 };
float CameraGUI_Attributes::eye_array[3] = { 0, 0, 0 };
float CameraGUI_Attributes::at_array[3] = { 0, 0, 1 };
float CameraGUI_Attributes::up_array[3] = { 0, 1, 0 };

int CameraGUI_Attributes::cameraMode = ORTHO;
float CameraGUI_Attributes::Fovy = -30.0f;
float CameraGUI_Attributes::aspect = 1;