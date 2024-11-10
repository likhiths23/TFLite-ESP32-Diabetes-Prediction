#include <EloquentTinyML.h>
#include "diabetes_model_esp32.h"

#define NUMBER_OF_INPUTS 8 
#define NUMBER_OF_OUTPUTS 2

#define TENSOR_ARENA_SIZE 10*1024  

Eloquent::TinyML::TfLite<NUMBER_OF_INPUTS, NUMBER_OF_OUTPUTS, TENSOR_ARENA_SIZE> ml;

// Mean and standard deviation values for standardization
const float means[NUMBER_OF_INPUTS] = {3.74267101, 120.85504886, 69.41530945, 20.3990228, 81.43811075, 31.98338762, 0.46916775, 32.90716612}; 
const float stds[NUMBER_OF_INPUTS] = {3.31056497, 32.00895893, 18.49751705, 15.42140098, 116.14014299, 7.73431907, 0.33657233, 11.49406506};

void setup() {
    Serial.begin(115200);
    ml.begin(diabetes_model_esp32);
}

float fResult[NUMBER_OF_OUTPUTS] = {0};

void loop() {
    float input1[8] = {6,	148,	72,	35,	0,	33.6,	0.627,	50}; 
    float input2[8] = {1,	85,	66,	29,	0,	26.6,	0.351,	31}; 
    float input3[8] = {8,	183,	64,	0,	0,	23.3,	0.672,	32}; 
    float input4[8] = {1,	89,	66,	23,	94,	28.1,	0.167,	21}; 

    standardizeInput(input1);
    predictAndDisplay(input1, "Input1");

    standardizeInput(input2);
    predictAndDisplay(input2, "Input2");

    standardizeInput(input3);
    predictAndDisplay(input3, "Input3");

    standardizeInput(input4);
    predictAndDisplay(input4, "Input4");

    delay(5000); // 5 seconds delay between predictions
}

void standardizeInput(float *input) {
    for (int i = 0; i < NUMBER_OF_INPUTS; i++) {
        input[i] = (input[i] - means[i]) / stds[i];
    }
}

void predictAndDisplay(float *input, const char* inputName) {
    initfResult(fResult);
    Serial.print(inputName);
    Serial.print(": ");
    printInputData(input);

    ml.predict(input, fResult);
    displayOutput(fResult);
}

void initfResult(float *fResult) {
    for (int i = 0; i < NUMBER_OF_OUTPUTS; i++) {
        fResult[i] = 0.0f;
    }
}

void displayOutput(float *fResult) {
    Serial.print("Output: ");
    Serial.print(fResult[0], 2); // Display the first output with 2 decimal places
    Serial.print(" ");
    Serial.println(fResult[1], 2); // Display the second output with 2 decimal places
}

void printInputData(float *input) {
    for (int i = 0; i < NUMBER_OF_INPUTS; i++) {
        Serial.print(input[i], 2);
        Serial.print(" ");
    }
    Serial.println();
}
