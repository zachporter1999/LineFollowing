//#define _DEBUG

#ifdef _DEBUG
#define DELAY 500
#else
#define DELAY 10
#endif 

#define N_IR_SENSORS 5

// Map of ir sensors to pins
int ir_pin_numbers[N_IR_SENSORS] = {27, 26, 25, 33};

// ir sensor values
int ir_values[N_IR_SENSORS];

// line position
double line_pos   = 0;

void read_IR_sensors(int* a_values, int n_sensors) {
  for (int sensor = 0; sensor < n_sensors; sensor++) {
    a_values[sensor] = (double)!digitalRead(ir_pin_numbers[sensor]);
  }
}

double get_position(int* a_values, int n_sensors) {
	unsigned sum = 0;
	unsigned weighted_sum = 0;

	for (int sensor = 0; sensor < n_sensors; sensor++) {
		weighted_sum += a_values[sensor] * (sensor + 1);
		sum += a_values[sensor];
	}

#ifdef _DEBUG
  Serial.print("w_sum = ");
  Serial.println(weighted_sum);
  Serial.print("Sum = ");
  Serial.println(sum);
#endif

  if (sum == 0)
  {
    return 0;
  }

	return (double)weighted_sum / (double)sum;
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
  read_IR_sensors(ir_values, N_IR_SENSORS);
  line_pos = get_position(ir_values, N_IR_SENSORS);

  // calculate angle to steer
  char buff[128];
  sprintf(buff, "Line Position: %f", line_pos);
  Serial.println(buff);

  delay(DELAY);
}
