#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <chrono>
#include <thread>

using namespace std;

#define ENABLE_LOG 1
#define LOG(msg) std::cout << msg
#define LOGLN(msg) std::cout << msg << std::endl

using namespace cv;
using namespace std;

int camera_no = 0;
int frameCount = 0;

static int camera_feed (int argc, char** argv) 
{
    for (int i = 1; i < argc; ++i)
    {
        if (string(argv[i]) == "--camerano")
        {
            //initialization of variables
            camera_no = stoi(argv[i + 1]);
            vector<VideoCapture> cam(camera_no);
            vector<string> folderPath(camera_no);
            vector<string> filename(camera_no);
            vector<Mat> frame(camera_no);
            vector<bool> success(camera_no);

            //set fps
            int fps = 10;
            // Calculate the time interval between each frame capture
            double interval_ms = 1000.0 / fps; // Interval in milliseconds

            //for debugging
            int counter = 0;
            bool check = true;

            //initialization of cameras
            for (int j = 0; j < camera_no; ++j)
            {
                cam[j].open(j);

                if (!cam[j].isOpened()) 
                {
                    cout << "Error: Could not open camera " << to_string(j) <<".\n" << endl;
                    return -1;
                }

                folderPath[j] = "C:/Users/admin/Desktop/feed/frames" + to_string(j) + "/";  // Specify the folder where frames will be saved
            }

            auto start_loop_time = std::chrono::high_resolution_clock::now();

            //actual parsing
            while (check){
                auto start_time = std::chrono::high_resolution_clock::now();

                //read and save current frame for all cameras
                for (int j = 0; j < camera_no; ++j)
                {
                    if (j == 1) {
                        stringstream arguments;
                        string cont_str, programPath;
                        int cont;

                        arguments << "--frameno 1" << " " << camera_no << " preview1.jpg --features akaze --conf_thresh 0.1 --match_conf 0.1 --warp mercator";

                        // Convert the stringstream to a string
                        string argumentString = arguments.str();
                        programPath = "C:\\Users\\admin\\Desktop\\footagecapture\\build\\Debug\\footagecapture.exe";
                        // Execute the program with the arguments
                        string command = programPath + " " + argumentString;
                        cout << "Executing command: " << command << endl;
                        system(command.c_str());

                        if(i == 1){
                            cout << "1st frame produced. Continue? (1/0)" << endl;
                            getline(cin, cont_str);
                                if(cont_str.length() == 0){
                                    std::cout << "No input provided. Proceeding with stitching..." << endl;
                                }

                                else {
                                    cont = stoi(cont_str);
                                    if (cont)
                                        std::cout << "Proceeding with stitching videos.." << endl;
                                    else{
                                        std::cout << "Stopping stiching..." << endl;
                                        return 1;
                                    }
                                }
                        }
                    }
                    
                    success[j] = cam[j].read(frame[j]);
                    if (!success[j]) 
                    {
                        cout << "Error: Cannot read frame from camera " << to_string(j) <<".\n" << endl;
                        break;
                    }

                    //saves all frame 1s first to respective folders
                    filename[j] = folderPath[j] + "frame" + to_string(frameCount) + ".jpg";
                    imwrite(filename[j], frame[j]);
                    cout << "Saved frame " << frameCount << " as " << filename[j] << endl;

                    
                    
                }
                
                auto end_time = std::chrono::high_resolution_clock::now();

                // Calculate the time taken for the function call
                double elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

                // Wait for the remaining time to achieve the desired FPS
                if (elapsed_ms < interval_ms) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(interval_ms - elapsed_ms)));
                }
                //next frame
                frameCount++;
                counter++;

                if(counter == 10)
                {
                    check = false;
                    auto end_loop_time = std::chrono::high_resolution_clock::now();
                    double elapsed_loop_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_loop_time - start_loop_time).count();

                    cout << "Time passed:" << to_string(elapsed_loop_ms) << endl;
                }
            }

            for (int j = 0; j < camera_no; ++j)
            {
                cam[j].release();
            }
            return 0;
        }
    }
    return 0;
}
    /*VideoCapture cam[3]; // Open the default camera (usually webcam)
    cam[1].open(0); 
    cam[2].open(1); // Open the default camera (usually webcam)
    cam[3].open(2); // Open the default camera (usually webcam)

    if (!cam[1].isOpened()) {
        cout << "Error: Could not open camera 1." << endl;
        return -1;
    }
    if (!cam[2].isOpened()) {
        cout << "Error: Could not open camera 2." << endl;
        return -1;
    }
    if (!cam[3].isOpened()) {
        cout << "Error: Could not open camera 3." << endl;
        return -1;
    }

    int frameCount = 0;
    string folderPath1 = "C:/Users/admin/Desktop/feed/camera1/"; // Specify the folder where frames will be saved
    string folderPath2 = "C:/Users/admin/Desktop/feed/camera2/"; // Specify the folder where frames will be saved
    string folderPath3 = "C:/Users/admin/Desktop/feed/camera3/"; // Specify the folder where frames will be saved


    while (true) {
        Mat frame1, frame2, frame3;
        bool success1 = cam[1].read(frame1); // Read a new frame from the camera
        bool success2 = cam[2].read(frame2); // Read a new frame from the camera
        bool success3 = cam[3].read(frame3); // Read a new frame from the camera

        if (!success1) {
            cout << "Error: Cannot read frame from camera 1." << endl;
            break;
        }

        if (!success2) {
            cout << "Error: Cannot read frame from camera 2." << endl;
            break;
        }

        if (!success3) {
            cout << "Error: Cannot read frame from camera 3." << endl;
            break;
        }

        // Save the frame with a unique filename
        string filename1 = folderPath1 + "frame" + to_string(frameCount) + ".jpg";
        string filename2 = folderPath2 + "frame" + to_string(frameCount) + ".jpg";
        string filename3 = folderPath3 + "frame" + to_string(frameCount) + ".jpg";

        imwrite(filename1, frame1);
        cout << "Saved frame " << frameCount << " as " << filename1 << endl;
        imwrite(filename2, frame2);
        cout << "Saved frame " << frameCount << " as " << filename2 << endl;
        imwrite(filename3, frame3);
        cout << "Saved frame " << frameCount << " as " << filename3 << endl;


        frameCount++;

        // Press 'q' to exit the loop
        if (waitKey(1) == 'q') {
            break;
        }
    }

    cam[1].release(); // Release the camera
    cam[2].release();
    cam[3].release();
    destroyAllWindows(); // Close all OpenCV windows

    return 0;
}*/


int main(int argc, char* argv[])
{
    camera_feed(argc, argv);
    return 0;

}