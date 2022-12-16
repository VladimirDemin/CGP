#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}
// https://russianblogs.com/article/9236915207/
// https://www.geeksforgeeks.org/python-thresholding-techniques-using-opencv-set-1-simple-thresholding/
// https://docs.opencv.org/4.x/dc/dd3/tutorial_gausian_median_blur_bilateral_filter.html
// (поиск линий) https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html



void MainWindow::on_pushButton_clicked()
{
    QString str;
    double Rho,Theta;
    int threshold;
    str = ui->lineEdit_13->text();
    Rho =str.toDouble();
    str = ui->lineEdit_14->text();
    Theta =str.toDouble();
    str = ui->lineEdit_15->text();
    threshold =str.toDouble();
     Mat dst, cdst;
     Mat src = imread(path, IMREAD_GRAYSCALE );
     if(src.empty()){
         ui->label_25->setText("Please choose image!");
             return;
         }
     cv::resize(src, src, Size(1280, 720), INTER_LINEAR);
     Canny(src, dst, 50, 200, 3);
     vector<Vec2f> lines;
     HoughLines(dst, lines, Rho,Theta,threshold);
     imshow("Hough Line Transform", dst);
}
// (поиск точек) https://docs.opencv.org/3.4/d2/d2c/tutorial_sobel_derivatives.html
void MainWindow::on_pushButton_2_clicked()
{
    Mat src, src_gray;
    int thresh,blockSize,apertureSize;
    double k;
    QString str;
    str = ui->lineEdit_16->text();
    thresh =str.toInt();
    str = ui->lineEdit_17->text();
    blockSize =str.toInt();
    str = ui->lineEdit_19->text();
    apertureSize =str.toInt();
    str = ui->lineEdit_18->text();
    k =str.toDouble();
    src = imread( path );
    if(src.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    cv::resize(src, src, Size(1280, 720), INTER_LINEAR);
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    Mat dst = Mat::zeros( src.size(), CV_32FC1 );
    cornerHarris( src_gray, dst, blockSize, apertureSize, k );
    Mat dst_norm, dst_norm_scaled;
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );
    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            if( (int) dst_norm.at<float>(i,j) > thresh )
            {
                circle( dst_norm_scaled, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
            }
        }
    }
    imshow( "Points", dst_norm_scaled );
}

