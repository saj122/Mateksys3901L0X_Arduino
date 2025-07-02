#include <Mateksys3901L0X.h>

Mateksys3901L0X optFlow;

void setup() {
  Serial.begin(115200);
  optFlow.begin(Serial1);

  delay(1000);
}

void loop() {
  if (optFlow.waitFlow()) {
    int32_t distance = optFlow.getCurrentFlow().distance; // mm
    double flowX = optFlow.getCurrentFlow().flowX; // mm/s
    double flowY = optFlow.getCurrentFlow().flowY; //mm/s
    
    Serial.println("Distance: " + String(distance));
    Serial.println("FlowX: " + String(flowX));
    Serial.println("FlowY: " + String(flowY));
  }
}
