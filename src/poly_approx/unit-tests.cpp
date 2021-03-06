/*
 * Copyright <2019> Saptarshi Sen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Unit-Tests for classes
 */

#include <set>
#include <stack>
#include <mutex>
#include <cassert>
#include <iostream>

#include "CCUUid.hpp"
#include "CCLogger.hpp"
#include "CCDataSet.hpp"
#include "CCImageReader.hpp"
#include "CCImageProcessor.hpp"
#include "CCDominatingPoints.hpp"
#include "CCConvexHull.hpp"
#include "CCErosionFilter.hpp"

#define MAX_UUIDS 100UL

// triangle
//#define RESULT_VERTICES 3

// square
//#define RESULT_VERTICES 4

// circle
#define RESULT_VERTICES 6

//#define TEST_IMAGE_DIR "images/triangles/"

//#define TEST_IMAGE_DIR "images/squares/"

#define TEST_IMAGE_DIR "images/circles/"

//#define TEST_IMAGE_PNG "images/triangles/drawing(4).png"

//#define TEST_IMAGE_PNG "images/squares/drawing(1).png"

#define TEST_IMAGE_PNG "images/circles/drawing(10).png"

//
// Logger
std::mutex CCLog::nanny_;
//
std::ofstream* CCLog::ostreamp_ = nullptr;
//
std::string CCLog::filename_;
//
CC_LOGLEVEL CCLog::level_ = CC_LOG_DEBUG;
//
bool CCConsoleLog::canLog = true;

int uuid_dup_test(void) {
    std::set<std::string> uuid_strings;

    for (unsigned long i = 0; i < MAX_UUIDS; i++) {
        CCUUid ccuid;
        std::string str(ccuid.getString());
        assert(uuid_strings.find(str) == uuid_strings.end());
        uuid_strings.insert(str);
        //std::cout << i << ":" << str << std::endl;
    }
    std::cout << __func__ << ":" <<  "pass" << std::endl;
    return 0;
}

int img_load_test(void) {
    CCImageReader im(TEST_IMAGE_PNG, CCImageSourceType::PNG);
    assert(im.Load());
    //assert(im.Save());
    assert(im.Destroy());
    std::cout << __func__ << ":" <<  "pass" << std::endl;
    return 0;
}

int img_convert_RGB2GRAY_test(void) {
    bool ok;
    CCImageReader im(TEST_IMAGE_PNG, CCImageSourceType::PNG, CCColorChannels::RGB);
    CCImageReader im2;
    assert(im.Load());
    im2 = im.ConvertRGB2GRAY(ok);
    assert(ok);
    assert(im2.Save());
    assert(im.Destroy());
    assert(im2.Destroy());
    std::cout << __func__ << ":" <<  "pass" << std::endl;
    return 0;
}

int dataset_file_load_test(void) {
    CCDataSet dataSet(TEST_IMAGE_PNG, CCDataSourceType::IMG);
    assert(dataSet.LoadFile());
    assert(dataSet.getNumRecords());
    dataSet.Destroy();
    std::cout << __func__ << ":" <<  "pass" << std::endl;
    return 0;
}

int dataset_dir_load_test(void) {
    CCDataSet dataSet(TEST_IMAGE_DIR, CCDataSourceType::IMG);
    assert(dataSet.LoadDirectory());
    assert(dataSet.getNumRecords());
    for (auto i : dataSet.dataItems_) {
        CCImageReader *im = dynamic_cast<CCImageReader*>(i);
        im->Load();
    }
    dataSet.Destroy();
    std::cout << __func__ << ":" <<  "pass" << std::endl;
    return 0;
}

int image_processor_test001(void) {
    CCImageProcessor imProcessor;
    CCImageProcessorBuilder imBuilder;
    CCImageReader imReal(TEST_IMAGE_PNG, CCImageSourceType::PNG, CCColorChannels::RGB), imGray;
    bool ok;

    assert(imReal.Load());
    imGray = imReal.ConvertRGB2GRAY(ok);
    assert(ok);
    imProcessor = imBuilder.addGaussianFilter(5, 5, 2.0)
                           .addSoebelFilter(3, 3, 1)
                           //.addMorphFilter(3, 3, 255)
                           .build();
    imProcessor.Run(imGray);
    assert(imGray.Save());
    assert(imGray.Destroy());
    assert(imReal.Destroy());
    std::cout << __func__ << ":" <<  "pass" << std::endl;
    return 0;
}

int image_processor_test002(void) {
    CCImageProcessor imProcessor;
    CCImageProcessorBuilder imBuilder;
    CCImageReader imReal(TEST_IMAGE_PNG, CCImageSourceType::PNG, CCColorChannels::RGB), imGray;
    bool ok;

    assert(imReal.Load());
    imGray = imReal.ConvertRGB2GRAY(ok);
    assert(ok);

    imProcessor = imBuilder.addGaussianFilter(5, 5, 2.0)
                           .addSoebelFilter(3, 3, 1)
                           .addFeatureExtractor(5)
                           .build();
    imProcessor.Run(imGray);
    assert(imGray.Save());
    assert(imGray.Destroy());
    assert(imReal.Destroy());
    std::cout << __func__ << ":" <<  "pass" << std::endl;
    return 0;
}

