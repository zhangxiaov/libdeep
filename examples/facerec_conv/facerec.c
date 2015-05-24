/*
 Face recognition demo
 Copyright (C) 2013-2015  Bob Mottram <bob@robotics.uk.to>

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 3. Neither the name of the University nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.
 .
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE HOLDERS OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include "libdeep/globals.h"
#include "libdeep/deeplearn.h"
#include "libdeep/deeplearn_images.h"
#include "libdeep/deepconvnet.h"

#define TITLE "Face Recognition"

/* the dimensions of each face image */
int image_width = 32;
int image_height = 32;

deepconvnet convnet;

deeplearn_conv convolution;
deeplearn learner;

/**
* @brief Train the deep learner
*/
static void facerec_training()
{
    int no_of_convolutions = 2;
    int no_of_deep_layers = 2;
    int max_features_per_convolution = 20;
    int reduction_factor = 2;
    int no_of_outputs = 5*5;
    int output_classes = 25;
    float error_threshold[] = { 0.001, 0.001, 0.001, 0.001, 0.001 };
    unsigned int random_seed = 34217;

    if (deepconvnet_read_images("../facerec/images",
                                &convnet,
                                image_width, image_height,
                                no_of_convolutions,
                                max_features_per_convolution,
                                reduction_factor,
                                no_of_deep_layers,
                                no_of_outputs,
                                output_classes,
                                error_threshold,
                                &random_seed) != 0) {
        return;
    }

    printf("Number of images: %d\n", convnet.no_of_images);
    printf("Number of labeled training examples: %d\n",convnet.no_of_images*8/10);
    printf("Number of test examples: %d\n",convnet.no_of_images*2/10);

    if (convnet.no_of_images == 0) {
        return;
    }

    deepconvnet_set_learning_rate(&convnet, 0.2f);

    deepconvnet_set_dropouts(&convnet, 0.001f);

    convnet.history_plot_interval = 900000;

    sprintf(convnet.history_plot_title, "%s", TITLE);

    while (deepconvnet_training(&convnet) != 0) {
    }

    printf("Training Completed\n");

    /*printf("Test data set performance is %.1f%%\n",
      deeplearndata_get_performance_convnet(&convnet));*/

    deepconvnet_free(&convnet);
}

/**
* @brief Main function
*/
int main(int argc, char* argv[])
{
    facerec_training();
    return 0;
}
