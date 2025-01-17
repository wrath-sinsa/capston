#ifndef LEPTON_I2C
#define LEPTON_I2C

#include <stdint.h>

// Radiometry 관련 명령어 정의
#define LEP_I2C_DEVICE_ADDRESS                  0x2A
#define LEP_RAD_ENABLE                          0x4E00
#define LEP_RAD_SPOTMETER                       0x4E04
#define LEP_RAD_ROI                            0x4E08
#define LEP_RAD_LINEAR_TMP_CORRECTION          0x4E14

void lepton_perform_ffc();
void lepton_reboot();

// Radiometry 관련 함수 추가
int lepton_enable_radiometry();
float lepton_get_spotmeter_temp();
// 온도 보정 설정
int lepton_set_rad_linear_correction(float offset, float slope);
// Raw 값을 온도로 변환
float lepton_raw_to_temperature(uint16_t raw_value);

#endif
