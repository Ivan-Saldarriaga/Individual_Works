public class Project2_Functions {
    static public float[][] KernelProcess(float[][] input, float[][] kernel){
//        float[][] kernel = {
//                {0.0625, 0.125,  0.0625},
//                {0.125,  0.25,   0.125 },
//                {0.0625, 0.125,  0.0625}
//        };
        int originalRows = input.length;
        int originalCols = input[0].length;
        int kernelRows = kernel.length;
        int kernelCols = kernel[0].length;
        //considering its always going to be 3x3 kernel, you can hard code this to be 2
        int padY = kernelRows/2;
        int padX = kernelCols/2;
        int extendedRows = originalRows + 2 * padY;
        int extendedCols = originalCols + 2 * padX;
        //make new 2d array that is the size of the original PLUS edges of padding
        float[][] extendedInput = new float[extendedRows][extendedCols];
        // output array does not include the padding, it was mainly for the convolution part that is coming up
        float[][] output = new float[originalRows][originalCols];
        // fill in all the original values in the new array, skipping the padding
        for (int i = 0; i < originalRows; i++) {
            for (int j = 0; j < originalCols; j++) {
                extendedInput[i + padY][j + padX] = input[i][j];
            }
        }
        // EDGE EXTENSIONS
        // Left and right side by copying the value of the inner section edge to the 2 first and 2 lasat spaces in the extended array (The padding)
        for (int i = 0; i < extendedRows; i++) {
            for (int j = 0; j < padX; j++) {
                extendedInput[i][j] = extendedInput[i][padX];
                extendedInput[i][extendedCols - j - 1] = extendedInput[i][extendedCols - padX - 1];
            }
        }
        // Top and bottom side by copying the value of the inner section edge to the 2 first and 2 lasat spaces in the extended array (The padding)
        for (int i = 0; i < padY; i++) {
            for (int j = 0; j < extendedCols; j++) {
                extendedInput[i][j] = extendedInput[padY][j];
                extendedInput[extendedRows - i - 1][j] = extendedInput[extendedRows - padY - 1][j];
            }
        }
        // do kernel process by multiplying kernel to each value centered aorund an original pixel, and adding them up to insert into the output array
        //loop through each center pixel (original array)
        for (int i = 0; i < originalRows; i++) {
            for (int j = 0; j < originalCols; j++) {
                float sum = 0;
                //loop tho=rough the kernel values to multiply each overlap with the extended array (with padding) and then add to the sum for the final output
                for (int kerneli = 0; kerneli < kernelRows; kerneli++) {
                    for (int kernelj = 0; kernelj < kernelCols; kernelj++) {
                        int xi = i + kerneli;
                        int yj = j + kernelj;
                        sum += (float)extendedInput[xi][yj] * kernel[kerneli][kernelj];
                    }
                }
                output[i][j] = sum;
            }
        }
        return output;
    }


    public static float[][] Divide(float[][] input){
        //first dimensions of terrain array are doubled in size
        int size = input.length * 2;
        float[][] output = new float[size][size];
        for(int x = 0; x < input.length; x++){
            for(int y = 0; y < input.length; y++){
                //place original pixel at 2i 2j and do nearest neighbor for remaing (just keep filling basically for the quad
                output[2 * x][2 * y] = input[x][y];
                //
                output[(2 * x) + 1][2 * y] = input[x][y];
                output[(2 * x)][(2 * y) + 1] = input[x][y];
                output[(2 * x) + 1][(2 * y) + 1] = input[x][y];
            }
        }
        //nearest neighbor (basically neighbor at spot top left in quad?
        return output;
    }
//    public static void main(String[] args) {
//        // Test input for the Divide function
//        float[][] input = {{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,10.0f,0.0f,0.0f,10.0f,10.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f},
//                {0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f},
//                {0.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f},
//                {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}}
//                ;
//
//        // Expected output after dividing the input array
//        float[][] expectedDivideOutput = {{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f},
//                {0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f},
//                {0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f},
//                {0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f},
//                {0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f},
//                {0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f},
//                {0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f},
//                {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}}
//                ;
//
//        // Perform the Divide operation
//        float[][] dividedOutput = Divide(input);
//
//        // Check if Divide output matches expected output
//        boolean dividePassed = true;
//        for (int i = 0; i < dividedOutput.length; i++) {
//            for (int j = 0; j < dividedOutput[0].length; j++) {
//                if (dividedOutput[i][j] != expectedDivideOutput[i][j]) {
//                    dividePassed = false;
//                    System.out.printf("Divide Mismatch at [%d][%d]: got %f, expected %f%n",
//                            i, j, dividedOutput[i][j], expectedDivideOutput[i][j]);
//                }
//            }
//        }
//        System.out.println("Divide function test passed: " + dividePassed);
//
//        // Test input for the KernelProcess function
//        float[][] kernel =
//                {{-1.0f,0.0f,1.0f},
//                        {-1.0f,0.0f,1.0f},
//                        {-1.0f,0.0f,1.0f}};
//        input = new float[][]{{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,10.0f,10.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
//                {0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,0.0f},
//                {0.0f,0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,10.0f,0.0f,0.0f,10.0f,10.0f,0.0f,0.0f,0.0f},
//                {0.0f,0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f},
//                {0.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,10.0f},
//                {0.0f,10.0f,10.0f,0.0f,0.0f,0.0f,0.0f,10.0f,10.0f,10.0f},
//                {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}};
//        // Expected output after applying the kernel
//        float[][] expectedKernelOutput =
//                {{10.0f,10.0f,0.0f,0.0f,-10.0f,-10.0f,0.0f,0.0f,0.0f,0.0f},
//                        {20.0f,20.0f,0.0f,0.0f,-10.0f,-10.0f,0.0f,0.0f,-10.0f,-10.0f},
//                        {20.0f,20.0f,0.0f,10.0f,0.0f,-10.0f,0.0f,0.0f,-20.0f,-20.0f},
//                        {10.0f,10.0f,0.0f,10.0f,20.0f,10.0f,-10.0f,-10.0f,-20.0f,-20.0f},
//                        {0.0f,10.0f,0.0f,0.0f,30.0f,20.0f,-20.0f,-20.0f,-10.0f,-10.0f},
//                        {0.0f,20.0f,10.0f,-10.0f,20.0f,20.0f,-20.0f,-20.0f,0.0f,0.0f},
//                        {10.0f,30.0f,10.0f,-10.0f,10.0f,10.0f,-10.0f,-10.0f,0.0f,0.0f},
//                        {20.0f,30.0f,0.0f,-10.0f,0.0f,0.0f,10.0f,10.0f,0.0f,0.0f},
//                        {20.0f,20.0f,-10.0f,-10.0f,0.0f,0.0f,10.0f,10.0f,0.0f,0.0f},
//                        {10.0f,10.0f,-10.0f,-10.0f,0.0f,0.0f,10.0f,10.0f,0.0f,0.0f}};
//
//        // Perform the KernelProcess operation
//        float[][] kernelOutput = KernelProcess(input, kernel);
//
//        // Check if KernelProcess output matches expected output
//        boolean kernelPassed = true;
//        for (int i = 0; i < kernelOutput.length; i++) {
//            for (int j = 0; j < kernelOutput[0].length; j++) {
//                if (Math.abs(kernelOutput[i][j] - expectedKernelOutput[i][j]) > 0.01) { // Allow a small margin of error
//                    kernelPassed = false;
//                    System.out.printf("Kernel Mismatch at [%d][%d]: got %f, expected %f%n",
//                            i, j, kernelOutput[i][j], expectedKernelOutput[i][j]);
//                }
//            }
//        }
//        System.out.println("KernelProcess function test passed: " + kernelPassed);
//    }


}
