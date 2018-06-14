// Copyright 2015 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//#import "RunModelViewController.h"

#include <fstream>
#include <utility>
#include <vector>

#include "tensorflow/cc/ops/const_op.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/stringpiece.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/str_util.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/command_line_flags.h"
#include <pthread.h>
#include <unistd.h>
//#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

#include "tensorflow/contrib/lite/kernels/register.h"
#include "tensorflow/contrib/lite/model.h"
#include "tensorflow/contrib/lite/string_util.h"
#include "tensorflow/contrib/lite/tools/mutable_op_resolver.h"

//#include "ios_image_load.h"

// These are all common classes it's handy to reference with no namespace.
using tensorflow::Flag;
using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;
using tensorflow::int32;

#define LOG(x) std::cerr
#define CHECK(x)                  \
  if (!(x)) {                     \
    LOG(ERROR) << #x << "failed"; \
    exit(1);                      \
  }

//NSString* RunInferenceOnImage();

/*
@interface RunModelViewController ()
@end

@implementation RunModelViewController {
}

- (IBAction)getUrl:(id)sender {
  NSString* inference_result = RunInferenceOnImage();
  self.urlContentTextView.text = inference_result;
}

@end
*//*
class RunModelViewController {

    IBAction RunModelViewController(id sender);
}

IBAction RunModelViewController::getUrl (id sender) {
    NSString* inference_result = RunInferenceOnImage();
    self.urlContentTextView.text = inference_result;

}



// Returns the top N confidence values over threshold in the provided vector,
// sorted by confidence in descending order.*/
static void GetTopN(const float* prediction, const int prediction_size, const int num_results,
                    const float threshold, std::vector<std::pair<float, int> >* top_results) {
  // Will contain top N results in ascending order.
  std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int> >,
                      std::greater<std::pair<float, int> > >
      top_result_pq;

  const long count = prediction_size;
  for (int i = 0; i < count; ++i) {
    const float value = prediction[i];

    // Only add it if it beats the threshold and has a chance at being in
    // the top N.
    if (value < threshold) {
      continue;
    }

    top_result_pq.push(std::pair<float, int>(value, i));

    // If at capacity, kick the smallest value out.
    if (top_result_pq.size() > num_results) {
      top_result_pq.pop();
    }
  }

  // Copy to output vector and reverse into descending order.
  while (!top_result_pq.empty()) {
    top_results->push_back(top_result_pq.top());
    top_result_pq.pop();
  }
  std::reverse(top_results->begin(), top_results->end());
}

std::vector<uint8_t> LoadImageFromFile(const char* file_name, int* out_width, int* out_height,
                                       int* out_channels) {
  /*cv::Mat image;

  //iimage = cv::imread("grasshopper.jpg");

  FILE* file_handle = fopen(file_name, "rb");
  fseek(file_handle, 0, SEEK_END);
  const size_t bytes_in_file = ftell(file_handle);
  fseek(file_handle, 0, SEEK_SET);
  std::vector<uint8_t> file_data(bytes_in_file);
  fread(file_data.data(), 1, bytes_in_file, file_handle);
  fclose(file_handle);

  CFDataRef file_data_ref =
      CFDataCreateWithBytesNoCopy(NULL, file_data.data(), bytes_in_file, kCFAllocatorNull);
  CGDataProviderRef image_provider = CGDataProviderCreateWithCFData(file_data_ref);
*/
  auto root = tensorflow::Scope::NewRootScope();
  using namespace ::tensorflow::ops;  // NOLINT(build/namespaces)

  string input_name = "file_reader";
  string output_name = "normalized";

  // read file_name into a tensor named input
  Tensor input(tensorflow::DT_STRING, tensorflow::TensorShape());
  TF_RETURN_IF_ERROR(
      ReadEntireFile(tensorflow::Env::Default(), file_name, &input));

  // use a placeholder to read input data
  auto file_reader =
      Placeholder(root.WithOpName("input"), tensorflow::DataType::DT_STRING);

  std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
      {"input", input},
};



  // Check File Type
  const char* suffix = strrchr(file_name, '.');
  if (!suffix || suffix == file_name) {
    suffix = "";
  }
  //CGImageRef image;
  tensorflow::Output image_reader;

} if (strcasecmp(suffix, ".png") == 0) {
    image_reader = DecodePng(root.WithOpName("png_reader"), file_reader,
                             DecodePng::Channels(wanted_channels));
    //image = CGImageCreateWithPNGDataProvider(image_provider, NULL, true, kCGRenderingIntentDefault);
  } else if ((strcasecmp(suffix, ".jpeg") == 0) || (strcasecmp(suffix, ".jpeg") == 0)) {
    image_reader = DecodeJpeg(root.WithOpName("jpeg_reader"), file_reader,DecodeJpeg::Channels(wanted_channels));    
    //image = CGImageCreateWithJPEGDataProvider(image_provider, NULL, true, kCGRenderingIntentDefault);
  } else {
    //CFRelease(image_provider);
    //CFRelease(file_data_ref);
    fprintf(stderr, "Unknown suffix for file '%s'\n", file_name);
    *out_width = 0;
    *out_height = 0;
    *out_channels = 0;
    return std::vector<uint8_t>();

  }
   

  //*out_width = image.cols();
  //*out_height = image.rows();
  //*out_channels = image.chnnels();

  const int width = image.cols;
  const int height = image.rows;
  const int channels = 4;
