#define N_IR_SENSORS 5

// Map of ir sensors to pins
int ir_pin_numbers[N_IR_SENSORS] = {27, 26, 25, 33};

// ir sensor values
int ir_values[N_IR_SENSORS];

// line position
double w_sum = 0;
double sum   = 0;
double line_pos   = -1;

void read_IR_sensors(int* a_values, int n_sensors) {
  for (int sensor = 0; sensor < n_sensors; sensor++) {
    a_values[sensor] = (double)digitalRead(ir_pin_numbers[sensor]);
  }
}

double get_position(int* a_values, int n_sensors, double* weighted_sum, double* sum) {
	*sum = 0;
	*weighted_sum = 0;

	for (int sensor = 0; sensor < n_sensors; sensor++) {
		*weighted_sum += a_values[sensor] * (sensor + 1);
		*sum += a_values[sensor];
	}

	return *weighted_sum / *sum;
}

void setup() {
  Serial.begin(9600);

  // setup ir pins
  for (int idx = 0; idx < N_IR_SENSORS; idx++) {
    pinMode(ir_pin_numbers[idx], INPUT);
  }
}

void loop() {
  //Line Follow
  read_IR_sensors();
  line_pos = get_position(ir_values, N_IR_SENSORS, &w_sum, &sum);

  // calculate angle to steer

  delay(500);
}
