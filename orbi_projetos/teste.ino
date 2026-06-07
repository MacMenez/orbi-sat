#include <Wire.h>

#define MPU6050_ADDR 0x68

// Registradores MPU6050
#define SMPLRT_DIV     0x19
#define CONFIG         0x1A
#define GYRO_CONFIG    0x1B
#define ACCEL_CONFIG   0x1C
#define INT_ENABLE     0x38
#define ACCEL_XOUT_H   0x3B
#define PWR_MGMT_1     0x6B
#define PWR_MGMT_2     0x6C
#define USER_CTRL      0x6A
#define FIFO_EN        0x23

// Sensibilidade para ±2g e ±250°/s
const float AccelScaleFactor = 16384.0;
const float GyroScaleFactor  = 131.0;

void writeRegister(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}

void readRegisters(uint8_t reg, uint8_t count, uint8_t *data)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(MPU6050_ADDR, count);

    uint8_t i = 0;
    while (Wire.available() && i < count)
    {
        data[i++] = Wire.read();
    }
}

void initMPU6050()
{
    delay(150);

    writeRegister(SMPLRT_DIV, 0x07);
    writeRegister(PWR_MGMT_1, 0x01);
    writeRegister(PWR_MGMT_2, 0x00);
    writeRegister(CONFIG, 0x00);

    // ±250 °/s
    writeRegister(GYRO_CONFIG, 0x00);

    // ±2g
    writeRegister(ACCEL_CONFIG, 0x00);

    writeRegister(FIFO_EN, 0x00);
    writeRegister(INT_ENABLE, 0x01);
    writeRegister(USER_CTRL, 0x00);
}

void setup()
{
    Serial.begin(115200);

    Wire.begin(21, 22);

    Serial.println();
    Serial.println("Inicializando MPU6050...");

    initMPU6050();

    Serial.println("MPU6050 pronto.");
}

void loop()
{
    uint8_t rawData[14];

    readRegisters(ACCEL_XOUT_H, 14, rawData);

    int16_t AccelX =
        (int16_t)((rawData[0] << 8) | rawData[1]);

    int16_t AccelY =
        (int16_t)((rawData[2] << 8) | rawData[3]);

    int16_t AccelZ =
        (int16_t)((rawData[4] << 8) | rawData[5]);

    int16_t TempRaw =
        (int16_t)((rawData[6] << 8) | rawData[7]);

    int16_t GyroX =
        (int16_t)((rawData[8] << 8) | rawData[9]);

    int16_t GyroY =
        (int16_t)((rawData[10] << 8) | rawData[11]);

    int16_t GyroZ =
        (int16_t)((rawData[12] << 8) | rawData[13]);

    // Conversões
    float Ax = AccelX / AccelScaleFactor;
    float Ay = AccelY / AccelScaleFactor;
    float Az = AccelZ / AccelScaleFactor;

    float Temp = (TempRaw / 340.0) + 36.53;

    float Gx = GyroX / GyroScaleFactor;
    float Gy = GyroY / GyroScaleFactor;
    float Gz = GyroZ / GyroScaleFactor;

    Serial.printf(
        "Ax: %.3f g | Ay: %.3f g | Az: %.3f g | "
        "Temp: %.2f °C | "
        "Gx: %.2f °/s | Gy: %.2f °/s | Gz: %.2f °/s\n",
        Ax, Ay, Az,
        Temp,
        Gx, Gy, Gz
    );

    delay(100);
}