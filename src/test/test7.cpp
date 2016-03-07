
-(void)featureDetection:(UIImage*)largerImage withImage:(UIImage*)subImage
{
    cv::Mat tempMat1 = [largerImage CVMat];
    cv::Mat tempMat2 = [subImage CVMat];

    cv::cvtColor(tempMat1, tempMat1, CV_RGB2GRAY);
    cv::cvtColor(tempMat2, tempMat2, CV_RGB2GRAY);

    if( !tempMat1.data || !tempMat2.data ) {
        return;
    }

    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 25;

    cv::SurfFeatureDetector detector( minHessian ); // More Accurate bt take more time..
    //cv::FastFeatureDetector detector( minHessian ); //Less Accurate bt take less time..

    std::vector<cv::KeyPoint> keypoints_1, keypoints_2;

    detector.detect( tempMat1, keypoints_1 );
    detector.detect( tempMat2, keypoints_2 );

    //-- Step 2: Calculate descriptors (feature vectors)
    cv::SurfDescriptorExtractor extractor;

    cv::Mat descriptors_1, descriptors_2;

    extractor.compute( tempMat1, keypoints_1, descriptors_1 );
    extractor.compute( tempMat2, keypoints_2, descriptors_2 );

    std::vector<cv::Point2f> obj_corners(4);

    //Get the corners from the object
    obj_corners[0] = (cvPoint(0,0));
    obj_corners[1] = (cvPoint(tempMat2.cols,0));
    obj_corners[2] = (cvPoint(tempMat2.cols,tempMat2.rows));
    obj_corners[3] = (cvPoint(0, tempMat2.rows));

    //-- Step 3: Matching descriptor vectors with a brute force matcher
    //cv::BruteForceMatcher < cv::L2<float> > matcher;
    cv::FlannBasedMatcher matcher;
    //std::vector< cv::DMatch > matches;
    std::vector<cv::vector<cv::DMatch > > matches;

    std::vector<cv::DMatch > good_matches;
    std::vector<cv::Point2f> obj;
    std::vector<cv::Point2f> scene;
    std::vector<cv::Point2f> scene_corners(4);
    cv::Mat H;

    matcher.knnMatch( descriptors_2, descriptors_1, matches,2);

    for(int i = 0; i < cv::min(tempMat1.rows-1,(int) matches.size()); i++)  {

        if((matches[i][0].distance < 0.6*(matches[i][1].distance)) && ((int) matches[i].size()<=2 && (int) matches[i].size()>0))  {
            good_matches.push_back(matches[i][0]);
        }
    }
    cv::Mat img_matches;
    drawMatches( tempMat2, keypoints_2, tempMat1, keypoints_1, good_matches, img_matches );

    NSLog(@"good matches %lu",good_matches.size());

    if (good_matches.size() >= 4)  {

         for( int i = 0; i < good_matches.size(); i++ ) {
             //Get the keypoints from the good matches
             obj.push_back( keypoints_2[ good_matches[i].queryIdx ].pt );
             scene.push_back( keypoints_1[ good_matches[i].trainIdx ].pt );
         }

         H = findHomography( obj, scene, CV_RANSAC );

         perspectiveTransform( obj_corners, scene_corners, H);

         NSLog(@"%f %f",scene_corners[0].x,scene_corners[0].y);
         NSLog(@"%f %f",scene_corners[1].x,scene_corners[1].y);
         NSLog(@"%f %f",scene_corners[2].x,scene_corners[2].y);
         NSLog(@"%f %f",scene_corners[3].x,scene_corners[3].y);


         //Draw lines between the corners (the mapped object in the scene image )
         line( tempMat1, scene_corners[0], scene_corners[1], cvScalar(0, 255, 0), 4 );

         line( tempMat1, scene_corners[1], scene_corners[2], cvScalar( 0, 255, 0), 4 );

         line( tempMat1, scene_corners[2], scene_corners[3], cvScalar( 0, 255, 0), 4 );

         line( tempMat1, scene_corners[3], scene_corners[0], cvScalar( 0, 255, 0), 4 );
     }

     // View matching..

     UIImage *resultimage = [UIImage imageWithCVMat:img_matches];
     UIImageView *imageview = [[UIImageView alloc] initWithImage:resultimage];
     imageview.frame = CGRectMake(0, 0, 320, 240);
     [self.view addSubview:imageview];

     // View Result

     UIImage *resultimage2 = [UIImage imageWithCVMat:tempMat1];
     UIImageView *imageview2 = [[UIImageView alloc] initWithImage:resultimage2];
     imageview2.frame = CGRectMake(0, 240, 320, 240);
     [self.view addSubview:imageview2];
}