//  CGColorSpaceRef color_space = CGColorSpaceCreateDeviceRGB();
  const int bytes_per_row = (width * channels);
  const int bytes_in_image = (bytes_per_row * height);
  std::vector<uint8_t> result(bytes_in_image);
  const int bits_per_component = 8;

  // Now cast the image data to float so we can do normal math on it.
  auto float_caster =
      Cast(root.WithOpName("float_caster"), image_reader, tensorflow::DT_FLOAT);
  // The convention for image ops in TensorFlow is that all images are expected
  // to be in batches, so that they're four-dimensional arrays with indices of
  // [batch, height, width, channel]. Because we only have a single image, we
  // have to add a batch dimension of 1 to the start with ExpandDims().
  auto dims_expander = ExpandDims(root, float_caster, 0);
  // Bilinearly resize the image to fit the required dimensions.
  auto resized = ResizeBilinear(
      root, dims_expander,
      Const(root.WithOpName("size"), {input_height, input_width}));



/*
  CGContextRef context =
      CGBitmapContextCreate(result.data(), width, height, bits_per_component, bytes_per_row,
                            color_space, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
  CGColorSpaceRelease(color_space);
  CGContextDrawImage(context, CGRectMake(0, 0, width, height), image);
  CGContextRelease(context);
  CFRelease(image);
  CFRelease(image_provider);
  CFRelease(file_data_ref);

  *out_width = width;
  *out_height = height;
  *out_channels = channels;*/
  return result;
}

/*
NSString* FilePathForResourceName(NSString* name, NSString* extension) {
  NSBundle mainBundle = new mainBundle();
  NSString* file_path = mainBundle->pathForResource(name, extension);
  //NSString* file_path = [[NSBundle mainBundle] pathForResource:name ofType:extension];
  if (file_path == NULL) {
    LOG(FATAL) << "Couldn't find '" << [name UTF8String] << "." << [extension UTF8String]
               << "' in bundle.";
  }
  return file_path;
}

const char* RunInferenceOnImage() {*/
int main(){
  std::string graph;
  const int num_threads = 1;
  std::string input_layer_type = "float";
  std::vector<int> sizes = {1, 224, 224, 3};

  const char* graph_path = "mobilenet_v1_1.0_224.tflite\0";

  std::unique_ptr<tflite::FlatBufferModel> model(
      tflite::FlatBufferModel::BuildFromFile(graph_path));
  if (!model) {
    LOG(FATAL) << "Failed to mmap model " << graph;
  }
  LOG(INFO) << "Loaded model " << graph;
  model->error_reporter();
  LOG(INFO) << "resolved reporter";

#ifdef TFLITE_CUSTOM_OPS_HEADER
  tflite::MutableOpResolver resolver;
  RegisterSelectedOps(&resolver);
#else
  tflite::ops::builtin::BuiltinOpResolver resolver;
#endif

  std::unique_ptr<tflite::Interpreter> interpreter;
  tflite::InterpreterBuilder(*model, resolver)(&interpreter);
  if (!interpreter) {
    LOG(FATAL) << "Failed to construct interpreter";
  }

  if (num_threads != -1) {
    interpreter->SetNumThreads(num_threads);
  }

  int input = interpreter->inputs()[0];

  if (input_layer_type != "string") {
    interpreter->ResizeInputTensor(input, sizes);
  }

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    LOG(FATAL) << "Failed to allocate tensors!";
  }

  // Read the label list
  const char* labels_path = "labels.txt\0";
  std::vector<std::string> label_strings;
  std::ifstream t;
  t.open(labels_path);
  std::string line;
  while (t) {
    std::getline(t, line);
    label_strings.push_back(line);
  }
  t.close();

  // Read the Grace Hopper image.
  const char* image_path = "grace_hopper.jpg";
  int image_width;
  int image_height;
  int image_channels;
  std::vector<uint8_t> image_data =
      LoadImageFromFile(image_path, &image_width, &image_height, &image_channels);
  const int wanted_width = 224;
  const int wanted_height = 224;
  const int wanted_channels = 3;
  const float input_mean = 127.5f;
  const float input_std = 127.5f;
  assert(image_channels >= wanted_channels);
  uint8_t* in = image_data.data();
  float* out = interpreter->typed_tensor<float>(input);
  for (int y = 0; y < wanted_height; ++y) {
    const int in_y = (y * image_height) / wanted_height;
    uint8_t* in_row = in + (in_y * image_width * image_channels);
    float* out_row = out + (y * wanted_width * wanted_channels);
    for (int x = 0; x < wanted_width; ++x) {
      const int in_x = (x * image_width) / wanted_width;
      uint8_t* in_pixel = in_row + (in_x * image_channels);
      float* out_pixel = out_row + (x * wanted_channels);
      for (int c = 0; c < wanted_channels; ++c) {
        out_pixel[c] = (in_pixel[c] - input_mean) / input_std;
      }
    }
  }

  if (interpreter->Invoke() != kTfLiteOk) {
    LOG(FATAL) << "Failed to invoke!";
  }

  float* output = interpreter->typed_output_tensor<float>(0);
  const int output_size = 1000;
  const int kNumResults = 5;
  const float kThreshold = 0.1f;
  std::vector<std::pair<float, int> > top_results;
  GetTopN(output, output_size, kNumResults, kThreshold, &top_results);

  std::stringstream ss;
  ss.precision(3);
  for (const auto& result : top_results) {
    const float confidence = result.first;
    const int index = result.second;

    ss << index << " " << confidence << "  ";

    // Write out the result as a string
    if (index < label_strings.size()) {
      // just for safety: theoretically, the output is under 1000 unless there
      // is some numerical issues leading to a wrong prediction.
      ss << label_strings[index];
    } else {
      ss << "Prediction: " << index;
    }

    ss << "\n";
  }

  LOG(INFO) << "Predictions: " << ss.str();

  std::string predictions = ss.str();
  const char* result = "\0";
//  result = [NSString stringWithFormat:@"%@ - %s", result, predictions.c_str()];
  std::printf("%s - %s \n", result, predictions.c_str());
  return 0;
}
