#include "Lepton_I2C.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

static int i2c_fd = -1;

// I2C 초기화
static int init_i2c() {
	if (i2c_fd != -1) return 0;
	
	i2c_fd = open("/dev/i2c-1", O_RDWR);
	if (i2c_fd < 0) {
		perror("Failed to open I2C bus");
		return -1;
	}
	
	if (ioctl(i2c_fd, I2C_SLAVE, LEP_I2C_DEVICE_ADDRESS) < 0) {
		perror("Failed to acquire bus access");
		return -1;
	}
	
	return 0;
}

// Radiometry 활성화
int lepton_enable_radiometry() {
	if (init_i2c() < 0) return -1;
	
	uint16_t enable = 1;
	// I2C를 통해 radiometry 활성화 명령 전송
	if (write(i2c_fd, &enable, sizeof(enable)) != sizeof(enable)) {
		perror("Failed to enable radiometry");
		return -1;
	}
	
	return 0;
}

// 스팟미터 온도 읽기
float lepton_get_spotmeter_temp() {
	if (init_i2c() < 0) return -1;
	
	uint16_t raw_temp;
	if (read(i2c_fd, &raw_temp, sizeof(raw_temp)) != sizeof(raw_temp)) {
		perror("Failed to read spotmeter temperature");
		return -1;
	}
	
	return lepton_raw_to_temperature(raw_temp);
}

// 온도 보정 설정
int lepton_set_rad_linear_correction(float offset, float slope) {
	if (init_i2c() < 0) return -1;
	
	struct {
		float offset;
		float slope;
	} correction = {offset, slope};
	
	if (write(i2c_fd, &correction, sizeof(correction)) != sizeof(correction)) {
		perror("Failed to set temperature correction");
		return -1;
	}
	
	return 0;
}

// Raw 값을 온도로 변환
float lepton_raw_to_temperature(uint16_t raw_value) {
	// Lepton 2.5 전용 온도 변환식
	// 참고: Lepton 2.5는 raw 값이 켈빈 온도의 100배
	float temp_k = raw_value / 100.0f;
	return temp_k - 273.15f; // 켈빈을 섭씨로 변환
}
