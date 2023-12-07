#include "helpers.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

// DONE, AND WORKS GREAT! - Convert image to grayscale
/* Before we start, we need to remember a few things:
   - The greyscale number must be the average of R, G and B.
   - The values of R, G and B are in inverted order; that is,
     B comes first, followed by G and R.
   - Each RGBTRIPLE item is an 8-bit unsigned integer (uint8_t). */
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // We'll start creating these four local variables.
    uint8_t b;
    uint8_t g;
    uint8_t r;
    int avg;

    // For each item...
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // We'll split the triplet of values.
            b = image[i][j].rgbtBlue;
            g = image[i][j].rgbtGreen;
            r = image[i][j].rgbtRed;

            // Now we can calculate the average of them (rounded down)
            // and assign it back to each member of the triplet.
            avg = ((int) b + (int) g + (int) r) / 3;

            // We must remember to convert 'avg' to 'unint8_t'!!
            image[i][j].rgbtBlue = (uint8_t) avg;
            image[i][j].rgbtGreen = (uint8_t) avg;
            image[i][j].rgbtRed = (uint8_t) avg;
        }
    }
    return;
}

// DONE, AND WORKS LIKE A CHARM :) - Reflect image horizontally
/* Just a few warnings:
   - The 'j' index that we introduced in 'grayscale' must NOT exceed 'width / 2'.
   - We will need a temporary variable to swap pixels. */
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Here are some local variables.
    // The second one will help stop the inner 'for' loop.
    RGBTRIPLE rgbtemp;
    int half_width = width / 2;

    // For each pixel...
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half_width; j++)
        {
            // ... we swap places.
            rgbtemp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = rgbtemp;
        }
    }
    return;
}

