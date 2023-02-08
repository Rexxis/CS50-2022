#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float average; // initiate variable to store average

    // loop through every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calculate the average
            average = (image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3.0;
            // set the all the pixel color to average
            image[i][j].rgbtRed = image[i][j].rgbtBlue = image[i][j].rgbtGreen = round(average);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // loop through every pixel
    for (int i = 0; i < height; i++)
    {
        // loop until half column
        for (int j = 0; j < width / 2; j++)
        {
            // swap the value of the pixel
            RGBTRIPLE swap = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = swap;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create copy of original image
    // original image needed for average calculation so that new values does not used to calculate the average
    RGBTRIPLE originalimage[height][width];
    for (int temprow = 0; temprow < height; temprow++)
    {
        for (int tempcol = 0; tempcol < width; tempcol++)
        {
            originalimage[temprow][tempcol] = image[temprow][tempcol];
        }
    }

    // calculates the box average
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initiate temporary variables to store the sum value
            int red = 0;
            int blue = 0;
            int green = 0;
            int pixelcount = 0; // saves the number of pixel for average calculation

            for (int boxrow = i - 1; boxrow < i + 2; boxrow++)
            {
                for (int boxcol = j - 1; boxcol < j + 2; boxcol++)
                {
                    // check if row and columns are within the image boundary (height and width)
                    // if the pixel is out of the boundary it will not be counted
                    if ((boxrow >= 0 && boxrow < height) && (boxcol >= 0 && boxcol < width))
                    {
                        // append each color
                        red += originalimage[boxrow][boxcol].rgbtRed;
                        blue += originalimage[boxrow][boxcol].rgbtBlue;
                        green += originalimage[boxrow][boxcol].rgbtGreen;
                        pixelcount += 1; // increment the pixel count
                    }
                }
            }

            // assign the average values to the image pixel
            image[i][j].rgbtRed = round(red / (float) pixelcount);
            image[i][j].rgbtBlue = round(blue / (float) pixelcount);
            image[i][j].rgbtGreen = round(green / (float) pixelcount);

        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create copy of original image
    RGBTRIPLE originalimage[height][width];
    for (int temprow = 0; temprow < height; temprow++)
    {
        for (int tempcol = 0; tempcol < width; tempcol++)
        {
            originalimage[temprow][tempcol] = image[temprow][tempcol];
        }
    }
    // create matrix for sobel kernel
    int gx[3][3] =  {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initiate variables for Gx
            int gxred = 0;
            int gxgreen = 0;
            int gxblue = 0;

            // initiate variables for Gy
            int gyred = 0;
            int gygreen = 0;
            int gyblue = 0;

            // box loop
            for (int boxrow = -1; boxrow < 2; boxrow++)
            {
                for (int boxcol = -1; boxcol < 2; boxcol++)
                {
                    // check for boundary
                    if (i + boxrow < 0 || i + boxrow > height - 1 || j + boxcol < 0 || j + boxcol > width - 1)
                    {
                        continue;
                    }
                    // sum each channel values to respective gx and gy
                    gxred += originalimage[i + boxrow][j + boxcol].rgbtRed * gx[boxrow + 1][boxcol + 1];
                    gxgreen += originalimage[i + boxrow][j + boxcol].rgbtGreen * gx[boxrow + 1][boxcol + 1];
                    gxblue += originalimage[i + boxrow][j + boxcol].rgbtBlue * gx[boxrow + 1][boxcol + 1];

                    gyred += originalimage[i + boxrow][j + boxcol].rgbtRed * gy[boxrow + 1][boxcol + 1];
                    gygreen += originalimage[i + boxrow][j + boxcol].rgbtGreen * gy[boxrow + 1][boxcol + 1];
                    gyblue += originalimage[i + boxrow][j + boxcol].rgbtBlue * gy[boxrow + 1][boxcol + 1];
                }
            }

            // get the square root of each channel sum
            float sqrtRed = sqrt((gxred * gxred) + (gyred * gyred));
            float sqrtGreen = sqrt((gxgreen * gxgreen) + (gygreen * gygreen));
            float sqrtBlue = sqrt((gxblue * gxblue) + (gyblue * gyblue));

            // cap the max value to 255
            if (sqrtRed > 255)
            {
                sqrtRed = 255;
            }
            if (sqrtGreen > 255)
            {
                sqrtGreen = 255;
            }
            if (sqrtBlue > 255)
            {
                sqrtBlue = 255;
            }

            // update the image pixel to new value
            image[i][j].rgbtRed = round(sqrtRed);
            image[i][j].rgbtGreen = round(sqrtGreen);
            image[i][j].rgbtBlue = round(sqrtBlue);

        }
    }
    return;
}