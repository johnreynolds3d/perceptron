#include "headers/perceptron.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TrainingSet *TrainingSet_create(int input_1, int input_2, int output) {

  TrainingSet *training_set = calloc(1, sizeof(TrainingSet));
  assert(training_set != NULL);

  training_set->inputs[0] = input_1;
  training_set->inputs[1] = input_2;

  training_set->output = output;

  return training_set;
}

void TrainingSet_destroy(TrainingSet *training_set) {
  assert(training_set != NULL);
  free(training_set);
}

void TrainingSet_print(TrainingSet *training_set) {
  assert(training_set != NULL);
  printf("\t[%d %d] %d\n", training_set->inputs[0], training_set->inputs[1],
         training_set->output);
}

void initialise_weights(int num_weights, double *weights, double *bias) {

  assert(weights != NULL && bias != NULL);

  unsigned int i = 0;

  for (i = 0; i < num_weights; i++) {
    weights[i] = (double)rand() / RAND_MAX * 2.0 - 1.0;
  }
  *bias = (double)rand() / RAND_MAX * 2.0 - 1.0;
}

double calc_dot_product_bias(int num_weights, double *weights, int num_inputs,
                             double *inputs, double *bias) {

  assert(weights != NULL && inputs != NULL && bias != NULL);

  assert(num_weights == num_inputs);

  double dot_product_bias = 0;

  unsigned int i = 0;

  for (i = 0; i < num_weights; i++) {
    dot_product_bias = dot_product_bias + (weights[i] * inputs[i]);
  }
  dot_product_bias = dot_product_bias + *bias;

  return dot_product_bias;
}

int calc_output(int num_weights, double *weights, double input_1,
                double input_2, double *bias) {

  assert(weights != NULL && bias != NULL);

  double inputs[] = {input_1, input_2};

  double dot_product_bias = calc_dot_product_bias(
      num_weights, weights, (int)(sizeof(inputs) / sizeof(double)), inputs,
      bias);

  return (dot_product_bias > 0) ? 1 : 0;
}

void update_weights(TrainingSet **training_sets, int num_weights,
                    double *weights, double *bias, double *total_error, int j) {

  assert(training_sets != NULL && weights != NULL && bias != NULL &&
         total_error != NULL);

  double error = training_sets[j]->output -
                 calc_output(num_weights, weights, training_sets[j]->inputs[0],
                             training_sets[j]->inputs[1], bias);

  *total_error = *total_error + fabs(error);

  unsigned int i = 0;

  for (i = 0; i < num_weights; i++) {
    weights[i] = weights[i] + (error * training_sets[j]->inputs[i]);
  }

  *bias = *bias + error;
}

void train(int num_training_sets, TrainingSet **training_sets, int num_weights,
           double *weights, double *bias, double *total_error, int num_epochs) {

  assert(training_sets != NULL && weights != NULL && bias != NULL &&
         total_error != NULL);

  printf("I start out by setting my weights and bias to random numbers between "
         "-1 and 1:\n\n");

  initialise_weights(num_weights, weights, bias);

  printf("\tWeights: %9f  %9f\tBias: %9f\n\nThen the training begins...\n\n",
         weights[0], weights[1], *bias);

  unsigned int i = 0;
  unsigned int j = 0;

  for (i = 0; i < num_epochs; i++) {

    printf("Epoch %d:\n", i + 1);
    *total_error = 0;

    for (j = 0; j < num_training_sets; j++) {
      update_weights(training_sets, num_weights, weights, bias, total_error, j);
      printf("\tWeights: %9f  %9f\tBias: %9f\n", weights[0], weights[1], *bias);
    }
    printf("\n\tTotal Error: %d\n\n", (int)*total_error);
  }
}
