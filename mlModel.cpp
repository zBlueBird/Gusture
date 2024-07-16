#if 1
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "alipay_mem.h"
#include "trace.h"

extern "C" {void RunModel(float* input_data);}

// Include your model data. Replace this with the header file for your model.
//#include "model.h"
// Define the number of elements in the input tensor
#define INPUT_SIZE 96*96
using MyOpResolver = tflite::MicroMutableOpResolver<10>;
// Replace this with your model's input and output tensor sizes
//const int tensor_arena_size = 20 * 1024;
//uint8_t tensor_arena[tensor_arena_size] __attribute__ ((aligned(16)));

char* tflite_data = NULL;

void RunModel(float* input_data) {
	tflite::MicroErrorReporter micro_error_reporter;
	tflite::ErrorReporter* error_reporter = &micro_error_reporter;
	
	//
	tflite_data = (char*)csi_malloc(800 * 1024);
	memcpy(tflite_data, (void*)(uint32_t)0x04500000, 700* 1024);
	
	const tflite::Model* model = ::tflite::GetModel(tflite_data);
	MyOpResolver op_resolver;
	// Register your model's operations here
//	MicroInterpreter(const Model* model, const MicroOpResolver& op_resolver,
//                   uint8_t* tensor_arena, size_t tensor_arena_size,
//                   MicroResourceVariables* resource_variables = nullptr,
//                   MicroProfilerInterface* profiler = nullptr);
	const int tensor_arena_size = 100 * 1024;
	uint8_t* tensor_arena = (uint8_t*)csi_malloc(tensor_arena_size);
				   
	tflite::MicroInterpreter interpreter(model, op_resolver, tensor_arena, tensor_arena_size, nullptr, nullptr);
	interpreter.AllocateTensors();
	TfLiteTensor* input = interpreter.input(0);
	
	// Add checks for input tensor properties here if needed
	DBG_DIRECT("input->type %d", input->type);
	
	
	// Copy image data to model's input tensor
	for (int i = 0; i < INPUT_SIZE; ++i) {
		input->data.f[i] = input_data[i];
	}
	TfLiteStatus invoke_status = interpreter.Invoke();
	// Add checks for successful inference here if needed
	TfLiteTensor* output = interpreter.output(0);
	// Add checks for output tensor properties here if needed
	// Process your output value here
	// For example, SSD models typically produce an array of bounding boxes
	int num_elements = 1;
    for (int i = 0; i < output->dims->size; i++) {
        num_elements *= output->dims->data[i];
    }

    // ???????
    switch (output->type) {
        case kTfLiteFloat32: {
            float* output_data = output->data.f;
            for (int i = 0; i < num_elements; i++) {
                error_reporter->Report("Output[%d]: %f", i, output_data[i]);
            }
            break;
        }
        case kTfLiteInt8: {
            int8_t* output_data = output->data.int8;
            for (int i = 0; i < num_elements; i++) {
                error_reporter->Report("Output[%d]: %d", i, output_data[i]);
            }
            break;
        }
        case kTfLiteUInt8: {
            uint8_t* output_data = output->data.uint8;
            for (int i = 0; i < num_elements; i++) {
                error_reporter->Report("Output[%d]: %u", i, output_data[i]);
            }
            break;
        }
        default: {
            error_reporter->Report("Unsupported tensor type: %d", output->type);
            break;
        }
    }
	
	//DBG_DIRECT("output->type %d", output->type);
	csi_free(tensor_arena);
	csi_free(tflite_data);
}

#endif
