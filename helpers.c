#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avgValue = (int) round((float) (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = avgValue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width / 2; c++)
        {
            RGBTRIPLE space = image[r][c];
            image[r][c] = image[r][width - c - 1];
            image[r][width - c - 1] = space;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            int red = 0;
            int blue = 0;
            int green = 0;
            int count = 0;

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    int row = r + i;
                    int col = p + j;
                    if (row >= 0 && row < height && col >= 0 && col < width)
                    {
                        red += image[row][col].rgbtRed;
                        blue += image[row][col].rgbtBlue;
                        green += image[row][col].rgbtGreen;
                        count++;
                    }
                }
            }

            temp[r][p].rgbtRed = round((float) red / count);
            temp[r][p].rgbtBlue = round((float) blue / count);
            temp[r][p].rgbtGreen = round((float) green / count);
        }
    }

    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            image[r][p] = temp[r][p];
        }
    }

    return;
}

int clamp(int value)
{
    if (value < 0)
    {
        return 0;
    }
    else if (value > 255)
    {
        return 255;
    }
    return value;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    RGBTRIPLE temp[height][width];

    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            int gx_red = 0, gx_green = 0, gx_blue = 0;
            int gy_red = 0, gy_green = 0, gy_blue = 0;

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    int row = r + i;
                    int col = p + j;
                    if (row < 0 || row >= height || col < 0 || col >= width)
                    {
                        continue;
                    }
                    {
                        gx_red += Gx[i + 1][j + 1] * image[row][col].rgbtRed;
                        gx_green += Gx[i + 1][j + 1] * image[row][col].rgbtGreen;
                        gx_blue += Gx[i + 1][j + 1] * image[row][col].rgbtBlue;

                        gy_red += Gy[i + 1][j + 1] * image[row][col].rgbtRed;
                        gy_green += Gy[i + 1][j + 1] * image[row][col].rgbtGreen;
                        gy_blue += Gy[i + 1][j + 1] * image[row][col].rgbtBlue;
                    }
                }
            }

            int red = clamp((int) round(sqrt((double) (gx_red * gx_red + gy_red * gy_red))));
            int green = clamp((int) round(sqrt((double) (gx_green * gx_green + gy_green * gy_green))));
            int blue = clamp((int) round(sqrt((double) (gx_blue * gx_blue + gy_blue * gy_blue))));

            temp[r][p].rgbtRed = red;
            temp[r][p].rgbtGreen = green;
            temp[r][p].rgbtBlue = blue;
        }
    }

    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            image[r][p] = temp[r][p];
        }
    }

    return;
}
