#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "model/airc_model.h"

namespace {
  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* input = nullptr;
  TfLiteTensor* output = nullptr;
  int inference_count = 0;
  
  // Create an area of memory to use for input, output, and intermediate arrays.
  // Finding the minimum value for your model may require some trial and error.
  constexpr int kTensorArenaSize = 2 * 1024;
  uint8_t tensor_arena[kTensorArenaSize];
} 

void setup() {

  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;
  
  model = ::tflite::GetModel(converted_model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report(
        "Model provided is schema version %d not equal "
        "to supported version %d.",
        model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }
  
  static tflite::ops::micro::AllOpsResolver resolver;
  
  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena,
                                     kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;
  
  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    error_reporter->Report("AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

  // Keep track of how many inferences we have performed.
  inference_count = 0;

  Serial.begin(115200);
  Serial.print(input->dims->size);
  Serial.print(input->dims->data[0]);
  Serial.print(input->dims->data[1]);
}

void loop() {
                   
  input->data.f[0]=3.74626993;// center
  input->data.f[1]=1.21416047;// right
  input->data.f[2]=2.05516886;// left

  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    error_reporter->Report("Invoke failed on 1.: %f\n",
                           static_cast<double>(1.));
    return;
  }

  float omega = output->data.f[0];//舵角
  float nu = output->data.f[1];//速度
  Serial.print("omega: ");
  Serial.println(omega);
  Serial.print("nu: ");
  Serial.println(nu);
}
