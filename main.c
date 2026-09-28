#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "training_data.h"


struct perceptron {
    float weights[2];
    float bias;
    float learning_rate;
};


//HELPER FUNCTIONS
float MSE(const float *restrict target, const float *restrict values, size_t target_len, size_t values_len){

    assert(target_len == values_len && "target and value arrays have different lengths");
    float sum = 0.0;
    for(size_t i = 0; i < values_len; i++){
        float tmp = target[i] - values[i];
        sum += tmp * tmp;
        
    }
    return sum / 2;
}

float dot_product(const float *restrict a, const float *restrict b, size_t a_len, size_t b_len){

    assert(a_len == b_len && "target and value arrays have different lengths");
    float sum = 0.0;
    for(size_t i = 0; i < a_len; i++){
        sum += a[i] * b[i];
        
    }
    return sum;
}

float sigmoid(const float in){
    return 1 / (1 + exp(-in));

}

//RENDER FUNCTIONS
void render_perceptron_data(const struct perceptron_data *array, size_t length, const char *filepath){

    FILE* fptr;

    fptr = fopen(filepath, "w");

    if (fptr == NULL) {
        printf("Error: %s\n",strerror(errno));

    }

    //define buffer len based on number of points to render
    size_t string_len = 160 + length * 55;
    char buffer[string_len];
    memset(buffer, 0, string_len);

    //drawing portion

    int square_size = 500;

    strcat(buffer, "<svg width=\"500\" height=\"500\" viewBox=\"0 0 500 500\" xmlns=\"http://www.w3.org/2000/svg\">\n");
    strcat(buffer, "<rect width=\"500\" height=\"500\" fill=\"white\" />\n");

    for(int i = 0; i < length; i++){

        char tmp[55];
        const char *color;

        if((*(array + i)).label == 1) {
            color = &"blue"[0];
        } else {

        }


        //use square_size - y to flip y axis -> normal coordinate grid
        int written = snprintf(tmp,
            sizeof(tmp),
            "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"red\" />\n",
            (int)*((*(array + i)).data + 0),
            square_size - (int)*((*(array + i)).data + 1),
            5 + (*(array + i)).label * 4
        );

        if (written >= sizeof(tmp)){
            printf("truncation occured");
        }

        strcat(buffer, tmp);
        
    }

    strcat(buffer, "</svg>");

    fputs(buffer, fptr);

    fclose(fptr);


}

void create_plot(const float *array, size_t length, char *filepath){

    FILE* fptr;

    fptr = fopen(filepath, "w");

    if (fptr == NULL) {
        printf("The file is not opened.");
    }

    //define buffer len based on number of points to render
    size_t string_len = 100000 + length * 55;
    char buffer[string_len];
    memset(buffer, 0, string_len);


    //drawing portion

    int square_size = 500;

    strcat(buffer, "<svg width=\"600\" height=\"600\" viewBox=\"0 0 600 600\" xmlns=\"http://www.w3.org/2000/svg\">\n");
    strcat(buffer, "<rect width=\"600\" height=\"600\" fill=\"white\" />\n");
    strcat(buffer, "<line x1=\"100\" y1=\"0\" x2=\"100\" y2=\"600\" stroke=\"black\" stroke-width=\"2\" />\n");
    strcat(buffer, "<line x1=\"0\" y1=\"500\" x2=\"600\" y2=\"500\" stroke=\"black\" stroke-width=\"2\" />\n");
/*     strcat(buffer, "<text x=\"20\" y=\"50\">Hallo Welt</text>\n"); */



    int offset = square_size / length;


    for(int i = 0; i < length; i++){

        char tmp[55];
        const char *color;

        //use square_size - y to flip y axis -> normal coordinate grid
        int written = snprintf(tmp,
            sizeof(tmp),
            "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"green\" />\n",
            i * offset + 100,
            square_size - (int)(*(array + i)),
            2
        );

        if (written >= sizeof(tmp)){
            printf("truncation occured");
        }

        strcat(buffer, tmp);
        
    }

    strcat(buffer, "</svg>");

    fputs(buffer, fptr);

    fclose(fptr); 
}

