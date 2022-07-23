#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int mean1 = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / (float) 3);
            image[i][j].rgbtBlue = mean1;
            image[i][j].rgbtRed = mean1;
            image[i][j].rgbtGreen = mean1;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    // interating rows and cols
    for (int row = 0; row < height; row ++)
    {
        for (int col = 0; col < width; col++)
        {
            int dividend = 0;
            float R = 0, G = 0, B = 0;
            int change_row[] = {row - 1, row, row + 1};
            int change_col[] = {col - 1, col, col + 1};
            
            //iterating 3x3 matrix
            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int changed_r = change_row[r];
                    int changed_c = change_col[c];
                    if (changed_r >= 0 && changed_r < height && changed_c >= 0 && changed_c < width)
                    {
                        RGBTRIPLE pixel = image[changed_r][changed_c];
                        B += pixel.rgbtBlue;
                        G += pixel.rgbtGreen;
                        R += pixel.rgbtRed;
                        dividend++;
                    }
                }
            }
            temp[row][col].rgbtBlue = round(B /  dividend);
            temp[row][col].rgbtGreen = round(G / dividend);
            temp[row][col].rgbtRed = round(R / dividend);
        }
    }
      
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    //gx matrix
    int gx[3][3] = 
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    //gy matrix
    int gy[3][3] = 
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    // interating rows and cols
    for (int row = 0; row < height; row ++)
    {
        for (int col = 0; col < width; col++)
        {
            int change_row[] = {row - 1, row, row + 1};
            int change_col[] = {col - 1, col, col + 1};
            int totalRx = 0, totalBx = 0, totalGx = 0;
            int totalRy = 0, totalBy = 0, totalGy = 0;
            int gxy[3] = {0, 0, 0};
            
            //iterating 3x3 matrix
            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int changed_r = change_row[r];
                    int changed_c = change_col[c];
                    if (changed_r >= 0 && changed_r < height && changed_c >= 0 && changed_c < width)
                    {
                        RGBTRIPLE pixel = image[changed_r][changed_c];
                        //totalx stands for the total sum of gx; same goes for totaly
                        totalRx += pixel.rgbtRed * gx[r][c];
                        totalBx += pixel.rgbtBlue * gx[r][c];
                        totalGx += pixel.rgbtGreen * gx[r][c];
                      
                        totalRy += pixel.rgbtRed * gy[r][c];
                        totalBy += pixel.rgbtBlue * gy[r][c];
                        totalGy += pixel.rgbtGreen * gy[r][c];
                    }
                }
            }
            //coputing gx squared plus gy squared and taking square root
            gxy[0] = round(sqrt(totalRx * totalRx + totalRy * totalRy));
            gxy[1] = round(sqrt(totalBx * totalBx + totalBy * totalBy));
            gxy[2] = round(sqrt(totalGx * totalGx + totalGy * totalGy));
            
            for (int x = 0; x < 3; x++)
            {
                if (gxy[x] > 255)
                {
                    gxy[x] = 255;
                }
            }
            //temporary storing new pixels value
            temp[row][col].rgbtRed = gxy[0];
            temp[row][col].rgbtBlue = gxy[1];
            temp[row][col].rgbtGreen = gxy[2];
            
            
        }
    }
      
    //stroning new pixel values in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;

}

