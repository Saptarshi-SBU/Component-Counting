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
#include <cassert>
#include <iostream>

#include "CCUUid.hpp"
#include "CCDataSet.hpp"
#include "CCImageReader.hpp"

#define MAX_UUIDS 100UL

#define TEST_IMAGE_DIR "images/shapes/triangles/"

#define TEST_IMAGE_PNG "images/shapes/triangles/drawing(4).png"

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

int main(void) {
    uuid_dup_test();
    img_load_test();
    img_convert_RGB2GRAY_test();
    dataset_file_load_test();
    dataset_dir_load_test();
    return 0;
}