//MODEL TESTS
void two_dim_perceptron_test(const struct perceptron_data *training_data, const struct perceptron_data *test_data, size_t training_data_len, size_t test_data_len, size_t num_epochs){


    render_perceptron_data(training_data, training_data_len, "/home/julian/Personal/programming/01_active/c_test/output.svg");

    struct perceptron model = {
        {0, 0}, //weights
        0, //bias
        0.01 //learning rate
    };

    float bias[num_epochs];
    int bias_array_index = 0;


    for(int e = 0; e < num_epochs; e++){

        /* printf("epoch: %d\n", e); */

        bias[bias_array_index] = model.bias;
        bias_array_index += 1;

        //stochastic gradient descent
        for(int i = 0; i < training_data_len; i++) {

            //calcualte actual output of perceptron
            float output = (dot_product(model.weights, training_data[i].data, 2, 2) + model.bias) >= 0 ? 1 : 0;

            float error = training_data[i].label - output;

            model.weights[0] = model.weights[0] + model.learning_rate * error * training_data[i].data[0];
            model.weights[1] = model.weights[1] + model.learning_rate * error * training_data[i].data[1];

            model.bias = model.bias + model.learning_rate * error;

        }

    
    }

    create_plot(bias, bias_array_index, "/home/julian/Personal/programming/01_active/c_test/bias_convergence.svg");
    

    printf("TRAINING FINISHED\n");

    //test model
    for(int i = 0; i < test_data_len; i++){

        float output = (dot_product(model.weights, test_data[i].data, 2, 2) + model.bias) >= 0 ? 1 : 0;
        float error = test_data[i].label - output;

    }

    printf("Final Parameters: first weight %f, second weight %f, bias %f\n", model.weights[0], model.weights[1], model.bias);


}

void neural_net_test_gfg(float learning_rate){

    //https://www.geeksforgeeks.org/machine-learning/backpropagation-in-neural-network/

    float inputs[] = {0.35, 0.7};

    float weights[2][2] = {
        {0.2, 0.2}, //neuron h1
        {0.3, 0.3}  //neuron h2
    };



    //calcualte activations layer 1
    float outputs_l1[2];

    for(int neuron = 0; neuron < sizeof(weights) / sizeof(weights[0]); neuron++){
        
        float sum = 0.0;

        for(int input = 0; input < sizeof(inputs) / sizeof(inputs[0]); input++){
            sum += weights[neuron][input] * inputs[input];
        };

        outputs_l1[neuron] = sigmoid(sum);

    };


    //calculate final layer activations
    float weights2[] = {0.3, 0.9};

    float sum = 0.0;

    for(int output = 0; output < sizeof(outputs_l1) / sizeof(outputs_l1[0]); output++){
        sum += weights2[output] * outputs_l1[output];
    }

    float final_activation = sigmoid(sum);

    printf("%f\n", final_activation);


    //MSE
    float error = 0.5 - final_activation;

    printf("final_error %f\n", error);


    //CALCULATE ERROR TERMS

    //final error term
    float e_o = final_activation * ( 1 - final_activation) * error;
    printf("03_error: %f\n", e_o);

    //hidden layer error terms
    float e_h1 = outputs_l1[0] * (1 - outputs_l1[0]) * ( weights2[0] * e_o);
    float e_h2 = outputs_l1[1] * (1 - outputs_l1[1]) * ( weights2[1] * e_o);

    printf("h1_error: %f\n", e_h1);
    printf("h2_error: %f\n", e_h2);

    //UPDATE WEIGHS

    weights2[0] = weights2[0] + ( learning_rate * e_o * outputs_l1[0]);
    weights2[1] = weights2[1] + ( learning_rate * e_o * outputs_l1[1]);

    weights[0][0] = weights[0][0] + ( learning_rate * e_h1 * inputs[0]);
    weights[0][1] = weights[0][1] + ( learning_rate * e_h1 * inputs[1]);

    weights[1][0] = weights[1][0] + ( learning_rate * e_h2 * inputs[0]);
    weights[1][1] = weights[1][1] + ( learning_rate * e_h2 * inputs[1]);

    printf("%f\n", weights2[0]);
    printf("%f\n", weights2[1]);

    printf("%f\n", weights[0][0]);
    printf("%f\n", weights[0][1]);
    printf("%f\n", weights[1][0]);
    printf("%f\n", weights[1][1]);




}

void neural_net_test_3b1b(){

    




}

int main(){





    //two_dim_perceptron_test(train_data, test_data, TRAIN_DATA_LEN, TEST_DATA_LEN, 450);

    //neural_net_test(1);

}