int image_processor_test003(int matchValue) {
    CCImageProcessor imProcessor;
    CCImageProcessorBuilder imBuilder;
    CCImageReader imReal(TEST_IMAGE_PNG, CCImageSourceType::PNG, CCColorChannels::RGB), imGray;
    std::vector<int> result {matchValue};
    bool ok;

    assert(imReal.Load());
    imGray = imReal.ConvertRGB2GRAY(ok);
    assert(ok);
    imProcessor = imBuilder.addGaussianFilter(5, 5, 2.0)
                           .addSoebelFilter(3, 3, 1)
                           .addThresholding(60)
                           //.addMorphFilter(3, 3, 80)
                           .addFeatureExtractor(1)
                           .build();
    imProcessor.Run(imGray);
    if (imProcessor.Classify(imGray, result))
        std::cout << __func__ << ":" <<  "pass" << std::endl;
    else
        assert(0);
    assert(imGray.Save());
    assert(imGray.Destroy());
    assert(imReal.Destroy());
    return 0;
}

int image_processor_test004(int matchValue) {
    int matchCount = 0, totalCount = 0;
    CCDataSet dataSet(TEST_IMAGE_DIR, CCDataSourceType::IMG);
    assert(dataSet.LoadDirectory());
    assert(dataSet.getNumRecords());
    for (auto i : dataSet.dataItems_) {
        bool ok;
        std::vector<int> result {matchValue};
        CCImageProcessor imProcessor;
        CCImageProcessorBuilder imBuilder;
        imProcessor = imBuilder.addGaussianFilter(5, 5, 2.0)
                               .addSoebelFilter(3, 3, 1)
                               .addThresholding(60)
                               //.addMorphFilter(3, 3, 150)
                               .addFeatureExtractor(1)
                               .build();
        CCImageReader *im = dynamic_cast<CCImageReader*>(i);
        assert(im->Load());
        CC_INFO("processing image ", im->getFilename());
        CONSOLE_INFO("processing image ", im->getFilename());
        CCImageReader imGray = im->ConvertRGB2GRAY(ok);
        assert(ok);
        imProcessor.Run(imGray);
        if (imProcessor.Classify(imGray, result))
            matchCount++;
        totalCount++;
        assert(imGray.Save());
        assert(imGray.Destroy());
        assert(im->Destroy());
        std::cout << matchCount << "/" << totalCount << std::endl;
    }
    dataSet.Destroy();
    std::cout << __func__ << ":" <<  "pass" << std::endl;
    return 0;
}

int polygon_approx_test(void) {
    Prng<int> prng;
    std::list<Pixel<int>> pixels;
    std::list<Pixel<int>> upperhull;
    std::list<Pixel<int>> lowerhull;
    std::list<Pixel<int>> convexhull;
    std::list<Pixel<int>> polyPointsApprox, polyPoints;

    for (int i = 0; i < 100; i++) {
        Pixel<int> p(prng.next_random() % 50,
                     prng.next_random() % 50);
        pixels.push_back(p);
    }

    MakeConvexHullUpper(pixels, upperhull);
    MakeConvexHullLower(pixels, lowerhull);

    lowerhull.reverse();
    lowerhull.pop_front();
    lowerhull.pop_back();

    convexhull.splice(convexhull.begin(), upperhull);
    convexhull.splice(convexhull.end(), lowerhull);
    for (auto &p : convexhull) {
        std::cout << "HULL " << p.getX() << " " << p.getY() << std::endl;
    } 

    polyPointsApprox.push_back(*convexhull.begin());
    polyPointsApprox.push_back(*convexhull.rbegin());

    approxPolyDP(convexhull,
                *convexhull.begin(),
                *convexhull.rbegin(),
                 polyPointsApprox,
                 15);

    for (auto &p : convexhull) {
        if (std::find(polyPointsApprox.begin(), polyPointsApprox.end(), p)
                != polyPointsApprox.end()) {
            polyPoints.push_back(p);
            std::cout << "APPROX " << p.getX() << " " << p.getY() << std::endl;
        }
    }

    std::cout << "size1 :" << polyPoints.size() << std::endl;
    MergePoints<int>(polyPoints, 15);

    std::cout << "size2 :" << polyPoints.size() << std::endl;
    MergePoints<int>(polyPoints, 15);

    std::cout << "size3 :" << polyPoints.size() << std::endl;
    for (auto &p : polyPoints) {
        std::cout << "POLY " << p.getX() << " " << p.getY() << std::endl;
    } 

    DominatingPoints<int>(polyPoints, 0, 1000, 180);
    return 0;
}

int main(void) {
    CCLog::Initialize(CC_LOGFILE);
    CCLog::SetLogLevel(CC_LOG_INFO);
#if 0
    uuid_dup_test();
    img_load_test();
    img_convert_RGB2GRAY_test();
    dataset_file_load_test();
    dataset_dir_load_test();
    polygon_approx_test();
    image_processor_test001();
    image_processor_test002();
    image_processor_test003(RESULT_VERTICES);
#endif
    image_processor_test004(RESULT_VERTICES);
    return 0;
}
