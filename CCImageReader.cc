/*
 * MIT License
 *
 * Copyright (c) 2019 Saptarshi Sen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 *  DataSet : An immutable container of data. The container can be fed from
 *  data prepared offline (e.g. image files, *.dat files) or data provided
 *  at run-time (say data from camera frame).
 *
 */

#include "CCImageReader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

CCImageReader::CCImageReader(const char *filename, CCImageSourceType type) :
    CCDataObject(),
    filename_(filename),
    type_(type) { }

CCImageReader::CCImageReader(const char *filename, CCImageSourceType type,
    int width, int height, int numChannels, unsigned char* data ) :
    CCDataObject(),
    filename_(filename),
    type_(type),
    width_(width),
    height_(height),
    numChannels_(numChannels),
    data_(data) { }
    
CCImageReader::CCImageReader(const CCImageReader &srcImg) {
    width_  = srcImg.width_;
    height_ = srcImg.height_;
    data_   = srcImg.data_;
    type_   = srcImg.type_;
}

CCImageReader::~CCImageReader() {}

int CCImageReader::getWidth() {
    return width_;
}

int CCImageReader::getHeight() {
    return height_;
}

int CCImageReader::getNumChannels() {
    return numChannels_;
}

unsigned char* CCImageReader::getDataBlob() {
    return data_;
}

bool CCImageReader::Load() {
    if (type_ >= CCImageSourceType::UNSUPPORTED)
        goto error;

    data_ = stbi_load(filename_, &width_, &height_, &numChannels_, STBI_rgb_alpha);
    if (data_ == nullptr)
        goto error;

    return true;
error:
    return false;
}

bool CCImageReader::Save() {
    int ret;
    if (data_ == nullptr)
        goto error;

    switch (type_) {
    case CCImageSourceType::PNG:
        ret = stbi_write_png(filename_, width_, height_, numChannels_, data_, width_ * numChannels_);
        break;
    case CCImageSourceType::BMP:
        ret = stbi_write_bmp(filename_, width_, height_, numChannels_, data_);
        break;
    case CCImageSourceType::JPG:
        ret = stbi_write_jpg(filename_, width_, height_, numChannels_, data_, 100);
    default:
        ret = 0;
        break;
    }
    return ret > 0;
error:
    return false;
}

bool CCImageReader::Destroy() {
    if (data_) {
        stbi_image_free(data_);
        data_= nullptr;
    }
    return true;
}
