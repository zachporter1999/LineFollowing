// ir pins
/*
 * Assuming all ir sensors are wired next to one another
 * IR_PIN is the values of the first pin, and N_IR_SENSORS
 * is the number of ir sensors.
 * 
 * Allows for easy changing of the number of sensors.
 */
#define IR_PIN 2
#define N_IR_SENSORS 5

// line follow constants
/*
 * CENTER is the position of the middle of the LAD Unit
 * used for controling motor.
 */
#define MOTOR_SPEED 75
#define STEERING_SENSITIVITY 10
#define CENTER (1 + N_IR_SENSORS) / 2

// ir sensor values
int ir_values[N_IR_SENSORS];

// line position
double w_sum = NULL;
double sum   = NULL;
double line_pos   = NULL;
double mtr_spd_l = NULL;
double mtr_spd_r = NULL;

/*
 * Reads the ir sensors.
 * 
 * reads from pin IR_PIN to IR_PIN + N_IR_SENSOrs
 * sensor reads 0 when no line is detected
 * sensor reads 1 when a line is detected
 */
void read_IR_sensors() {
  for (int sensor = 0; sensor < N_IR_SENSORS; sensor++) {
    ir_values[sensor] = (double)digitalRead(sensor + IR_PIN);
  }
}

double get_position(int values[], int n_sensors, double* weighted_sum, double* sum) {
	*sum = 0;
	*weighted_sum = 0;
	for (int sensor = 0; sensor < n_sensors; sensor++) {
		*weighted_sum += values[sensor] * (sensor + 1);
		*sum += values[sensor];
	}

	return *weighted_sum / *sum;
}
void control_motors(double pid_output, double factor, double speed, double* motor_left, double* motor_right) {
	// adjust wheel power to steer toward line
	*motor_left = (double)speed - factor * pid_output;
	*motor_right = (double)speed + factor * pid_output;
}

void setup() {
  Serial.begin(9600);

  // setup ir pins
  for (int pin = IR_PIN; pin < IR_PIN + N_IR_SENSORS; pin++) {
    pinMode(pin, INPUT);
  }
}

void loop() {
  //Line Follow
  read_IR_sensors();
  line_pos = get_position(ir_values, N_IR_SENSORS, &w_sum, &sum);

  // checks that line exists
  if (line_pos != -eLadNoLine) {
    // CENTER - line_pos shifts line position to be with respect to the center of the LAD Unit
    control_motors(CENTER - line_pos, STEERING_SENSITIVITY, MOTOR_SPEED, &mtr_spd_l, &mtr_spd_r);
  } else {
    Serial.println("Recalculating...");
  }

    // drive motor
  if (mtr_spd_l > 0 && mtr_spd_r > 0) {
    motorLeft.setSpeed(mtr_spd_l);
    motorRight.setSpeed(mtr_spd_r);
  
    motorLeft.run(FORWARD);
    motorRight.run(FORWARD);
  } else {
    motorLeft.run(RELEASE);
    motorRight.run(RELEASE);
  }
    
  Serial.print(mtr_spd_l);
  Serial.print("% ");
  Serial.print(mtr_spd_r);
  Serial.println("%");

  delay(500);
}