// DONE, AND WORKS WELL! ;) - Blur image
/* We will need to work with a 3x3 array whose centre goes
   from top to bottom and from left to right. */
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Let's create and initialise that local array.
    // It may look like we're hardcoding here, but there's no need
    // to worry since the blurring procedure works with 3x3 arrays.
    RGBTRIPLE arr_blur[3][3];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            arr_blur[i][j].rgbtBlue = 0;
            arr_blur[i][j].rgbtGreen = 0;
            arr_blur[i][j].rgbtRed = 0;
        }
    }

    // We now need an 'RGBTRIPLE' array identical in shape to 'image'.
    RGBTRIPLE blurred[height][width];

    // We also need three 'int' variables: one for blue, one for green and one for red.
    int b;
    int g;
    int r;

    // Time to blur 'image'!
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            b = 0;
            g = 0;
            r = 0;
            /* We need to consider all possible cases (a total of 9). */
            // CASE 1 - Top-left corner.
            if ((i == 0) && (j == 0))
            {
                b = ((int) image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 4;
                g = ((int) image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 4;
                r = ((int) image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 4;
            }
            // CASE 2 - Top edge.
            else if ((i == 0) && ((j > 0) && (j < width - 1)))
            {
                b = ((int) image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 6;
                g = ((int) image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 6;
                r = ((int) image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 6;
            }
            // CASE 3 - Top-right corner.
            else if ((i == 0) && (j == width - 1))
            {
                b = ((int) image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue) / 4;
                g = ((int) image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen) / 4;
                r = ((int) image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed) / 4;
            }
            // CASE 4 - Left edge.
            else if (((i > 0) && (i < height - 1)) && (j == 0))
            {
                b = ((int) image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 6;
                g = ((int) image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 6;
                r = ((int) image[i - 1][j].rgbtRed + image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed) / 6;
            }
            // CASE 5 - Neither edge nor corner.
            else if (((i > 0) && (i < height - 1)) && ((j > 0) && (j < width - 1)))
            {
                b = ((int) image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue) / 9;
                g = ((int) image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen) / 9;
                r = ((int) image[i - 1][j].rgbtRed + image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed) / 9;
            }
            // CASE 6 - Right edge.
            else if (((i > 0) && (i < height - 1)) && (j == width - 1))
            {
                b = ((int) image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue) / 6;
                g = ((int) image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen) / 6;
                r = ((int) image[i - 1][j].rgbtRed + image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed) / 6;
            }
            // CASE 7 - Bottom-left corner.
            else if ((i == height - 1) && (j == 0))
            {
                b = ((int) image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue) / 4;
                g = ((int) image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen) / 4;
                r = ((int) image[i - 1][j].rgbtRed + image[i][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed) / 4;
            }
            // CASE 8 - Bottom edge.
            else if ((i == height - 1) && ((j > 0) && (j < width - 1)))
            {
                b = ((int) image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i][j - 1].rgbtBlue) / 6;
                g = ((int) image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i][j - 1].rgbtGreen) / 6;
                r = ((int) image[i - 1][j].rgbtRed + image[i][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed) / 6;
            }
            // CASE 9 - Bottom-right corner.
            else if ((i == height - 1) && (j == width - 1))
            {
                b = ((int) image[i - 1][j].rgbtBlue + image[i][j].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i][j - 1].rgbtBlue) / 4;
                g = ((int) image[i - 1][j].rgbtGreen + image[i][j].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i][j - 1].rgbtGreen) / 4;
                r = ((int) image[i - 1][j].rgbtRed + image[i][j].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed) / 4;
            }
            blurred[i][j].rgbtBlue = (uint8_t) b;
            blurred[i][j].rgbtGreen = (uint8_t) g;
            blurred[i][j].rgbtRed = (uint8_t) r;
        }
    }

    // It is now safe to modify 'image'.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred[i][j];
        }
    }
    return;
}

// DONE - Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Let's start creating an 'RGBTRIPLE' array that will store the resulting values.
    RGBTRIPLE output[height][width];

    // We'll also need two 3x3 arrays that describe the edge detection procedure.
    int gx[3][3];
    int gy[3][3];
    gx[0][0] = -1;
    gx[1][0] = -2;
    gx[2][0] = -1;
    gx[0][1] = 0;
    gx[1][1] = 0;
    gx[2][1] = 0;
    gx[0][2] = 1;
    gx[1][2] = 2;
    gx[2][2] = 1;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            gy[i][j] = gx[j][i];
        }
    }

    // Another one will have its central item at [i][j] in 'output'.
    RGBTRIPLE crop[3][3];

    // The following variable will store the result of the edge detection procedure.
    RGBTRIPLE result;

    // These ones are needed to calculate the output.
    long bx, by, grx, gry, rx, ry, bl, gr, rd;
    int blue, green, red;

    // Now, for each pixel of 'image', we'll calculate the output.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // First, we'll initialise 'crop', an auxiliary array.
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    crop[k][l].rgbtBlue = (uint8_t) 0;
                    crop[k][l].rgbtGreen = (uint8_t) 0;
                    crop[k][l].rgbtRed = (uint8_t) 0;
                }
            }

            // Let's fetch the appropriate section from 'image' and copy it to 'crop'.
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    // The following set of conditions prevents the loops from stepping out of boundaries.
                    if((i + k >= 0) && (i + k < height) && (j + l >= 0) && (j + l < width))
                    {
                        crop[k + 1][l + 1] = image[i + k][j + l];
                    }
                }
            }

            // I'll now need six variables:
            // - Three for 'gx' and three for 'gy'.
            // - One for each (colour, array) pair.
            // They'll all be 'long' just to stay safe.
            bx = 0;
            grx = 0;
            rx = 0;
            by = 0;
            gry = 0;
            ry = 0;

            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    bx = bx + (gx[k][l] * crop[k][l].rgbtBlue);
                    by = by + (gy[k][l] * crop[k][l].rgbtBlue);
                    grx = grx + (gx[k][l] * crop[k][l].rgbtGreen);
                    gry = gry + (gy[k][l] * crop[k][l].rgbtGreen);
                    rx = rx + (gx[k][l] * crop[k][l].rgbtRed);
                    ry = ry + (gy[k][l] * crop[k][l].rgbtRed);
                }
            }

            // We're close!
            // Now let's multiply each 'long' variable by itself
            // and add each channel's values together.
            bl = bx * bx + by * by;
            gr = grx * grx + gry * gry;
            rd = rx * rx + ry * ry;

            // What we need to do now is get the square root of 'bl', 'gr' and 'rd'
            // and convert them to integer numbers (rounded down).
            // Remember that <math.h>'s 'sqrt' function requires and returns a 'double'.
            blue = (int) sqrt((double) bl);
            green = (int) sqrt((double) gr);
            red = (int) sqrt((double) rd);

            // Time to cap these numbers if they exceed 255.
            if (blue > 255)
            {
                blue = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (red > 255)
            {
                red = 255;
            }

            // And, at last, let's define 'result' and copy it to 'output'.
            // Remember to convert to 'uint8_t'!
            result.rgbtBlue = (uint8_t) blue;
            result.rgbtGreen = (uint8_t) green;
            result.rgbtRed = (uint8_t) red;

            output[i][j] = result;
        }
    }

    // Finally, we replace the pixels in 'image' with those from 'output'.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = output[i][j];
        }
    }
    return;
}