void MainWindow::on_pushButton_3_clicked()
{
    QString str;
    double lower_bound,upper_bound;
    str = ui->lineEdit_11->text();
    lower_bound =str.toDouble();
    str = ui->lineEdit_12->text();
    upper_bound =str.toDouble();
    Mat g_srcImage, g_srcGrayImage, g_dstImage;
    Mat g_cannyDetectedEdges;
    g_srcImage = imread(path);
    if(g_srcImage.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    cv::resize(g_srcImage, g_srcImage, Size(1280, 720), INTER_LINEAR);
    g_dstImage.create(g_srcImage.size(), g_srcImage.type());
    cvtColor(g_srcImage, g_srcGrayImage, COLOR_BGRA2GRAY);
    blur(g_srcGrayImage, g_cannyDetectedEdges, Size(3, 3));
    Canny(g_cannyDetectedEdges, g_cannyDetectedEdges, lower_bound, upper_bound);
    g_dstImage = Scalar::all(0);
    g_srcImage.copyTo(g_dstImage, g_cannyDetectedEdges);
    imshow("Canny edge", g_dstImage);
}
void MainWindow::on_pushButton_4_clicked()
{
    QString str;
    int dx,dy,kernel_size;
    double scale,delta;
    str = ui->lineEdit_6->text();
    dx =str.toInt();
    str = ui->lineEdit_7->text();
    dy =str.toInt();
    str = ui->lineEdit_10->text();
    kernel_size =str.toInt();
    str = ui->lineEdit_9->text();
    scale =str.toDouble();
    str = ui->lineEdit_8->text();
    delta =str.toDouble();
    Mat g_srcImage, g_srcGrayImage, g_dstImage;
    Mat g_sobelGradient_X, g_sobelGradient_Y;
    Mat g_sobelAbsGradient_X, g_sobelAbsGradient_Y;
    g_srcImage = imread(path);
    if(g_srcImage.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    cv::resize(g_srcImage, g_srcImage, Size(1280, 720), INTER_LINEAR);
    g_dstImage.create(g_srcImage.size(), g_srcImage.type());
    cvtColor(g_srcImage, g_srcGrayImage, COLOR_BGRA2GRAY);
    Sobel(g_srcImage, g_sobelGradient_X, CV_16S, dx, 0,kernel_size, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(g_sobelGradient_X, g_sobelAbsGradient_X);
    Sobel(g_srcImage, g_sobelGradient_Y, CV_16S, 0, dy,kernel_size, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(g_sobelGradient_Y, g_sobelAbsGradient_Y);
    addWeighted(g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, g_dstImage);
    imshow("Sobel edge", g_dstImage);



    //меньше действий но результат хуже
    /*
    Mat g_srcImage, g_srcGrayImage, end;
    g_srcImage = imread(path);
    cvtColor(g_srcImage, g_srcGrayImage, COLOR_BGRA2GRAY);
    Sobel(g_srcImage, end, CV_16S, dx, dy,kernel_size, scale, delta, BORDER_DEFAUL);
    convertScaleAbs(end, end);
    imshow("Sobel edge", end);
     */

}

void MainWindow::on_pushButton_5_clicked()
{
    QString str;
    int dx,dy;
    double scale,delta;
    str = ui->lineEdit_6->text();
    dx =str.toInt();
    str = ui->lineEdit_7->text();
    dy =str.toInt();
    str = ui->lineEdit_9->text();
    scale =str.toDouble();
    str = ui->lineEdit_8->text();
    delta =str.toDouble();
    Mat g_srcImage, g_srcGrayImage, g_dstImage;
    Mat g_scharrGradient_X, g_scharrGradient_Y;
    Mat g_scharrAbsGradient_X, g_scharrAbsGradient_Y;
    g_srcImage = imread(path);
    if(g_srcImage.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    cv::resize(g_srcImage, g_srcImage, Size(1280, 720), INTER_LINEAR);
    g_dstImage.create(g_srcImage.size(), g_srcImage.type());
    cvtColor(g_srcImage, g_srcGrayImage, COLOR_BGRA2GRAY);
    Scharr(g_srcImage, g_scharrGradient_X, CV_16S, dx, 0, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(g_scharrGradient_X, g_scharrAbsGradient_X);
    Scharr(g_srcImage, g_scharrGradient_Y, CV_16S, 0, dy, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(g_scharrGradient_Y, g_scharrAbsGradient_Y);
    addWeighted(g_scharrAbsGradient_X, 0.5, g_scharrAbsGradient_Y, 0.5, 0, g_dstImage);
    imshow("Shcarr edge", g_dstImage);


    //меньше действий но результат хуже
    /*
    Mat g_srcImage, g_srcGrayImage, end;
    g_srcImage = imread(path);
    cvtColor(g_srcImage, g_srcGrayImage, COLOR_BGRA2GRAY);
    Scharr(g_srcImage, end, CV_16S, dx, dy,scale,delta, BORDER_DEFAULT);
    convertScaleAbs(end, end);
    imshow("Sobel edge", end);
     */
}

void MainWindow::on_pushButton_6_clicked()
{
    QString str;
    double x,z;
    int y;
    str = ui->lineEdit_3->text();
    x = str.toDouble();
    str = ui->lineEdit_4->text();
    y = str.toInt();
    str = ui->lineEdit_5->text();
    z = str.toDouble();
    Mat img = imread(path,0);
    if(img.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    Mat src;
    cv::resize(img, img, Size(1280, 720), INTER_LINEAR);
    medianBlur(img,src,5);
    Mat th;
    adaptiveThreshold(src,th,x,ADAPTIVE_THRESH_MEAN_C,\
                THRESH_BINARY,y,z);
    imshow("Adaptive Mean threshold",th);
}

void MainWindow::on_pushButton_7_clicked()
{
    QString str;
    double x,y;
    str = ui->lineEdit->text();
    x = str.toDouble();
    str = ui->lineEdit_2->text();
    y = str.toDouble();
    Mat src = imread(path);
    if(src.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    Mat dst;
    cv::resize(src, src, Size(1280, 720), INTER_LINEAR);
    threshold(src,dst,x,y, THRESH_BINARY);
    imshow("Binary threshold", dst);
    vector<Mat> bgr_planes;
    split(dst, bgr_planes );
    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;
    Mat b_hist, g_hist, r_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }
    imshow("Binary hist", histImage );
}

void MainWindow::on_pushButton_11_clicked()
{
    QString str;
    double x,y;
    str = ui->lineEdit->text();
    x = str.toDouble();
    str = ui->lineEdit_2->text();
    y = str.toDouble();
    Mat src = imread(path);
    if(src.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    Mat dst;
    cv::resize(src, src, Size(1280, 720), INTER_LINEAR);
    threshold(src,dst,x,y, THRESH_BINARY_INV);
    imshow("Inverted binary threshold", dst);
    vector<Mat> bgr_planes;
    split(dst, bgr_planes );
    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;
    Mat b_hist, g_hist, r_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }
    imshow("Inverted binary hist", histImage );
}

void MainWindow::on_pushButton_10_clicked()
{
    QString str;
    double x,y;
    str = ui->lineEdit->text();
    x = str.toDouble();
    str = ui->lineEdit_2->text();
    y = str.toDouble();
    Mat src = imread(path);
    if(src.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    Mat dst;
    cv::resize(src, src, Size(1280, 720), INTER_LINEAR);
    threshold(src,dst,x,y, THRESH_TRUNC);
    imshow("Truncated threshold", dst);
    vector<Mat> bgr_planes;
    split(dst, bgr_planes );
    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;
    Mat b_hist, g_hist, r_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }
    imshow("Truncated hist", histImage );
}

void MainWindow::on_pushButton_8_clicked()
{
    QString str;
    double x,y;
    str = ui->lineEdit->text();
    x = str.toDouble();
    str = ui->lineEdit_2->text();
    y = str.toDouble();
    Mat src = imread(path);
    if(src.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    Mat dst;
    cv::resize(src, src, Size(1280, 720), INTER_LINEAR);
    threshold(src,dst,x,y, THRESH_TOZERO);
    imshow("To Zero threshold", dst);
    vector<Mat> bgr_planes;
    split(dst, bgr_planes );
    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;
    Mat b_hist, g_hist, r_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }
    imshow("To zero hist", histImage );
}

void MainWindow::on_pushButton_9_clicked()
{
    QString str;
    double x,y;
    str = ui->lineEdit->text();
    x = str.toDouble();
    str = ui->lineEdit_2->text();
    y = str.toDouble();
    Mat src = imread(path);
    if(src.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    Mat dst;
    cv::resize(src, src, Size(1280, 720), INTER_LINEAR);
    threshold(src,dst,x,y, THRESH_TOZERO_INV);
    imshow("To Zero inverted threshold", dst);
    vector<Mat> bgr_planes;
    split(dst, bgr_planes );
    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;
    Mat b_hist, g_hist, r_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }
    imshow("To zero inverted hist", histImage );
}

void MainWindow::on_pushButton_13_clicked()
{
    QString str;
    double x,y;
    str = ui->lineEdit->text();
    x = str.toDouble();
    str = ui->lineEdit_2->text();
    y = str.toDouble();
    Mat src = imread(path,IMREAD_GRAYSCALE);
    if(src.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    Mat dst;
    cv::resize(src, src, Size(1280, 720), INTER_LINEAR);
    threshold(src,dst,x,y,THRESH_BINARY+THRESH_OTSU);
    imshow("Threshold with Otsu", dst);
    imwrite("tmp.jpg",dst);
    Mat img = imread("tmp.jpg");
    vector<Mat> bgr_planes;
    split(img, bgr_planes );
    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;
    Mat b_hist, g_hist, r_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }
    imshow("Otsu hist", histImage );
}

void MainWindow::on_pushButton_14_clicked()
{
    QString str;
    double x,y;
    str = ui->lineEdit->text();
    x = str.toDouble();
    str = ui->lineEdit_2->text();
    y = str.toDouble();
    Mat src = imread(path,IMREAD_GRAYSCALE);
    if(src.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    Mat dst;
    cv::resize(src, src, Size(1280, 720), INTER_LINEAR);
    threshold(src,dst,x,y,THRESH_BINARY+THRESH_TRIANGLE);
    imshow("Threshold with Triangle", dst);
    imwrite("tmp.jpg",dst);
    Mat img = imread("tmp.jpg");
    vector<Mat> bgr_planes;
    split(img, bgr_planes );
    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;
    Mat b_hist, g_hist, r_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }
    imshow("Triangle hist", histImage );
}

void MainWindow::on_pushButton_12_clicked()
{

    QString str;
    double x,z;
    int y;
    str = ui->lineEdit_3->text();
    x = str.toDouble();
    str = ui->lineEdit_4->text();
    y = str.toInt();
    str = ui->lineEdit_5->text();
    z = str.toDouble();
    Mat img = imread(path,0);
    if(img.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    Mat src;
    cv::resize(img, img, Size(1280, 720), INTER_LINEAR);
   // medianBlur(img,src,5);
    Mat th;
    adaptiveThreshold(img,th,x,ADAPTIVE_THRESH_GAUSSIAN_C,\
                THRESH_BINARY,y,z);
    imshow("Adaptive Gaussian threshold",th);
}

void MainWindow::on_pushButton_15_clicked()
{
    QString arr[]={".tiff",".gif",".bmp",".png",".jpeg"};
    QString arch;
         QString fileName = QFileDialog ::getOpenFileName(
            this,
            tr("open a file."),
            "D:/",
            tr("images(*tiff *bmp *png *jpeg *jpg)"));
       path = fileName.toStdString();
        ui->label_25->setText("");
}

void MainWindow::on_pushButton_16_clicked()
{
    Mat img = imread(path, COLOR_BGR2RGB);
    if(img.empty()){
        ui->label_25->setText("Please choose image!");
            return;
        }
    cv::resize(img, img, Size(1280, 720), INTER_LINEAR);
    imshow("Original picture",img);
}
