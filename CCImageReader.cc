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

CCImageReader::CCImageReader():
    CCDataObject(),
    filename_(nullptr),
    data_(nullptr) { }

CCImageReader::CCImageReader(const char *filename, CCImageSourceType type) :
    CCDataObject(),
    filename_(filename),
    type_(type),
    desiredChannels_(CCColorChannels::DEFAULT) { }

CCImageReader::CCImageReader(const char *filename, CCImageSourceType type, CCColorChannels channels) :
    CCDataObject(),
    filename_(filename),
    type_(type),
    desiredChannels_(channels) { }

CCImageReader::CCImageReader(const CCImageReader &srcImg) {
    width_  = srcImg.width_;
    height_ = srcImg.height_;
    desiredChannels_ = srcImg.desiredChannels_;
    data_ = srcImg.data_;
    type_ = srcImg.type_;
}

CCImageReader::~CCImageReader() {}

void CCImageReader::setFilename(const char *filename) {
    filename_ = filename;
}

int CCImageReader::getWidth() {
    return width_;
}

void CCImageReader::setWidth(int width) {
    width_ = width;
}

int CCImageReader::getHeight() {
    return height_;
}

void CCImageReader::setHeight(int height) {
    height_ = height;
}

int CCImageReader::getNumChannels() {
    return numChannels_;
}

void CCImageReader::setNumChannels(int numChannels) {
    numChannels_ = numChannels;
}

CCColorChannels CCImageReader::getColorChannels() {
    return desiredChannels_;
}

void CCImageReader::setColorChannels(CCColorChannels colorChannel) {
    desiredChannels_ = colorChannel;
}

unsigned char* CCImageReader::getDataBlob() {
    return data_;
}

void CCImageReader::setDataBlob(unsigned char *data) {
    data_ = data;
}

int CCImageReader::getSize(void) {
    return width_ * height_;
}

CCImageSourceType CCImageReader::getFormat(void) {
    return type_;
}

void CCImageReader::setFormat(CCImageSourceType type) {
    type_ = type;
}

bool CCImageReader::Load() {
    if (type_ >= CCImageSourceType::UNSUPPORTED)
        goto error;

    switch (desiredChannels_) {
    case CCColorChannels::DEFAULT:
        data_ = stbi_load(filename_, &width_, &height_, &numChannels_, 0);
        break;
    case CCColorChannels::GRAY:
        data_ = stbi_load(filename_, &width_, &height_, &numChannels_, STBI_grey);
        break;
    case CCColorChannels::RGB:
        data_ = stbi_load(filename_, &width_, &height_, &numChannels_, STBI_rgb);
        break;
    case CCColorChannels::RGBA:
        data_ = stbi_load(filename_, &width_, &height_, &numChannels_, STBI_rgb_alpha);
        break;
    default:
        assert(0);
        break;
    }

    if (data_ != nullptr)
        return true;

error:
    return false;
}

std::string CCImageReader::generateUUIDName(void) {
    CCUUid ccuid = UUidInfo();
    std::string str = ccuid.getString();

    if (type_ == CCImageSourceType::PNG)
        str.append(".png");
    else if (type_ == CCImageSourceType::BMP) 
        str.append(".bmp");
    else if (type_ == CCImageSourceType::JPG) 
        str.append(".jpg");
    return str;
}

bool CCImageReader::Save() {
    std::string name;
    int ret, req_channels = static_cast<int> (desiredChannels_);

    if ((data_ == nullptr) || (width_ == 0) || (height_ == 0))
        goto error;

    if (!filename_) {
        name = generateUUIDName();
    } else
        name = std::string(filename_);

    switch (type_) {
    case CCImageSourceType::PNG:
        ret = stbi_write_png(name.c_str(), width_, height_, req_channels, data_, width_ * req_channels);
        break;
    case CCImageSourceType::BMP:
        ret = stbi_write_bmp(name.c_str(), width_, height_, req_channels, data_);
        break;
    case CCImageSourceType::JPG:
        ret = stbi_write_jpg(name.c_str(), width_, height_, req_channels, data_, CCJPEG_LOSS);
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

CCImageReader CCImageReader::ConvertRGB2GRAY(bool &ok) {
    CCImageReader newImg;
    CCColorChannels cchannels;
    unsigned char *pSrc, *pData;

    newImg.setWidth(getWidth());
    newImg.setHeight(getHeight());
    newImg.setFormat(getFormat());

    cchannels = getColorChannels();
    if (cchannels == CCColorChannels::RGBA) {
        newImg.setNumChannels(2);
        newImg.setColorChannels(CCColorChannels::GRAY2);
    } else if (cchannels == CCColorChannels::RGB) {
        newImg.setNumChannels(1);
        newImg.setColorChannels(CCColorChannels::GRAY);
    } else
        goto error;

    pData = (unsigned char *) malloc(sizeof(unsigned char) * newImg.getWidth() *
        newImg.getHeight() * newImg.getNumChannels());
    if (!pData)
        goto error;

    newImg.setDataBlob(pData);
    pSrc = getDataBlob();

    for (int count = 0; count < getSize(); pSrc += getNumChannels(),
            pData += newImg.getNumChannels(), count++) {
        *pData = (*pSrc + *(pSrc + 1) + *(pSrc + 2)) / 3.0;
        if (cchannels == CCColorChannels::RGBA)
            *(pData + 1) = *(pSrc + 3);
    }

    ok = true;
    return newImg;

error:
    ok = false;
    return newImg;
}

